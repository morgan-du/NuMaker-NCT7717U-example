/*
 * Copyright (c) 2023, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#ifndef __NCT7717U_H__
#define __NCT7717U_H__

#include "drivers/I2C.h"
#include "drivers/DigitalIn.h"
#include "PinNames.h"
#include "platform/mbed_chrono.h"
#include "platform/mbed_error.h"
#include "platform/mbed_mem_trace.h"
#include "platform/Callback.h"
#include "rtos/Mutex.h"
#include "rtos/ThisThread.h"


#define NCT7717U_SLAVE_ADDR     0x48

/* NCT7717U Register Map */
#define NCT7717U_LDT_READOUT                0x00
#define NCT7717U_ALERT_STATUS               0x02
#define NCT7717U_READ_CONFIG                0x03
#define NCT7717U_READ_CONV_RATE             0x04
#define NCT7717U_READ_LT_HIGH_ALERT_TEMP    0x05
#define NCT7717U_WRITE_CONFIG               0x09
#define NCT7717U_WRITE_CONV_RATE            0x0A
#define NCT7717U_WRITE_LT_HIGH_ALERT_TEMP   0x0B
#define NCT7717U_ONE_SHOT_CONV              0x0F
#define NCT7717U_CUST_DATA_LOG_REG1         0x2D
#define NCT7717U_CUST_DATA_LOG_REG2         0x2E
#define NCT7717U_CUST_DATA_LOG_REG3         0x2F
#define NCT7717U_ALERT_MODE                 0xBF
#define NCT7717U_CHIP_ID                    0xFD
#define NCT7717U_VENDOR_ID                  0xFE
#define NCT7717U_DEVICE_ID                  0xFF

/** NCT7717U class.
    This is the class to contorl NCT7717U temperature sensor

    NOTICE: The class does not support interrupt alert yet.
 */
class NCT7717U {
public:
    NCT7717U(PinName sda, PinName scl, PinName alert = NC, int freq = 400000, int addr = NCT7717U_SLAVE_ADDR, bool debug = false);
    ~NCT7717U();

    /**
    * Get current measured temperature in Celsius
    *
    * @return the temperature in Celsius
    */
    int get_termperature();

    /**
    * Get Chip ID
    *
    * @return the chip ID
    */
    int get_cid();

    /**
    * Get Vendor ID
    *
    * @return the vendor ID
    */
    int get_vid();

    /**
    * Get Device ID
    *
    * @return the Device ID
    */
    int get_did();

    /**
    * Set Configuration register
    *
    * @param conf is configuration value.
    *             bit 7: ALERT_MSK
    *             bit 6: STOP_MNT
    *             bit 0: EN_FaultQuere
    */
    void set_configuration(int conf);

    /**
    * Get Configuration register
    *
    * @return configuration value.
    *             bit 7: ALERT_MSK
    *             bit 6: STOP_MNT
    *             bit 0: EN_FaultQuere
    */
    int get_configuration();

    /**
    * Set Conversion Rate
    *
    * @param rate is 0 for 0.0625Hz or 16 secs
    *                1 for  0.125Hz or 8 secs
    *                2 for   0.25Hz or 4 secs
    *                3 for    0.5Hz or 2 secs
    *                4 for      1Hz or 1 sec
    *                5 for      2Hz or 0.5 secs
    *                6 for      4Hz or 0.25 secs
    *                7 for      8Hz or 0.125 secs
    *                8 for     16Hz or 0.0625 secs (Default)
    */
    void set_conversion_rate(int rate);

    /**
    * Get Conversion Rate
    *
    * @return Conversion Rate
    */
    int get_conversion_rate();

    /**
    * Set high alert temperature
    *
    * @param temp is the high temperature
    */
    void set_alert_temperature(int temp);

    /**
    * Get high alert temperature
    *
    * @return high alert temperature
    */
    int get_alert_temperature();

    /**
    * Trigger one shot conversion (only at stop monitor state)
    */
    void one_shot_conversion();


    /**
    * Set Customer Data Log
    *
    * @param i is the index from 1 to 3 for three log registers
    * @param data is the value set to the log register
    */
    void set_data_log(int i, char data);

    /**
    * Get Customer Data Log
    *
    * @param i is the index from 1 to 3 for three log registers
    * @return the value in the log register
    */
    int get_data_log(int i);

    /**
    * Set Alert mode
    *
    * @param mode is 0 for interrupt or SMBus alert mode.
    *                1 for comparator mode.
    */
    void set_alert_mode(int mode);

    /**
    * Get Alert mode
    *
    * @return the alert mode. Please refer set_alert_mode()
    */
    int get_alert_mode();

    /**
    * Get Alert status
    *
    * @return true if temperature is higher than alert temperature.
    */
    int get_alert_status();

private:
    bool debug;
    mbed::I2C _i2c; 
    int _addr;
    mbed::DigitalIn _alert;
//    bool initialized;
//    rtos::Mutex _mutex;

    char read_reg(char cmd);
    void write_reg(char cmd, char data);
};

#endif // __NCT7717U_H__
