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

#include <MQTTNetwork.h>
#include "mqtt_tests.h"
#include "unity/unity.h"

#include <MQTTClientNew.h>
#include <MQTTmbed.h> // Countdown

void MQTT_FULL_NEW()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    SocketAddress sockAddr(mqtt_global::hostname, mqtt_global::port);
    TCPSocket socket;
    socket.open(net);
    socket.connect(sockAddr);

    MQTTClient client(&socket);

    send_messages<MQTTClient>(client, "MQTT_FULL_NEW");

    socket.close();
}

void MQTT_FULL_NEW_TLS()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    TLSSocket *socket = new TLSSocket; // Allocate on heap to avoid stack overflow.
    TEST_ASSERT(NSAPI_ERROR_OK == socket->open(net));
    TEST_ASSERT(NSAPI_ERROR_OK == socket->set_root_ca_cert(mqtt_global::SSL_CA_PEM));
    int ret = socket->connect(mqtt_global::hostname, mqtt_global::port_tls);
    TEST_ASSERT(NSAPI_ERROR_OK == ret);

    MQTTClient client(socket);

    send_messages<MQTTClient>(client, "MQTT_FULL_NEW_TLS");

    socket->close();
    delete socket;
}

void MQTT_FULL_NEW_UDP()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    SocketAddress sockAddr(mqtt_global::hostname, mqtt_global::port_udp);
    UDPSocket socket;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, socket.open(net));
    TEST_ASSERT(NSAPI_ERROR_OK == socket.connect(sockAddr));
    MQTTClient client(&socket);

    send_messages_sn< MQTTClient >(client, "MQTT_FULL_NEW_UDP");

    socket.close();
}
