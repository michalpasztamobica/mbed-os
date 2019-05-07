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

typedef enum MQTTSocketType_t {
    MQTT_SOCKET_TCP,
    MQTT_SOCKET_TLS,
    MQTT_SOCKET_UDP,
    MQTT_SOCKET_DTLS,
} MQTTSocketType;

class MQTTNetworkTCP {
public:
    MQTTNetworkTCP(TCPSocket* _socket) : socket(_socket) {}

    int read(unsigned char* buffer, int len, int timeout) {
        return socket->recv(buffer, len);
    }

    int write(unsigned char* buffer, int len, int timeout) {
        printf("write3\n");
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
    TCPSocket* socket;
};

class MQTTNetworkTLSNew { // Avoid name clash with MQTTNetworkTLS
public:
    MQTTNetworkTLSNew(TLSSocket* _socket) : socket(_socket) {}

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
    TLSSocket* socket;
};

class MQTTNetworkUDP {
public:
    MQTTNetworkUDP(UDPSocket* _socket) : socket(_socket) {}

    ~MQTTNetworkUDP() {
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
    UDPSocket* socket;
};

class MQTTNetworkDTLS {
public:
    MQTTNetworkDTLS(DTLSSocket* _socket) : socket(_socket) {}

    ~MQTTNetworkDTLS() {
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
    DTLSSocket* socket;
};

class MQTTClient {
public:
    typedef void (*messageHandler)(MQTT::MessageData&);
    typedef void (*messageHandlerSN)(MQTTSN::MessageData&);

    // Perhaps write macro generators for these functions?
    MQTTClient(TCPSocket* socket) {
        init();
        socketType = MQTT_SOCKET_TCP;
        tcp = socket;
        mqttNetTCP = new MQTTNetworkTCP(tcp);
        clientTCP = new MQTT::Client<MQTTNetworkTCP, Countdown>(*mqttNetTCP);
    };

    MQTTClient(TLSSocket* socket) {
        init();
        socketType = MQTT_SOCKET_TLS;
        tls = socket;
        mqttNetTLS = new MQTTNetworkTLSNew(tls);
        clientTLS = new MQTT::Client<MQTTNetworkTLSNew, Countdown>(*mqttNetTLS);
    };

    MQTTClient(UDPSocket* socket) {
        init();
        socketType = MQTT_SOCKET_UDP;
        udp = socket;
        mqttNetUDP = new MQTTNetworkUDP(udp);
        clientUDP = new MQTTSN::Client<MQTTNetworkUDP, Countdown>(*mqttNetUDP);
    };

     MQTTClient(DTLSSocket* socket) {
        init();
        socketType = MQTT_SOCKET_DTLS;
        dtls = socket;
        mqttNetDTLS = new MQTTNetworkDTLS(dtls);
        clientDTLS = new MQTTSN::Client<MQTTNetworkDTLS, Countdown>(*mqttNetDTLS);
    };

    int connect(MQTTPacket_connectData& options) {
        switch (socketType) {
        case MQTT_SOCKET_TCP:
            return clientTCP->connect(options);
            break;
        case MQTT_SOCKET_TLS:
            return clientTLS->connect(options);
            break;
        default:
            return -1;
        }
    }

    int connect(MQTTSNPacket_connectData& options) {
        switch (socketType) {
        case MQTT_SOCKET_UDP:
            return clientUDP->connect(options);
            break;
        case MQTT_SOCKET_DTLS:
            return clientDTLS->connect(options);
            break;
        default:
            return -1;
        }
    }

    int publish(const char* topicName, MQTT::Message& message) {
        switch (socketType) {
        case MQTT_SOCKET_TCP:
            return clientTCP->publish(topicName, message);
            break;
        case MQTT_SOCKET_TLS:
            return clientTLS->publish(topicName, message);
            break;
        default:
            return -1;
        }
    }

