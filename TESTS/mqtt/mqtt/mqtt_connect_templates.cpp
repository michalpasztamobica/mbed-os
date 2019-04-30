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

#include <MQTTClientTemplates.h>
#include <MQTTmbed.h> // Countdown

extern int arrivedcount;

void MQTT_CONNECT_TEMPLATES()
{
    arrivedcount = 0;

    NetworkInterface *net = NetworkInterface::get_default_instance();
    nsapi_error_t err = net->connect();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    printf("MBED: TCPClient IP address is '%s'\n", net->get_ip_address());

    int port = 1883;
    SocketAddress sockAddr(hostname, port);
    TCPSocket socket;
    socket.open(net);
    socket.connect(sockAddr);
    printf("Connecting to %s:%d\r\n", hostname, port);

    MQTTClient<TCPSocket> client(&socket);

    send_messages< MQTTClient<TCPSocket> >(client, "MQTT_CONNECT_TEMPLATES");

    socket.close();
    net->disconnect();
}

void MQTT_CONNECT_TEMPLATES_TLS()
{
    arrivedcount = 0;

    NetworkInterface *net = NetworkInterface::get_default_instance();
    nsapi_error_t err = net->connect();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    printf("MBED: TCPClient IP address is '%s'\n", net->get_ip_address());

    int port = 8883;
    SocketAddress sockAddr(hostname, port);
    TLSSocket *socket = new TLSSocket; // Allocate on heap to avoid stack overflow.
    socket->open(net);
    socket->set_root_ca_cert(SSL_CA_PEM);
    TEST_ASSERT(NSAPI_ERROR_OK == socket->connect(sockAddr));
    printf("Connecting to %s:%d\r\n", hostname, port);

    MQTTClient<TLSSocket> client(socket);

    send_messages< MQTTClient<TLSSocket> >(client, "MQTT_CONNECT_TEMPLATES_TLS");

    socket->close();
    delete socket;
    net->disconnect();
}
