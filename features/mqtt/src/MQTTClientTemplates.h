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

#ifndef MQTT_CLIENT_NEW_H
#define MQTT_CLIENT_NEW_H

#include <TCPSocket.h>
#include <TLSSocket.h>
#include <DTLSSocket.h>
#include <UDPSocket.h>
#include "mqtt_tests.h"
#include "unity/unity.h"

#include "FP.h"
#include "MQTTPacket.h"
#include <MQTTClient.h>
#include <MQTTmbed.h> // Countdown

template <class SocketType> class MQTTNetworkTemplate {
public:
    MQTTNetworkTemplate(SocketType* _socket) : socket(_socket) {}

    ~MQTTNetworkTemplate() {
        delete socket;
    }

    int read(unsigned char* buffer, int len, int timeout) {
        return socket->recv(buffer, len);
    }

    int write(unsigned char* buffer, int len, int timeout) {
        return socket->send(buffer, len);
    }

    int connect(const char* hostname, int port) {
        SocketAddress sockAddr(hostname, port);
        return socket->connect(sockAddr);
    }

    int disconnect() {
        return socket->close();
    }

private:
    SocketType* socket;
};

template <class SocketType> class MQTTClient {
public:
    typedef void (*messageHandler)(MQTT::MessageData&);

    // Should this assume that the socket is open/connected or do the job itself?
    MQTTClient(SocketType* _socket) {
        socket = _socket;
        mqttNet = new MQTTNetworkTemplate<SocketType>(socket);
        client = new MQTT::Client<MQTTNetworkTemplate<SocketType>, Countdown>(*mqttNet);
    };

    int connect(MQTTPacket_connectData& options) {
        return client->connect(options);
    }

    int publish(const char* topicName, MQTT::Message& message) {
        return client->publish(topicName, message);
    }

    int subscribe(const char* topicFilter, enum MQTT::QoS qos, messageHandler mh) {
        return client->subscribe(topicFilter, qos, mh);
    }

    int unsubscribe(const char* topicFilter) {
        return client->unsubscribe(topicFilter);
    }

    int yield(unsigned long timeout_ms = 1000L) {
        return client->yield(timeout_ms);
    }

    int disconnect() {
        return client->disconnect();
    }
private:

    SocketType* socket;
    MQTTNetworkTemplate<SocketType>* mqttNet;
    MQTT::Client<MQTTNetworkTemplate<SocketType>, Countdown>* client;
};

template <class SocketType> class MQTTSNClient {
public:
    typedef void (*messageHandlerSN)(MQTTSN::MessageData&);

    // Should this assume that the socket is open/connected or do the job itself?
    MQTTSNClient(SocketType* _socket) {
        socket = _socket;
        mqttNet = new MQTTNetworkTemplate<SocketType>(socket);
        client = new MQTTSN::Client<MQTTNetworkTemplate<SocketType>, Countdown>(*mqttNet);
    };

    int connect(MQTTSNPacket_connectData& options) {
        return client->connect(options);
    }

    int publish(MQTTSN_topicid& topicName, MQTTSN::Message& message) {
        return client->publish(topicName, message);
    }

    int subscribe(MQTTSN_topicid& topicFilter, enum MQTTSN::QoS qos, messageHandlerSN mh) {
        return client->subscribe(topicFilter, qos, mh);
    }

    int unsubscribe(MQTTSN_topicid& topicFilter) {
        return client->unsubscribe(topicFilter);
    }

    int yield(unsigned long timeout_ms = 1000L) {
        return client->yield(timeout_ms);
    }

    int disconnect() {
        return client->disconnect();
    }
private:

    SocketType* socket;
    MQTTNetworkTemplate<SocketType>* mqttNet;
    MQTTSN::Client<MQTTNetworkTemplate<SocketType>, Countdown>* client;
};

#endif // MQTT_CLIENT_NEW_H
