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

#ifndef MICROBIT_IO_H
#define MICROBIT_IO_H

#include "mbed.h"
#include "CodalConfig.h"
#include "NRF52Pin.h"
#include "PinNames.h"
//
// Unique Pin number for each pin (synonymous with mbedos PinName)
//

// IDENTITY MAPPINGS
#define P0_00   P0_0
#define P0_01   P0_1
#define P0_02   P0_2
#define P0_03   P0_3
#define P0_04   P0_4
#define P0_05   P0_5
#define P0_06   P0_6
#define P0_07   P0_7
#define P0_08   P0_8
#define P0_09   P0_9

#define P1_00   P1_0
#define P1_01   P1_1
#define P1_02   P1_2
#define P1_03   P1_3
#define P1_04   P1_4
#define P1_05   P1_5
#define P1_06   P1_6
#define P1_07   P1_7
#define P1_08   P1_8
#define P1_09   P1_9



//Edge Connector
#define MICROBIT_PIN_P0                             P0_02
#define MICROBIT_PIN_P1                             P0_03
#define MICROBIT_PIN_P2                             P0_04
#define MICROBIT_PIN_P3                             P0_31
#define MICROBIT_PIN_P4                             P0_28
#define MICROBIT_PIN_P5                             P0_14
#define MICROBIT_PIN_P6                             P1_05
#define MICROBIT_PIN_P7                             P0_11
#define MICROBIT_PIN_P8                             P0_10
#define MICROBIT_PIN_P9                             P0_09
#define MICROBIT_PIN_P10                            P0_30
#define MICROBIT_PIN_P11                            P0_23
#define MICROBIT_PIN_P12                            P0_12
#define MICROBIT_PIN_P13                            P0_17
#define MICROBIT_PIN_P14                            P0_01
#define MICROBIT_PIN_P15                            P0_13
#define MICROBIT_PIN_P16                            P1_02
#define MICROBIT_PIN_P19                            P0_26
#define MICROBIT_PIN_P20                            P1_00

// User Input
#define MICROBIT_PIN_BUTTON_A                       P0_14
#define MICROBIT_PIN_BUTTON_B                       P0_23
#define MICROBIT_PIN_FACE_TOUCH                     P1_04

// Audio
#define MICROBIT_PIN_RUN_MIC                        P0_20
#define MICROBIT_PIN_MIC_IN                         P0_05
#define MICROBIT_PIN_SPEAKER                        P0_00

// Display
#define MICROBIT_PIN_ROW1                           P0_21
#define MICROBIT_PIN_ROW2                           P0_22
#define MICROBIT_PIN_ROW3                           P0_15
#define MICROBIT_PIN_ROW4                           P0_24
#define MICROBIT_PIN_ROW5                           P0_19
#define MICROBIT_PIN_COL1                           P0_28
#define MICROBIT_PIN_COL2                           P0_11
#define MICROBIT_PIN_COL3                           P0_31
#define MICROBIT_PIN_COL4                           P1_05
#define MICROBIT_PIN_COL5                           P0_30

// I2C (internal)
#define MICROBIT_PIN_INT_SDA                        P0_16
#define MICROBIT_PIN_INT_SCL                        P0_08
#define MICROBIT_PIN_SENSOR_DATA_READY              P0_25
// I2C (external)
#define MICROBIT_PIN_EXT_SDA                        P1_00
#define MICROBIT_PIN_EXT_SCL                        P0_26

// SPI
#define MICROBIT_PIN_MOSI                           P0_13
#define MICROBIT_PIN_MISO                           P0_01
#define MICROBIT_PIN_SCK                            P0_17

// UART
#define MICROBIT_PIN_UART_TX                        P0_06
#define MICROBIT_PIN_UART_RX                        P1_08

// Unused
#define MICROBIT_PIN_BUTTON_RESET                   -1

//
// Component IDs for each pin.
// The can be user defined, but uniquely identify a pin when using the eventing APIs/
//
#define ID_PIN_P0        (DEVICE_ID_IO_P0 + 0)
#define ID_PIN_P1        (DEVICE_ID_IO_P0 + 1)
#define ID_PIN_P2        (DEVICE_ID_IO_P0 + 2)
#define ID_PIN_P3        (DEVICE_ID_IO_P0 + 3)
#define ID_PIN_P4        (DEVICE_ID_IO_P0 + 4)
#define ID_PIN_P5        (DEVICE_ID_IO_P0 + 5)
#define ID_PIN_P6        (DEVICE_ID_IO_P0 + 6)
#define ID_PIN_P7        (DEVICE_ID_IO_P0 + 7)
#define ID_PIN_P8        (DEVICE_ID_IO_P0 + 8)
#define ID_PIN_P9        (DEVICE_ID_IO_P0 + 9)
#define ID_PIN_P10       (DEVICE_ID_IO_P0 + 10)
#define ID_PIN_P11       (DEVICE_ID_IO_P0 + 11)
#define ID_PIN_P12       (DEVICE_ID_IO_P0 + 12)
#define ID_PIN_P13       (DEVICE_ID_IO_P0 + 13)
#define ID_PIN_P14       (DEVICE_ID_IO_P0 + 14)
#define ID_PIN_P15       (DEVICE_ID_IO_P0 + 15)
#define ID_PIN_P16       (DEVICE_ID_IO_P0 + 16)
#define ID_PIN_P17       (DEVICE_ID_IO_P0 + 17)
#define ID_PIN_P18       (DEVICE_ID_IO_P0 + 18)
#define ID_PIN_P19       (DEVICE_ID_IO_P0 + 19)
#define ID_PIN_P20       (DEVICE_ID_IO_P0 + 20)
#define ID_PIN_P21       (DEVICE_ID_IO_P0 + 21)
#define ID_PIN_P22       (DEVICE_ID_IO_P0 + 22)
#define ID_PIN_P23       (DEVICE_ID_IO_P0 + 23)
#define ID_PIN_P24       (DEVICE_ID_IO_P0 + 24)
#define ID_PIN_P25       (DEVICE_ID_IO_P0 + 25)
#define ID_PIN_P26       (DEVICE_ID_IO_P0 + 26)
#define ID_PIN_P27       (DEVICE_ID_IO_P0 + 27)
#define ID_PIN_P28       (DEVICE_ID_IO_P0 + 28)
#define ID_PIN_P29       (DEVICE_ID_IO_P0 + 29)
#define ID_PIN_P30       (DEVICE_ID_IO_P0 + 30)
#define ID_PIN_P31       (DEVICE_ID_IO_P0 + 31)

