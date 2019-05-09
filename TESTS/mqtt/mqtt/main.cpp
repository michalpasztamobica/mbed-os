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

#include "mbed.h"
#include "mqtt_tests.h"
#include "greentea-client/test_env.h"
#include "utest.h"

using namespace utest::v1;

const char* mqtt_global::SSL_CA_PEM =
/* Local mosquitto server certificate */
"-----BEGIN CERTIFICATE-----\n"
"MIIDxTCCAq2gAwIBAgIJAIwHDSxAwQemMA0GCSqGSIb3DQEBCwUAMHkxCzAJBgNV\n"
"BAYTAlBMMQ8wDQYDVQQIDAZQb2xhbmQxDzANBgNVBAcMBldhcnNhdzEPMA0GA1UE\n"
"CgwGTW9iaWNhMRUwEwYDVQQDDAwxOTIuMTY4LjguNzYxIDAeBgkqhkiG9w0BCQEW\n"
"EW1vYmljYUBtb2JpY2EuY29tMB4XDTE5MDQzMDA3NTYyNVoXDTI0MDQyOTA3NTYy\n"
"NVoweTELMAkGA1UEBhMCUEwxDzANBgNVBAgMBlBvbGFuZDEPMA0GA1UEBwwGV2Fy\n"
"c2F3MQ8wDQYDVQQKDAZNb2JpY2ExFTATBgNVBAMMDDE5Mi4xNjguOC43NjEgMB4G\n"
"CSqGSIb3DQEJARYRbW9iaWNhQG1vYmljYS5jb20wggEiMA0GCSqGSIb3DQEBAQUA\n"
"A4IBDwAwggEKAoIBAQDmFW8eaA9VM8QlEmhbbGQJVs96JkRDok85fjvRc3DagSxU\n"
"/dU3qlkVBlPl5E8WFJQrKmXmq0OTpNgYPKeJuoCLtHO4FRMtK3eVKMTMuge2tR4y\n"
"sRTFeOzbmllXSlCbpR40afsnStQDASD3Bf4sZMCh3Mubi3tvP92M5+kdfhU+5Wdl\n"
"7t5zsCzRGRCkJhErqeuruHCjTQ/YugKWMff540KN/bTO4HDSBYvR+0l/ru+eLj+/\n"
"4mmoX53ypbnw23WLE4Xt3+A+BHCW2g2CZtHRtP8Kzs8K5a7XS7uSACjc63QBSEeI\n"
"V3xozAso8lvFWt2+csyTH4GWn62pyJXVNZF7/a6bAgMBAAGjUDBOMB0GA1UdDgQW\n"
"BBQdLLdnTh5o78nz/pDPojAB4MhfezAfBgNVHSMEGDAWgBQdLLdnTh5o78nz/pDP\n"
"ojAB4MhfezAMBgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQCOPL/t8OWl\n"
"2BJsKdOj3JDm1tE2XqDt+7M9tmv7l7cH06WiFTt7DnoZumiSnBpY5Nwkv9nJhEIY\n"
"HN2ONDWJ23glsaesf9P1KFrZ6SRwx46J2mEiDLDuYBRuS3OY5hQB6oINXMLPAtE+\n"
"1LX/Bl+qpK851XPmQmEoJMXRHggUrt3ojxF0L7KDiyGFwq1Rdnow9AcsRd5rungq\n"
"DVdNbV9IZvdXUzp/qhXQXbySD7b2B+9fL1k5xz87binVAyZ9aT2YOKbMCTsAr62m\n"
"sHPy5HFdm1loZTEA+vhMSbUtDCue/6Wmp/ZSCU5OBXyHaFhhr8to6Ta8cpZoAOmH\n"
"9maSHwg3X9QL\n"
"-----END CERTIFICATE-----\n";

/*
 * (optional) Client certificate here in PEM format.
 * Set NULL if you don't use.
 * "-----BEGIN CERTIFICATE-----\n"
 * ...
 * "-----END CERTIFICATE-----\n";
 */
const char* mqtt_global::SSL_CLIENT_CERT_PEM = NULL;

/*
 * (optional) Client private key here in PEM format.
 * Set NULL if you don't use.
 * "-----BEGIN RSA PRIVATE KEY-----\n"
 * ...
 * "-----END RSA PRIVATE KEY-----\n";
 */
const char* mqtt_global::SSL_CLIENT_PRIVATE_KEY_PEM = NULL;

#define MQTT_MBED_PUBLIC_TEST_TOPIC_AVAILABLE
#define RUN_INTEGRATION_TESTS
#define MQTT_MBED_PASSWORD_PROTECTION_AVAILABLE

MQTT::Message mqtt_global::default_message;
MQTTSN::Message mqtt_global::default_message_sn;

