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
 
#include "mbed.h"
#include "NCT7717U.h"

NCT7717U::NCT7717U(PinName sda, PinName scl, PinName alert, int freq, int addr, bool debug) :
    _i2c(sda, scl), _addr(addr), _alert(alert), debug(debug)
{
    _i2c.frequency(freq);
}

char NCT7717U::read_reg(char cmd)
{
    char buf[2];
    buf[0] = cmd;

    _i2c.write(_addr<<1, buf, 1);
    _i2c.read(_addr<<1, buf, 1);

    return buf[0];
}

void NCT7717U::write_reg(char cmd, char data)
{
    char buf[2];
    buf[0] = cmd;
    buf[1] = data;

    _i2c.write(_addr<<1, buf, 2);
}

int NCT7717U::get_termperature()
{
    return (int)read_reg(NCT7717U_LDT_READOUT);
}

int NCT7717U::get_cid()
{
    return (int)read_reg(NCT7717U_CHIP_ID);
}

int NCT7717U::get_vid()
{
    return (int)read_reg(NCT7717U_VENDOR_ID);
}

int NCT7717U::get_did()
{
    return (int)read_reg(NCT7717U_DEVICE_ID);
}

void NCT7717U::set_configuration(int conf)
{
    write_reg(NCT7717U_WRITE_CONFIG, conf);
}

int NCT7717U::get_configuration()
{
    return (int)(read_reg(NCT7717U_READ_CONFIG) & 0xC1);
}

void NCT7717U::set_conversion_rate(int rate)
{
    if (rate < 0 || rate > 8)
    {
        if (debug)
            printf("NCT7717U: Error in Setting Rate.\r\n");
        return;
    }

    write_reg(NCT7717U_WRITE_CONV_RATE, rate);
}

int NCT7717U::get_conversion_rate()
{
    return (int)(read_reg(NCT7717U_READ_CONV_RATE) & 0x0F);
}

void NCT7717U::set_alert_temperature(int temp)
{
    write_reg(NCT7717U_WRITE_LT_HIGH_ALERT_TEMP, temp);
}

int NCT7717U::get_alert_temperature()
{
    return (int)read_reg(NCT7717U_READ_LT_HIGH_ALERT_TEMP);
}

void NCT7717U::one_shot_conversion()
{
    write_reg(NCT7717U_ONE_SHOT_CONV, 0);
}

void NCT7717U::set_data_log(int i, char data)
{
    if (i < 1 || i > 3)
    {
        if (debug)
            printf("NCT7717U: Error in Setting Data Log.\r\n");
        return;
    }

    write_reg(NCT7717U_CUST_DATA_LOG_REG1 - 1 + i, data);
}

int NCT7717U::get_data_log(int i)
{
    if (i < 1 || i > 3)
    {
        if (debug)
            printf("NCT7717U: Error in Geting Data Log.\r\n");
        return 0;
    }

    return (int)read_reg(NCT7717U_CUST_DATA_LOG_REG1 - 1 + i);
}

void NCT7717U::set_alert_mode(int mode)
{
    if (mode)
        mode = 1;

    write_reg(NCT7717U_ALERT_MODE, mode);

    if (mode == 0)
        write_reg(0x21, 0);
}

int NCT7717U::get_alert_mode()
{
    return (int)read_reg(NCT7717U_ALERT_MODE);
}

int NCT7717U::get_alert_status()
{
    bool s = ((read_reg(NCT7717U_ALERT_STATUS) & 0x40) != 0);
    return s;
}
