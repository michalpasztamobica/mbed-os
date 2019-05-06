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
#include <MQTTNetworkTLS.h>
#include <MQTTSNNetworkUDP.h>
#include "mqtt_tests.h"
#include "unity/unity.h"

#include <MQTTClient.h>
#include <MQTTSNClient.h>
#include <MQTTmbed.h> // Countdown

int arrivedcount = 0;

void messageArrived(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;
    printf("Message arrived: qos %d, retained %d, dup %d, packetid %d\r\n", message.qos, message.retained, message.dup, message.id);
    printf("Payload %.*s\r\n", message.payloadlen, (char*)message.payload);
    ++arrivedcount;
}

void messageArrivedSN(MQTTSN::MessageData& md)
{
    MQTTSN::Message &message = md.message;
    printf("Message arrived: qos %d, retained %d, dup %d, packetid %d\r\n", message.qos, message.retained, message.dup, message.id);
    printf("Payload %.*s\r\n", message.payloadlen, (char*)message.payload);
    ++arrivedcount;
}

void MQTT_CONNECT()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    nsapi_error_t err = net->connect();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    printf("MBED: TCPClient IP address is '%s'\n", net->get_ip_address());

    MQTTNetwork mqttNet(net);

    MQTT::Client<MQTTNetwork, Countdown> client(mqttNet);

    int port = 1883;
    printf("Connecting to %s:%d\r\n", hostname, port);
    int rc = mqttNet.connect(hostname, port);
    if (rc != 0)
        printf("rc from TCP connect is %d\r\n", rc);

    send_messages< MQTT::Client<MQTTNetwork, Countdown> >(client, "MQTT_CONNECT");

    mqttNet.disconnect();

    net->disconnect();
}

void MQTT_CONNECT_TLS()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    nsapi_error_t err = net->connect();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    printf("MBED: TCPClient IP address is '%s'\n", net->get_ip_address());

    MQTTNetworkTLS mqttNet(net);

    MQTT::Client<MQTTNetworkTLS, Countdown> client(mqttNet);

    int port = 8883;
    printf("Connecting to %s:%d\r\n", hostname, port);
    int rc = mqttNet.connect(hostname, port, SSL_CA_PEM, SSL_CLIENT_CERT_PEM, SSL_CLIENT_PRIVATE_KEY_PEM);
    if (rc != 0)
        printf("rc from TCP connect is %d\r\n", rc);

    send_messages< MQTT::Client<MQTTNetworkTLS, Countdown> >(client, "MQTT_CONNECT_TLS");

    mqttNet.disconnect();

    net->disconnect();
}

void MQTT_CONNECT_UDP()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    nsapi_error_t err = net->connect();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    printf("MBED: TCPClient IP address is '%s'\n", net->get_ip_address());

    UDPSocket sock;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(NetworkInterface::get_default_instance()));

    MQTTSNNetworkUDP mqttNet(sock);

    MQTTSN::Client<MQTTSNNetworkUDP, Countdown> client(mqttNet);

    int port = 10000;
    printf("Connecting to %s:%d\r\n", hostname, port);
    int rc = mqttNet.connect(hostname, port);
    if (rc != 0)
        printf("rc from TCP connect is %d\r\n", rc);

    send_messages_sn< MQTTSN::Client<MQTTSNNetworkUDP, Countdown> >(client, "MQTT_CONNECT_UDP");

    mqttNet.disconnect();

    net->disconnect();
}
