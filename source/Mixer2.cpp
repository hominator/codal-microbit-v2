/*
The MIT License (MIT)

Copyright (c) 2017 Lancaster University.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "Mixer2.h"
#include "StreamNormalizer.h"
#include "ErrorNo.h"
#include "CodalDmesg.h"

using namespace codal;

volatile int thingy;

Mixer2::Mixer2(int format)
{
    // Set valid defaults.
    this->channels = NULL;
    this->downStream = NULL;
    this->outputFormat = DATASTREAM_FORMAT_16BIT_UNSIGNED;
    this->bytesPerSampleOut = 2;
    this->volume = 1.0f;
    this->outputRange = CONFIG_MIXER_INTERNAL_RANGE;
    this->orMask = 0;

    // Attempt to configure output format to requested value
    this->setFormat(format);
}

Mixer2::~Mixer2()
{
    while (channels)
    {
        MixerChannel *n = channels;
        channels = n->next;
        n->stream->disconnect();
        delete n;
    }
}

void Mixer2::configureChannel(MixerChannel *c)
{
    c->volume = 1.0f;
    c->format = c->stream->getFormat();
    c->bytesPerSample = DATASTREAM_FORMAT_BYTES_PER_SAMPLE(c->format);
    c->gain = CONFIG_MIXER_INTERNAL_RANGE / (float) c->range;
    c->offset = 0.0f;

    if (c->format == DATASTREAM_FORMAT_8BIT_UNSIGNED || c->format == DATASTREAM_FORMAT_16BIT_UNSIGNED)
        c->offset = c->range * -0.5f;       
}

MixerChannel *Mixer2::addChannel(DataSource &stream, int range)
{
    MixerChannel *c = new MixerChannel();
    c->stream = &stream;
    c->range = range;
    c->pullRequests = 0;
    c->in = NULL;
    configureChannel(c);

    // Add channel to list.
    c->next = channels;
    channels = c;
    
    // Connect channel to the upstream source.
    stream.connect(*c);
    return c;
}

ManagedBuffer Mixer2::pull() 
{
    // If we have no channels, just return an empty buffer.
    if (!channels)
    {
        downStream->pullRequest();
        return ManagedBuffer(CONFIG_MIXER_BUFFER_SIZE);
    }

    // Clear the accumulator buffer
    for (int i=0; i<CONFIG_MIXER_BUFFER_SIZE/2; i++)
        mix[i] = 0.0f;

    MixerChannel *next;

    for (MixerChannel *ch = channels; ch; ch = next) {
        next = ch->next; // save next in case the current channel gets deleted

        // Attempt to discover the stream format if it is not already defined.
        if (ch->format == DATASTREAM_FORMAT_UNKNOWN)
        {
            configureChannel(ch);

            // If we still don't know, skip this channel until it decides what it is generating...
            if (ch->format == DATASTREAM_FORMAT_UNKNOWN)      
                continue;
        }

        float *out = &mix[0];
        float *end = &mix[CONFIG_MIXER_BUFFER_SIZE/2];
        int inputFormat = ch->format;

        while (out < end)
        {
            int len =  min(CONFIG_MIXER_BUFFER_SIZE / bytesPerSampleOut, (ch->end - ch->in) / ch->bytesPerSample);
            uint8_t *d = ch->in;

            while(len--)
            {
                float v = StreamNormalizer::readSample[inputFormat](d);
                v += ch->offset;
                v *= ch->gain;    
                v *= ch->volume;    
                *out += v;
 
                d += ch->bytesPerSample;
                out++;
            }

            ch->in = d;

            // Check if we've completed an input buffer. If so, pull down another if available.
            // if no buffer is available, then move on to the next channel.
            if (ch->in >= ch->end)
            {
                if (ch->pullRequests)
                {
                    ch->pullRequests--;
                    ch->buffer = ch->stream->pull();
                    ch->in = &ch->buffer[0];
                    ch->end = ch->in + ch->buffer.length();
                }
                else
                {
                    break;
                }
            }                
        }
    }       

    // Scale and pack to our output format
    ManagedBuffer output = ManagedBuffer(CONFIG_MIXER_BUFFER_SIZE);
    uint8_t *w = &output[0];
    float *r = mix;

    int len = output.length() / bytesPerSampleOut;
    float scale = volume * outputRange / CONFIG_MIXER_INTERNAL_RANGE;
    int offset = (outputFormat == DATASTREAM_FORMAT_16BIT_UNSIGNED) ? outputRange/2 : 0;
    float lo = (outputFormat == DATASTREAM_FORMAT_16BIT_UNSIGNED) ? 0 : -outputRange/2;
    float hi = (outputFormat == DATASTREAM_FORMAT_16BIT_UNSIGNED) ? outputRange : outputRange/2;

    while(len--)
    {
        float sample = *r * scale;
        sample += offset;
        
        if (sample < lo)
            sample = lo;
        if (sample > hi)
            sample = hi;

        // Apply any requested bit mask
        int s = (int)sample;
        s |= orMask;

        // Write out the sample.
        StreamNormalizer::writeSample[outputFormat](w, s);
        w += bytesPerSampleOut;
        r++;
    }

    // Return the buffer and we're done.
    downStream->pullRequest();
    return output;
}

int MixerChannel::pullRequest()
{
    pullRequests++;
    return DEVICE_OK;
}

void Mixer2::connect(DataSink &sink)
{
    this->downStream = &sink;
    this->downStream->pullRequest();
}

int Mixer2::getFormat()
{
    return outputFormat;
}
    
int Mixer2::setFormat(int format)
{
    if (format == DATASTREAM_FORMAT_16BIT_UNSIGNED || format == DATASTREAM_FORMAT_16BIT_SIGNED)
    {
        this->outputFormat = format;
        this->bytesPerSampleOut = DATASTREAM_FORMAT_BYTES_PER_SAMPLE(format);

        return DEVICE_OK;
    }

    return DEVICE_INVALID_PARAMETER;
}

/**
 * Defines the overall master volume of the mixer.
 *
 * @param volume The master volume of this mixer, in the range 0..1023.
 * @return DEVICE_OK on success.
 */
int
Mixer2::setVolume(int volume)
{
    if (volume < 0 || volume > 1023)
        return DEVICE_INVALID_PARAMETER;

    this->volume = (float)volume / 1023.f;
    return DEVICE_OK;
}

/**
 * Determines the overall master volume of the mixer.
 * @return the voluem level in the range 0..1023.
 */
int
Mixer2::getVolume()
{
    return (int) (this->volume * 1023.0f);
}

/**
 * Change the sample range used by this Synthesizer,
 * @param sampleRange The new sample range, that defines by the maximum sample value that will be output by the mixer.
 * @return DEVICE_OK on success.
 */
int Mixer2::setSampleRange(uint16_t sampleRange)
{
    this->outputRange = (float)sampleRange;
    return DEVICE_OK;
}

/**
 * Defines an optional bit mask to logical OR with each sample.
 * Useful if the downstream component encodes control data within its samples.
 *
 * @param mask The bitmask to to apply to each sample.
 * @return DEVICE_OK on success.
 */
int Mixer2::setOrMask(uint32_t mask)
{
    orMask = mask;
    return DEVICE_OK;
}