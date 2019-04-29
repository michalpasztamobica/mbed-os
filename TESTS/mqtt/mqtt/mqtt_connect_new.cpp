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

extern int arrivedcount;

extern void messageArrived(MQTT::MessageData& md);

void MQTT_CONNECT_NEW()
{
    arrivedcount = 0;
    float version = 0.7;
    char* topic = "test";

    NetworkInterface *net = NetworkInterface::get_default_instance();
    nsapi_error_t err = net->connect();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    printf("MBED: TCPClient IP address is '%s'\n", net->get_ip_address());

    //    const char* hostname = "iot.eclipse.org";
    const char* hostname = "192.168.8.76";
    int port = 1883;
    SocketAddress sockAddr(hostname, port);
    TCPSocket socket;
    socket.open(net);
    socket.connect(sockAddr);
    printf("Connecting to %s:%d\r\n", hostname, port);

    MQTTClient client(&socket);

    int rc;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "mbed-sample";
    data.username.cstring = "testuser";
    data.password.cstring = "testpassword";
    if ((rc = client.connect(data)) != 0)
        printf("rc from MQTT connect is %d\r\n", rc);

    if ((rc = client.subscribe(topic, MQTT::QOS2, messageArrived)) != 0)
        printf("rc from MQTT subscribe is %d\r\n", rc);

    MQTT::Message message;

    // QoS 0
    char buf[100];
    sprintf(buf, "Hello World!  QoS 0 message from app version %f\r\n", version);
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)buf;
    message.payloadlen = strlen(buf)+1;
    rc = client.publish(topic, message);
    while (arrivedcount < 1)
        client.yield(100);

    // QoS 1
    sprintf(buf, "Hello World!  QoS 1 message from app version %f\r\n", version);
    message.qos = MQTT::QOS1;
    message.payloadlen = strlen(buf)+1;
    rc = client.publish(topic, message);
    while (arrivedcount < 2)
        client.yield(100);

//    // QoS 2
//    sprintf(buf, "Hello World!  QoS 2 message from app version %f\r\n", version);
//    message.qos = MQTT::QOS2;
//    message.payloadlen = strlen(buf)+1;
//    rc = client.publish(topic, message);
//    while (arrivedcount < 3)
//        client.yield(100);

    if ((rc = client.unsubscribe(topic)) != 0)
        printf("rc from unsubscribe was %d\r\n", rc);

    if ((rc = client.disconnect()) != 0)
        printf("rc from disconnect was %d\r\n", rc);

    socket.close();
    net->disconnect();

    printf("Version %.2f: finish %d msgs\r\n", version, arrivedcount);
}
