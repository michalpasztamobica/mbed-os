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

#include "mqtt_tests.h"
#include "greentea-client/test_env.h"
#include "utest.h"

using namespace utest::v1;

// For TLS hostname must match the "Common Name" set in the server certificate
//const char* hostname = "iot.eclipse.org";
const char* hostname = "192.168.8.76";

const char* SSL_CA_PEM =
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
const char* SSL_CLIENT_CERT_PEM = NULL;

/*
 * (optional) Client private key here in PEM format.
 * Set NULL if you don't use.
 * "-----BEGIN RSA PRIVATE KEY-----\n"
 * ...
 * "-----END RSA PRIVATE KEY-----\n";
 */
const char* SSL_CLIENT_PRIVATE_KEY_PEM = NULL;

Case cases[] = {
    Case("MQTT_CONNECT_UDP", MQTT_CONNECT_UDP),
    Case("MQTT_CONNECT", MQTT_CONNECT),
    Case("MQTT_CONNECT_TLS", MQTT_CONNECT_TLS),
    Case("MQTT_CONNECT_NEW", MQTT_CONNECT_NEW),
    Case("MQTT_CONNECT_NEW_TLS", MQTT_CONNECT_NEW_TLS),
    Case("MQTT_CONNECT_TEMPLATES", MQTT_CONNECT_TEMPLATES),
    Case("MQTT_CONNECT_TEMPLATES_TLS", MQTT_CONNECT_TEMPLATES_TLS),
};

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
#ifdef MBED_GREENTEA_TEST_INTERFACE_TIMEOUT_S
    GREENTEA_SETUP(MBED_GREENTEA_TEST_INTERFACE_TIMEOUT_S, "default_auto");
#else
    GREENTEA_SETUP(480, "default_auto");
#endif
    return verbose_test_setup_handler(number_of_cases);
}

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