Case cases[] = {
    Case("MQTT_CONNECT", MQTT_CONNECT),
    Case("MQTT_CONNECT_INVALID", MQTT_CONNECT_INVALID),
    Case("MQTT_CONNECT_NOT_CONNECTED", MQTT_CONNECT_NOT_CONNECTED),
    Case("MQTT_SUBSCRIBE", MQTT_SUBSCRIBE),
    Case("MQTT_SUBSCRIBE_NETWORK_NOT_CONNECTED", MQTT_SUBSCRIBE_NETWORK_NOT_CONNECTED),
    Case("MQTT_SUBSCRIBE_CLIENT_NOT_CONNECTED", MQTT_SUBSCRIBE_CLIENT_NOT_CONNECTED),
    Case("MQTT_SUBSCRIBE_TOPIC_TOO_LONG", MQTT_SUBSCRIBE_TOPIC_TOO_LONG),
    Case("MQTT_SUBSCRIBE_INVALID_MESSAGE_HANDLER", MQTT_SUBSCRIBE_INVALID_MESSAGE_HANDLER),
#ifdef MQTT_MBED_PUBLIC_TEST_TOPIC_AVAILABLE
    Case("MQTT_SUBSCRIBE_RECEIVE", MQTT_SUBSCRIBE_RECEIVE),
#endif
    Case("MQTT_UNSUBSCRIBE_WITHOUT_SUBSCRIBE", MQTT_UNSUBSCRIBE_WITHOUT_SUBSCRIBE),
    Case("MQTT_UNSUBSCRIBE_INVALID", MQTT_UNSUBSCRIBE_INVALID),
    Case("MQTT_PUBLISH", MQTT_PUBLISH),
    Case("MQTT_PUBLISH_NOT_CONNECTED", MQTT_PUBLISH_NOT_CONNECTED),
    Case("MQTT_PUBLISH_TOPIC_TOO_LONG", MQTT_PUBLISH_TOPIC_TOO_LONG),
#ifdef MQTT_MBED_PASSWORD_PROTECTION_AVAILABLE
    Case("MQTT_CONNECT_USER_PASSWORD_INCORRECT", MQTT_CONNECT_USER_PASSWORD_INCORRECT),
#endif
#ifdef RUN_INTEGRATION_TESTS
#ifdef MQTT_MBED_PASSWORD_PROTECTION_AVAILABLE
    Case("MQTT_CONNECT_SUBSCRIBE_PUBLISH_USER_PASSWORD", MQTT_CONNECT_SUBSCRIBE_PUBLISH_USER_PASSWORD),
#endif
    Case("MQTT_CONNECT_SUBSCRIBE_PUBLISH", MQTT_CONNECT_SUBSCRIBE_PUBLISH),
    Case("MQTT_TLS_CONNECT_SUBSCRIBE_PUBLISH", MQTT_TLS_CONNECT_SUBSCRIBE_PUBLISH),

    Case("MQTT_FULL_NEW_UDP", MQTT_FULL_NEW_UDP),
    Case("MQTT_FULL_TEMPLATES_UDP", MQTT_FULL_TEMPLATES_UDP),
    Case("MQTT_FULL_NEW", MQTT_FULL_NEW),
    Case("MQTT_FULL_NEW_TLS", MQTT_FULL_NEW_TLS),
    Case("MQTT_FULL_TEMPLATES", MQTT_FULL_TEMPLATES),
    Case("MQTT_FULL_TEMPLATES_TLS", MQTT_FULL_TEMPLATES_TLS),
#endif

    // MQTT-SN
    Case("MQTTSN_TEST_CONNECT", MQTTSN_TEST_CONNECT),
    Case("MQTTSN_CONNECT_INVALID", MQTTSN_CONNECT_INVALID),
    Case("MQTTSN_CONNECT_NOT_CONNECTED", MQTTSN_CONNECT_NOT_CONNECTED),
    Case("MQTTSN_TEST_SUBSCRIBE", MQTTSN_TEST_SUBSCRIBE),
    Case("MQTTSN_SUBSCRIBE_NETWORK_NOT_CONNECTED", MQTTSN_SUBSCRIBE_NETWORK_NOT_CONNECTED),
    Case("MQTTSN_SUBSCRIBE_CLIENT_NOT_CONNECTED", MQTTSN_SUBSCRIBE_CLIENT_NOT_CONNECTED),
    Case("MQTTSN_SUBSCRIBE_TOPIC_TOO_LONG", MQTTSN_SUBSCRIBE_TOPIC_TOO_LONG),
    Case("MQTTSN_SUBSCRIBE_INVALID_MESSAGE_HANDLER", MQTTSN_SUBSCRIBE_INVALID_MESSAGE_HANDLER),
#ifdef MQTT_MBED_PUBLIC_TEST_TOPIC_AVAILABLE
    Case("MQTTSN_SUBSCRIBE_RECEIVE", MQTTSN_SUBSCRIBE_RECEIVE),
#endif
    Case("MQTTSN_UNSUBSCRIBE_WITHOUT_SUBSCRIBE", MQTTSN_UNSUBSCRIBE_WITHOUT_SUBSCRIBE),
    Case("MQTTSN_UNSUBSCRIBE_INVALID", MQTTSN_UNSUBSCRIBE_INVALID),
    Case("MQTTSN_TEST_PUBLISH", MQTTSN_TEST_PUBLISH),
    Case("MQTTSN_PUBLISH_NOT_CONNECTED", MQTTSN_PUBLISH_NOT_CONNECTED),
    Case("MQTTSN_PUBLISH_TOPIC_TOO_LONG", MQTTSN_PUBLISH_TOPIC_TOO_LONG),
#ifdef RUN_INTEGRATION_TESTS
    Case("MQTTSN_UDP_CONNECT_SUBSCRIBE_PUBLISH", MQTTSN_UDP_CONNECT_SUBSCRIBE_PUBLISH),
#endif
    Case("MQTTSN_IS_CONNECTED", MQTTSN_IS_CONNECTED),
    Case("MQTTSN_IS_CONNECTED_CLIENT_NOT_CONNECTED", MQTTSN_IS_CONNECTED_CLIENT_NOT_CONNECTED),
    Case("MQTTSN_IS_CONNECTED_NETWORK_NOT_CONNECTED", MQTTSN_IS_CONNECTED_NETWORK_NOT_CONNECTED),
};

