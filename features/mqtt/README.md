### MQTT protocol

MQTT is a machine-to-machine (M2M)/"Internet of Things" connectivity protocol. It was designed as an extremely lightweight publish/subscribe messaging transport. For more details see [http://mqtt.org/](http://mqtt.org/).

By design MQTT operates over TCP protocol, so an MQTTSN (For MQTT for Sensor Networks) was designed, which uses less resources and can operate over connectionless procols (such as UDP), but it requires a special gateway.

Mbed-os only allows you to create an MQTT client device, an external broker is needed for MQTT to operate. Clients can subscribe or publish to a topic, after connecting to a broker.

### API and usage

Mbed-os uses [Eclipse paho project emmbedded c implementation of MQTT protocol](https://github.com/eclipse/paho.mqtt.embedded-c), providing its own `Network` template arguments (see below).

To communicate over MQTT an instance of a template class `MQTT::Client<Network, Timer>` has to be created. mbed-os provides two Network specializations:

* `MQTTNetwork` - to communicate over mbed-os's `TCPSocket`

* `MQTTNetworkTLS` - to communicate over mbed-os's `TLSSocket`

Paho's default `Timer` implementation, called `Countdown` and available from `MQTTmbed.h` can be used.

The `MQTT::Client` constructor takes one argument which is a pointer to a successfully connected NetworkInterface.

Below is an example of how to create an instance of an MQTTClient:

```cpp
#include <MQTTNetwork.h>
#include <MQTTClient.h>
#include <MQTTmbed.h> // Countdown

NetworkInterface *net = NetworkInterface::get_default_instance();
net->connect();
MQTTNetwork mqttNet(net);
MQTT::Client<MQTTNetwork, Countdown> client(mqttNet);

```

The MQTTNetwork has to `connect` to a broker, given its address and port number.

Once connected the MQTT client is able to:

* `connect` to the broker, based on a filled `MQTTPacket_connectData` structure,
* `diconnect` from the broker,
* `subscribe` to a topic and register a callback function to be called whenever a new message arrives,
* `unsubscribe` from a topic,
* `publish` messages defined with `MQTT::Message` structure to a topic,
* if no operation is required, but an MQTT connection should be kept open, the client can `yield`,
* `MQTTNetworkTLS` requires a certificate to be set using `set_root_cert()` before calling connect.