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

#ifdef TARGET_NUMAKER_IOT_M467
NCT7717U TempSensor(PD_0, PD_1);

#else // for other boards
NCT7717U TempSensor(I2C_SDA, I2C_SCL);
#endif


int main()
{
    printf("NCT7717U demo program.\r\n");

    printf("Chip ID is %02X.\r\n", TempSensor.get_cid());
    printf("Vendor ID is %02X.\r\n", TempSensor.get_vid());
    printf("Device ID is %02X.\r\n", TempSensor.get_did());

    while (true) {
        printf("Curent Temperature is %d.\r\n", TempSensor.get_termperature());
        ThisThread::sleep_for(1s);
    }
}
