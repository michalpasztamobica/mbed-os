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
#include <MQTTPacket.h>
#include <MQTTClient.h>
#include <MQTTSNPacket.h>
#include <MQTTSNClient.h>
#include <MQTTmbed.h> // Countdown

class MQTTNetworkNew {
public:
    MQTTNetworkNew(Socket* _socket) : socket(_socket) {}

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
    Socket* socket;
};

class MQTTClient {
public:
    typedef void (*messageHandler)(MQTT::MessageData&);
    typedef void (*messageHandlerSN)(MQTTSN::MessageData&);

    // Perhaps write macro generators for these functions?
    MQTTClient(TCPSocket* _socket) {
        init(_socket);
        mqttNet = new MQTTNetworkNew(socket);
        client = new MQTT::Client<MQTTNetworkNew, Countdown>(*mqttNet);
    };

    MQTTClient(TLSSocket* _socket) {
        init(_socket);
        mqttNet = new MQTTNetworkNew(socket);
        client = new MQTT::Client<MQTTNetworkNew, Countdown>(*mqttNet);
    };

    MQTTClient(UDPSocket* _socket) {
        init(_socket);
        mqttNet = new MQTTNetworkNew(socket);
        clientSN = new MQTTSN::Client<MQTTNetworkNew, Countdown>(*mqttNet);
    };

     MQTTClient(DTLSSocket* _socket) {
        init(_socket);
        mqttNet = new MQTTNetworkNew(socket);
        clientSN = new MQTTSN::Client<MQTTNetworkNew, Countdown>(*mqttNet);
    };

    int connect(MQTTPacket_connectData& options) {
        if (client != NULL) {
            return client->connect(options);
        } else {
            return NSAPI_ERROR_NO_CONNECTION;
        }
    }

    int connect(MQTTSNPacket_connectData& options) {
        if (clientSN != NULL) {
            return clientSN->connect(options);
        } else {
            return NSAPI_ERROR_NO_CONNECTION;
        }
    }

    int publish(const char* topicName, MQTT::Message& message) {
        if (client != NULL) {
            return client->publish(topicName, message);
        } else {
            return NSAPI_ERROR_NO_CONNECTION;
        }
    }

    int publish(MQTTSN_topicid& topicName, MQTTSN::Message& message) {
        if (clientSN != NULL) {
            return clientSN->publish(topicName, message);
        } else {
            return NSAPI_ERROR_NO_CONNECTION;
        }
    }

    int subscribe(const char* topicFilter, enum MQTT::QoS qos, messageHandler mh) {
        if (client != NULL) {
            return client->subscribe(topicFilter, qos, mh);
        } else {
            return NSAPI_ERROR_NO_CONNECTION;
        }
    }

    int subscribe(MQTTSN_topicid& topicFilter, enum MQTTSN::QoS qos, messageHandlerSN mh) {
        if (clientSN != NULL) {
            return clientSN->subscribe(topicFilter, qos, mh);
        } else {
            return NSAPI_ERROR_NO_CONNECTION;
        }
    }

    int unsubscribe(const char* topicFilter) {
        if (client != NULL) {
            return client->unsubscribe(topicFilter);
        } else {
            return NSAPI_ERROR_NO_CONNECTION;
        }
    }

    int unsubscribe(MQTTSN_topicid& topicFilter) {
        if (clientSN != NULL) {
            return clientSN->unsubscribe(topicFilter);
        } else {
            return NSAPI_ERROR_NO_CONNECTION;
        }
    }

    int yield(unsigned long timeout_ms = 1000L) {
        if (client != NULL) {
            return client->yield(timeout_ms);
        } else if (clientSN != NULL) {
            return clientSN->yield(timeout_ms);
        } else {
            return NSAPI_ERROR_NO_CONNECTION;
        }
    }

    int disconnect() {
        if (client != NULL) {
            return client->disconnect();
        } else if (clientSN != NULL) {
            return clientSN->disconnect(0);
        } else {
            return NSAPI_ERROR_NO_CONNECTION;
        }
    }

    void init(Socket* sock) {
        socket = sock;
        client = NULL;
        clientSN = NULL;
    }

private:
    Socket* socket;
    MQTTNetworkNew* mqttNet;
    NetworkInterface *net;

    MQTT::Client<MQTTNetworkNew, Countdown>* client;
    MQTTSN::Client<MQTTNetworkNew, Countdown>* clientSN;
};

#endif // MQTT_CLIENT_NEW_H
