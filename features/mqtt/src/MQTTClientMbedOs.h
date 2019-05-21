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

#ifndef MQTT_CLIENT_MBED_OS_H
#define MQTT_CLIENT_MBED_OS_H

#include <TCPSocket.h>
#include <TLSSocket.h>
#include <DTLSSocket.h>
#include <UDPSocket.h>
#include "unity/unity.h"

#include "FP.h"
#include <MQTTPacket.h>
#include <MQTTClient.h>
#include <MQTTSNPacket.h>
#include <MQTTSNClient.h>
#include <MQTTmbed.h> // Countdown

class MQTTNetworkNew {
public:
    MQTTNetworkNew(Socket* _socket) : socket(_socket) {}

    int read(unsigned char* buffer, int len, int timeout);
    int write(unsigned char* buffer, int len, int timeout);
    int connect(const char* hostname, int port);
    int disconnect();

private:
    Socket* socket;
};

class MQTTClient {
public:
    typedef void (*messageHandler)(MQTT::MessageData&);
    typedef void (*messageHandlerSN)(MQTTSN::MessageData&);

    MQTTClient(TCPSocket* _socket);
    MQTTClient(TLSSocket* _socket);
    MQTTClient(UDPSocket* _socket);
    MQTTClient(DTLSSocket* _socket);

    int connect(MQTTPacket_connectData& options);
    int connect(MQTTSNPacket_connectData& options);

    int publish(const char* topicName, MQTT::Message& message);
    int publish(MQTTSN_topicid& topicName, MQTTSN::Message& message);

    int subscribe(const char* topicFilter, enum MQTT::QoS qos, messageHandler mh);
    int subscribe(MQTTSN_topicid& topicFilter, enum MQTTSN::QoS qos, messageHandlerSN mh);

    int unsubscribe(const char* topicFilter);
    int unsubscribe(MQTTSN_topicid& topicFilter);

    int yield(unsigned long timeout_ms = 1000L) ;

    int disconnect();

    void init(Socket* sock);

private:
    Socket* socket;
    MQTTNetworkNew* mqttNet;
    NetworkInterface *net;

    MQTT::Client<MQTTNetworkNew, Countdown>* client;
    MQTTSN::Client<MQTTNetworkNew, Countdown>* clientSN;
};

#endif // MQTT_CLIENT_MBED_OS_H