int arrivedcount = 0;
int arrivedcountSN = 0;

void messageArrived(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;
    printf("Message arrived: qos %d, retained %d, dup %d, packetid %d\r\n", message.qos, message.retained, message.dup, message.id);
    printf("Payload %.*s\r\n", message.payloadlen, (char*)message.payload);
    ++arrivedcount;
    printf("arrived msg: %d\n", arrivedcount);
}

void messageArrivedSN(MQTTSN::MessageData& md)
{
    MQTTSN::Message &message = md.message;
    printf("Message arrived SN: qos %d, retained %d, dup %d, packetid %d\r\n", message.qos, message.retained, message.dup, message.id);
    printf("Payload %.*s\r\n", message.payloadlen, (char*)message.payload);
    ++arrivedcountSN;
    printf("arrived msg: %d\n", arrivedcountSN);
}

void init_topic_sn(MQTTSN_topicid& topic_sn) {
    topic_sn.type = MQTTSN_TOPIC_TYPE_NORMAL;
    topic_sn.data.long_.len = strlen(mqtt_global::topic);
    topic_sn.data.long_.name = const_cast<char*>(mqtt_global::topic);
}

void init_topic_sn_too_long(MQTTSN_topicid& topic_sn) {
    topic_sn.type = MQTTSN_TOPIC_TYPE_NORMAL;
    topic_sn.data.long_.len = strlen(mqtt_global::topic_too_long);
    topic_sn.data.long_.name = const_cast<char*>(mqtt_global::topic_too_long);
}

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
#ifdef MBED_GREENTEA_TEST_INTERFACE_TIMEOUT_S
    GREENTEA_SETUP(MBED_GREENTEA_TEST_INTERFACE_TIMEOUT_S, "default_auto");
#else
    GREENTEA_SETUP(480, "default_auto");
#endif

    NetworkInterface *net = NetworkInterface::get_default_instance();
    nsapi_error_t err = net->connect();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    printf("MBED: TCPClient IP address is '%s'\n", net->get_ip_address());

    // Generate an string of length MAX_MQTT_PACKET_SIZE+1 with alphabet letters.
    char c = 'a';
    for (int i = 0; i < 100+1; i++) {
        mqtt_global::topic_too_long[i] = c;
        if (c < 'z') {
            c++;
        } else {
            c = 'a';
        }
    }

    // Prepare a default message, to avoid code duplication at a later stage.
    sprintf(mqtt_global::message_buffer, "%s", "MQTT_MBED_OS_TEST_MESSAGE");
    mqtt_global::default_message.qos = MQTT::QOS0;
    mqtt_global::default_message.retained = false;
    mqtt_global::default_message.dup = false;
    mqtt_global::default_message.payload = (void*)mqtt_global::message_buffer;
    mqtt_global::default_message.payloadlen = strlen(mqtt_global::message_buffer)+1;

    sprintf(mqtt_global::message_buffer, "%s", "MQTTSN_MBED_OS_TEST_MESSAGE");
    mqtt_global::default_message_sn.qos = MQTTSN::QOS0;
    mqtt_global::default_message_sn.retained = false;
    mqtt_global::default_message_sn.dup = false;
    mqtt_global::default_message_sn.payload = (void*)mqtt_global::message_buffer;
    mqtt_global::default_message_sn.payloadlen = strlen(mqtt_global::message_buffer)+1;

    mqtt_global::mbed_public_test_topic_sn.type = MQTTSN_TOPIC_TYPE_NORMAL;
    mqtt_global::mbed_public_test_topic_sn.data.long_.len = strlen(mqtt_global::mbed_public_test_topic)+1;
    mqtt_global::mbed_public_test_topic_sn.data.long_.name = const_cast<char*>(mqtt_global::mbed_public_test_topic);

    return verbose_test_setup_handler(number_of_cases);
}

void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    NetworkInterface::get_default_instance()->disconnect();
    return greentea_test_teardown_handler(passed, failed, failure);
}

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