    int publish(MQTTSN_topicid& topicName, MQTTSN::Message& message) {
        switch (socketType) {
        case MQTT_SOCKET_UDP:
            return clientUDP->publish(topicName, message);
            break;
        case MQTT_SOCKET_DTLS:
            return clientDTLS->publish(topicName, message);
            break;
        default:
            return -1;
        }
    }

    int subscribe(const char* topicFilter, enum MQTT::QoS qos, messageHandler mh) {
        switch (socketType) {
        case MQTT_SOCKET_TCP:
            return clientTCP->subscribe(topicFilter, qos, mh);
            break;
        case MQTT_SOCKET_TLS:
            return clientTLS->subscribe(topicFilter, qos, mh);
            break;
        default:
            return -1;
        }
    }

    int subscribe(MQTTSN_topicid& topicFilter, enum MQTTSN::QoS qos, messageHandlerSN mh) {
        switch (socketType) {
        case MQTT_SOCKET_UDP:
            return clientUDP->subscribe(topicFilter, qos, mh);
            break;
        case MQTT_SOCKET_DTLS:
            return clientDTLS->subscribe(topicFilter, qos, mh);
            break;
        default:
            return -1;
        }
    }

     int unsubscribe(const char* topicFilter) {
        switch (socketType) {
        case MQTT_SOCKET_TCP:
            return clientTCP->unsubscribe(topicFilter);
            break;
        case MQTT_SOCKET_TLS:
            return clientTLS->unsubscribe(topicFilter);
            break;
        default:
            return -1;
        }
    }

    int unsubscribe(MQTTSN_topicid& topicFilter) {
        switch (socketType) {
        case MQTT_SOCKET_UDP:
            return clientUDP->unsubscribe(topicFilter);
            break;
        case MQTT_SOCKET_DTLS:
            return clientDTLS->unsubscribe(topicFilter);
            break;
        default:
            return -1;
        }
    }

    int yield(unsigned long timeout_ms = 1000L) {
        switch (socketType) {
        case MQTT_SOCKET_TCP:
            return clientTCP->yield(timeout_ms);
            break;
        case MQTT_SOCKET_TLS:
            return clientTLS->yield(timeout_ms);
            break;
        case MQTT_SOCKET_UDP:
            return clientUDP->yield(timeout_ms);
            break;
        case MQTT_SOCKET_DTLS:
            return clientDTLS->yield(timeout_ms);
            break;
        default:
            return -1;
        }
    }

    int disconnect() {
        switch (socketType) {
        case MQTT_SOCKET_TCP:
            return clientTCP->disconnect();
            break;
        case MQTT_SOCKET_TLS:
            return clientTLS->disconnect();
            break;
        case MQTT_SOCKET_UDP:
            return clientUDP->disconnect();
            break;
        case MQTT_SOCKET_DTLS:
            return clientDTLS->disconnect();
            break;
        default:
            return -1;
        }
    }
private:

    void init(void) {
        tcp = NULL;
        tls = NULL;
        udp = NULL;
        dtls = NULL;

        mqttNetTCP = NULL;
        mqttNetTLS = NULL;
        mqttNetUDP = NULL;
        mqttNetDTLS = NULL;

        clientTCP = NULL;
        clientTLS = NULL;
        clientUDP = NULL;
        clientDTLS = NULL;
    }

    MQTTSocketType socketType;

    TCPSocket* tcp;
    TLSSocket* tls;
    UDPSocket* udp;
    DTLSSocket* dtls;
    MQTTNetworkTCP* mqttNetTCP;
    MQTTNetworkTLSNew* mqttNetTLS;
    MQTTNetworkUDP* mqttNetUDP;
    MQTTNetworkDTLS* mqttNetDTLS;
    NetworkInterface *net;

    MQTT::Client<MQTTNetworkTCP, Countdown>* clientTCP;
    MQTT::Client<MQTTNetworkTLSNew, Countdown>* clientTLS;
    MQTTSN::Client<MQTTNetworkUDP, Countdown>* clientUDP;
    MQTTSN::Client<MQTTNetworkDTLS, Countdown>* clientDTLS;
};

#endif // MQTT_CLIENT_NEW_H
