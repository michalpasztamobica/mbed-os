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

#include "MQTTClientMbedOs.h"

int MQTTNetworkNew::read(unsigned char* buffer, int len, int timeout) {
    return socket->recv(buffer, len);
}

int MQTTNetworkNew::write(unsigned char* buffer, int len, int timeout) {
    return socket->send(buffer, len);
}

int MQTTNetworkNew::connect(const char* hostname, int port) {
    SocketAddress sockAddr(hostname, port);
    return socket->connect(sockAddr);
}

int MQTTNetworkNew::disconnect() {
    return socket->close();
}

MQTTClient::MQTTClient(TCPSocket* _socket) {
    init(_socket);
    mqttNet = new MQTTNetworkNew(socket);
    client = new MQTT::Client<MQTTNetworkNew, Countdown>(*mqttNet);
};

MQTTClient::MQTTClient(TLSSocket* _socket) {
    init(_socket);
    mqttNet = new MQTTNetworkNew(socket);
    client = new MQTT::Client<MQTTNetworkNew, Countdown>(*mqttNet);
};

MQTTClient::MQTTClient(UDPSocket* _socket) {
    init(_socket);
    mqttNet = new MQTTNetworkNew(socket);
    clientSN = new MQTTSN::Client<MQTTNetworkNew, Countdown>(*mqttNet);
};

 MQTTClient::MQTTClient(DTLSSocket* _socket) {
    init(_socket);
    mqttNet = new MQTTNetworkNew(socket);
    clientSN = new MQTTSN::Client<MQTTNetworkNew, Countdown>(*mqttNet);
};

int MQTTClient::connect(MQTTPacket_connectData& options) {
    if (client != NULL) {
        return client->connect(options);
    } else {
        return NSAPI_ERROR_NO_CONNECTION;
    }
}

int MQTTClient::connect(MQTTSNPacket_connectData& options) {
    if (clientSN != NULL) {
        return clientSN->connect(options);
    } else {
        return NSAPI_ERROR_NO_CONNECTION;
    }
}

int MQTTClient::publish(const char* topicName, MQTT::Message& message) {
    if (client != NULL) {
        return client->publish(topicName, message);
    } else {
        return NSAPI_ERROR_NO_CONNECTION;
    }
}

int MQTTClient::publish(MQTTSN_topicid& topicName, MQTTSN::Message& message) {
    if (clientSN != NULL) {
        return clientSN->publish(topicName, message);
    } else {
        return NSAPI_ERROR_NO_CONNECTION;
    }
}

int MQTTClient::subscribe(const char* topicFilter, enum MQTT::QoS qos, messageHandler mh) {
    if (client != NULL) {
        return client->subscribe(topicFilter, qos, mh);
    } else {
        return NSAPI_ERROR_NO_CONNECTION;
    }
}

int MQTTClient::subscribe(MQTTSN_topicid& topicFilter, enum MQTTSN::QoS qos, messageHandlerSN mh) {
    if (clientSN != NULL) {
        return clientSN->subscribe(topicFilter, qos, mh);
    } else {
        return NSAPI_ERROR_NO_CONNECTION;
    }
}

int MQTTClient::unsubscribe(const char* topicFilter) {
    if (client != NULL) {
        return client->unsubscribe(topicFilter);
    } else {
        return NSAPI_ERROR_NO_CONNECTION;
    }
}

int MQTTClient::unsubscribe(MQTTSN_topicid& topicFilter) {
    if (clientSN != NULL) {
        return clientSN->unsubscribe(topicFilter);
    } else {
        return NSAPI_ERROR_NO_CONNECTION;
    }
}

int MQTTClient::yield(unsigned long timeout_ms) {
    if (client != NULL) {
        return client->yield(timeout_ms);
    } else if (clientSN != NULL) {
        return clientSN->yield(timeout_ms);
    } else {
        return NSAPI_ERROR_NO_CONNECTION;
    }
}

int MQTTClient::disconnect() {
    if (client != NULL) {
        return client->disconnect();
    } else if (clientSN != NULL) {
        return clientSN->disconnect(0);
    } else {
        return NSAPI_ERROR_NO_CONNECTION;
    }
}

void MQTTClient::init(Socket* sock) {
    socket = sock;
    client = NULL;
    clientSN = NULL;
}


