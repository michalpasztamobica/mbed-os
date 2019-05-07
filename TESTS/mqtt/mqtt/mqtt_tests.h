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

#ifndef MQTT_TESTS_H
#define MQTT_TESTS_H

#include <MQTTClient.h>

#include <MQTTSNClient.h>

extern const char* hostname;
extern const char* SSL_CA_PEM;
extern const char* SSL_CLIENT_CERT_PEM;
extern const char* SSL_CLIENT_PRIVATE_KEY_PEM;

/*
 * Test cases
 */
void MQTT_CONNECT();
void MQTT_CONNECT_TLS();
void MQTT_CONNECT_NEW();
void MQTT_CONNECT_NEW_TLS();
void MQTT_CONNECT_TEMPLATES();
void MQTT_CONNECT_TEMPLATES_TLS();
void MQTT_CONNECT_UDP();
void MQTT_CONNECT_NEW_UDP();
void MQTT_CONNECT_TEMPLATES_UDP();

extern int arrivedcount;
extern int arrivedcountSN;
void messageArrived(MQTT::MessageData& md);
void messageArrivedSN(MQTTSN::MessageData& md);

template <class Client> int send_messages(Client &client, char *clientID) {
    arrivedcount = 0;
    char* topic = "test";
    int rc;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = (char*)clientID;
    data.username.cstring = "testuser";
    data.password.cstring = "testpassword";
    if ((rc = client.connect(data)) != 0)
        printf("rc from MQTT connect is %d\r\n", rc);

    if ((rc = client.subscribe(topic, MQTT::QOS2, messageArrived)) != 0)
        printf("rc from MQTT subscribe is %d\r\n", rc);

    MQTT::Message message;

    // QoS 0
    char buf[100];
    sprintf(buf, "QoS 0 %s\n", clientID);
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)buf;
    message.payloadlen = strlen(buf)+1;
    rc = client.publish(topic, message);
    printf("arrived: %d\n", arrivedcount);
    while (arrivedcount < 1) {
        client.yield(100);
        printf("arrived: %d\n", arrivedcount);
    }

    printf("Moving to 1\n");
    // QoS 1
    sprintf(buf, "QoS 1 %s\n", clientID);
    message.qos = MQTT::QOS1;
    message.payloadlen = strlen(buf)+1;
    rc = client.publish(topic, message);
    while (arrivedcount < 2)
        client.yield(100);

//    // QoS 2
//    sprintf(buf, "QoS 2 %s\n", clientID);
//    message.qos = MQTT::QOS2;
//    message.payloadlen = strlen(buf)+1;
//    rc = client.publish(topic, message);
//    while (arrivedcount < 3)
//        client.yield(100);

    if ((rc = client.unsubscribe(topic)) != 0)
        printf("rc from unsubscribe was %d\r\n", rc);

    if ((rc = client.disconnect()) != 0)
        printf("rc from disconnect was %d\r\n", rc);

    return rc;
}

template <class Client> int send_messages_sn(Client &client, char *clientID) {
    arrivedcountSN = 0;
    char topicName[5] = "test";
    MQTTSN_topicid topic;
    topic.type = MQTTSN_TOPIC_TYPE_NORMAL;
    topic.data.long_.len = strlen(topicName);
    topic.data.long_.name = const_cast<char*>(topicName);
    int rc;
    MQTTSNPacket_connectData data = MQTTSNPacket_connectData_initializer;
    data.clientID.cstring = clientID;
    if ((rc = client.connect(data)) != 0)
        printf("rc from MQTT connect is %d\r\n", rc);

    if ((rc = client.subscribe(topic, MQTTSN::QOS2, messageArrivedSN)) != 0)
        printf("rc from MQTT subscribe is %d\r\n", rc);

    printf("Topic id: %d\n", topic.data.id);
    MQTTSN::Message message;

    // QoS 0
    char buf[100];
    sprintf(buf, "QoS 0 %s\n", clientID);
    message.qos = MQTTSN::QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)buf;
    message.payloadlen = strlen(buf)+1;
    rc = client.publish(topic, message);

//    TODO: get the gateway/client configuration right to have the subscribe working.
//    while (arrivedcountSN < 1)
//        client.yield(100);

    // QoS 1
    sprintf(buf, "QoS 1 %s\n", clientID);
    message.qos = MQTTSN::QOS1;
    message.payloadlen = strlen(buf)+1;
    rc = client.publish(topic, message);
//    while (arrivedcountSN < 2)
//        client.yield(100);

//    // QoS 2
//    sprintf(buf, "QoS 2 %s\n", clientID);
//    message.qos = MQTTSN::QOS2;
//    message.payloadlen = strlen(buf)+1;
//    rc = client.publish(topic, message);
//    while (arrivedcountSN < 3)
//        client.yield(100);

    if ((rc = client.unsubscribe(topic)) != 0)
        printf("rc from unsubscribe was %d\r\n", rc);

    if ((rc = client.disconnect()) != 0)
        printf("rc from disconnect was %d\r\n", rc);

    return rc;
}

#endif //MQTT_TESTS_H
