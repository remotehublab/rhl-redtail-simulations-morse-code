/*
 * Copyright (C) 2023 onwards LabsLand, Inc.
 * All rights reserved.
 *
 * This software is licensed as described in the file LICENSE, which
 * you should have received as part of this distribution.
 */
#ifndef LL_PROTOCOLS
#define LL_PROTOCOLS

/*
 * In this file we keep utilities for interacting with certain protocols, such as
 * GPIO, I2C or in the future any other (SPI...)
 */

namespace LabsLand::Protocols {

    //
    //    GPIO
    //

    /*
     * GPIO: these names are custom names that might depend on each particular device.
     * We will assign for example that maybe customSerialLatch is GPIO 1 in one board and GPIO 4
     * in another board. To refer to them using a high level protocol, we use NamedGpio.
     */
    enum NamedGpio {
        // custom serial
        customSerialLatch,
        customSerialDataOut,
        customSerialPulse,
        // other custom protocols
    };


    //
    //    I2C
    //

    class I2C_IO_Wrapper {
        virtual unsigned char readByte() = 0;
        virtual void writeByte(unsigned char byte) = 0;
    };

    enum I2CEventType {
        slaveReceive,
        slaveRequest,
        slaveFinish,
        other // to be completed in the future if more states appear
    };

    typedef void (*i2c_slave_callback)(I2C_IO_Wrapper * i2cWrapper, I2CEventType event);
}

#endif
