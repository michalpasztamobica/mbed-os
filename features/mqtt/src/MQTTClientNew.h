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

typedef enum MQTTSocketType_t {
    MQTT_SOCKET_TCP,
    MQTT_SOCKET_TLS,
    MQTT_SOCKET_UDP,
    MQTT_SOCKET_DTLS,
} MQTTSocketType;

class MQTTNetworkTCP {
public:
    MQTTNetworkTCP(TCPSocket* _socket) : socket(_socket) {}

    ~MQTTNetworkTCP() {
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
    TCPSocket* socket;
};

class MQTTNetworkTLS {
public:
    MQTTNetworkTLS(TLSSocket* _socket) : socket(_socket) {}

    ~MQTTNetworkTLS() {
        delete socket;
    }

    int read(unsigned char* buffer, int len, int timeout) {
        return socket->recv(buffer, len);
    }

    int write(unsigned char* buffer, int len, int timeout) {
        return socket->send(buffer, len);
    }

    // TODO add the security connection like in
    // https://github.com/eclipse/paho.mqtt-sn.embedded-c/blob/master/MQTTSNClient/src/MQTTSNClient.h
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

    // Should this also take NetworkInterface or should it just use NetworkInterface::get_default_instance()?
    // What if there are multiple interfaces?
    // Should this assume that the socket is open/connected or do the job itself?
    // Perhaps write macro generators for these functions?
    MQTTClient(TCPSocket* socket) {
        init();
        socketType = MQTT_SOCKET_TCP;
        tcp = socket;
        mqttNetTCP = new MQTTNetworkTCP(tcp);
        clientTCP = new MQTT::Client<MQTTNetworkTCP, Countdown>(*mqttNetTCP);
    };

    MQTTClient(TLSSocket* socket) {
        socketType = MQTT_SOCKET_TLS;
        init();
        tls = socket;
        mqttNetTLS = new MQTTNetworkTLS(tls);
        clientTLS = new MQTT::Client<MQTTNetworkTLS, Countdown>(*mqttNetTLS);
    };

    MQTTClient(UDPSocket* socket) {
        socketType = MQTT_SOCKET_UDP;
        init();
        udp = socket;
        mqttNetUDP = new MQTTNetworkUDP(udp);
        clientUDP = new MQTT::Client<MQTTNetworkUDP, Countdown>(*mqttNetUDP);
    };

     MQTTClient(DTLSSocket* socket) {
        socketType = MQTT_SOCKET_DTLS;
        init();
        dtls = socket;
        mqttNetDTLS = new MQTTNetworkDTLS(dtls);
        clientDTLS = new MQTT::Client<MQTTNetworkDTLS, Countdown>(*mqttNetDTLS);
    };

    int connect(MQTTPacket_connectData& options) {
        switch (socketType) {
        case MQTT_SOCKET_TCP:
            return clientTCP->connect(options);
            break;
        case MQTT_SOCKET_TLS:
            return clientTLS->connect(options);
            break;
        case MQTT_SOCKET_UDP:
            return clientTCP->connect(options);
            break;
        case MQTT_SOCKET_DTLS:
            return clientTCP->connect(options);
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
        case MQTT_SOCKET_UDP:
            return clientTCP->publish(topicName, message);
            break;
        case MQTT_SOCKET_DTLS:
            return clientTCP->publish(topicName, message);
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
        case MQTT_SOCKET_UDP:
            return clientTCP->subscribe(topicFilter, qos, mh);
            break;
        case MQTT_SOCKET_DTLS:
            return clientTCP->subscribe(topicFilter, qos, mh);
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
        case MQTT_SOCKET_UDP:
            return clientTCP->unsubscribe(topicFilter);
            break;
        case MQTT_SOCKET_DTLS:
            return clientTCP->unsubscribe(topicFilter);
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
            return clientTCP->yield(timeout_ms);
            break;
        case MQTT_SOCKET_DTLS:
            return clientTCP->yield(timeout_ms);
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
            return clientTCP->disconnect();
            break;
        case MQTT_SOCKET_DTLS:
            return clientTCP->disconnect();
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
    MQTTNetworkTLS* mqttNetTLS;
    MQTTNetworkUDP* mqttNetUDP;
    MQTTNetworkDTLS* mqttNetDTLS;
    NetworkInterface *net;

    MQTT::Client<MQTTNetworkTCP, Countdown>* clientTCP;
    MQTT::Client<MQTTNetworkTLS, Countdown>* clientTLS;
    MQTT::Client<MQTTNetworkUDP, Countdown>* clientUDP;
    MQTT::Client<MQTTNetworkDTLS, Countdown>* clientDTLS;
};

#endif // MQTT_CLIENT_NEW_H
