/*
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mqtt_tests.h"
#include "greentea-client/test_env.h"
#include "utest.h"

using namespace utest::v1;

Case cases[] = {
    Case("MQTT_CONNECT", MQTT_CONNECT),
    Case("MQTT_CONNECT_NEW", MQTT_CONNECT_NEW),
    Case("MQTT_CONNECT_TEMPLATES", MQTT_CONNECT_TEMPLATES),
};

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
#ifdef MBED_GREENTEA_TEST_INTERFACE_TIMEOUT_S
    GREENTEA_SETUP(MBED_GREENTEA_TEST_INTERFACE_TIMEOUT_S, "default_auto");
#else
    GREENTEA_SETUP(480, "default_auto");
#endif
    return verbose_test_setup_handler(number_of_cases);
}

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