#define ID_PIN_P32       (DEVICE_ID_IO_P0 + 32)
#define ID_PIN_P33       (DEVICE_ID_IO_P0 + 33)
#define ID_PIN_P34       (DEVICE_ID_IO_P0 + 34)
#define ID_PIN_P35       (DEVICE_ID_IO_P0 + 35)
#define ID_PIN_P36       (DEVICE_ID_IO_P0 + 36)
#define ID_PIN_P37       (DEVICE_ID_IO_P0 + 37)
#define ID_PIN_P38       (DEVICE_ID_IO_P0 + 38)
#define ID_PIN_P39       (DEVICE_ID_IO_P0 + 39)
#define ID_PIN_P40       (DEVICE_ID_IO_P0 + 40)
#define ID_PIN_P41       (DEVICE_ID_IO_P0 + 41)
#define ID_PIN_P42       (DEVICE_ID_IO_P0 + 42)
#define ID_PIN_P43       (DEVICE_ID_IO_P0 + 43)
#define ID_PIN_P44       (DEVICE_ID_IO_P0 + 44)
#define ID_PIN_P45       (DEVICE_ID_IO_P0 + 45)
#define ID_PIN_P46       (DEVICE_ID_IO_P0 + 46)
#define ID_PIN_P47       (DEVICE_ID_IO_P0 + 47)



namespace codal
{
    /**
     * Represents a collection of all I/O pins exposed by the device.
     */
    class MicroBitIO
    {
        public:

            NRF52Pin          pin[0];
            NRF52Pin          speaker;      // P0_00
            NRF52Pin          P14;          // P0_01
            NRF52Pin          P0;           // P0_02
            NRF52Pin          P1;           // P0_03
            NRF52Pin          P2;           // P0_04
            NRF52Pin          microphone;   // P0_05
            NRF52Pin          uart_rx;      // P0_06
            NRF52Pin          unused1;      // P0_07
            NRF52Pin          scl;          // P0_08
            NRF52Pin          P9;           // P0_09
            NRF52Pin          P8;           // P0_10
            NRF52Pin          P7;           // P0_11
            NRF52Pin          P12;          // P0_12
            NRF52Pin          P15;          // P0_13
            NRF52Pin          P5;           // P0_14
            NRF52Pin          row3;         // P0_15
            NRF52Pin          sda;          // P0_16
            NRF52Pin          P13;          // P0_17
            NRF52Pin          unused2;      // P0_18
            NRF52Pin          row5;         // P0_19
            NRF52Pin          runmic;       // P0_20
            NRF52Pin          row1;         // P0_21
            NRF52Pin          row2;         // P0_22
            NRF52Pin          P11;          // P0_23
            NRF52Pin          row4;         // P0_24
            NRF52Pin          irq1;         // P0_25
            NRF52Pin          P19;          // P0_26
            NRF52Pin          unused3;      // P0_27
            NRF52Pin          P4;           // P0_28
            NRF52Pin          unused4;      // P0_29
            NRF52Pin          P10;          // P0_30
            NRF52Pin          P3;           // P0_31

            NRF52Pin          P20;          // P1_00
            NRF52Pin          unused5;      // P1_01
            NRF52Pin          P16;          // P1_02
            NRF52Pin          unused6;      // P1_03
            NRF52Pin          face;         // P1_04
            NRF52Pin          P6;           // P1_05

            NRF52Pin&         col1;
            NRF52Pin&         col2;
            NRF52Pin&         col3;
            NRF52Pin&         col4;
            NRF52Pin&         col5;
            NRF52Pin&         buttonA;
            NRF52Pin&         buttonB;

            NRF52Pin          usbTx;
            NRF52Pin          usbRx;

            /**
             * Constructor.
             *
             * Create a representation of all given I/O pins on the edge connector
             *
             * Accepts a sequence of unique ID's used to distinguish events raised
             * by MicroBitPin instances on the default EventModel.
             */
            MicroBitIO();
    };
}

#endif