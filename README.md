# rmf69w-mqtt-sn
Implementation of MQTT-SN over an RFM69W network.

# Introduction
This project aims to provide a minimal implementation of MQTT-SN for a very simple sensor network.
The idea is to have battery powered sensor nodes publishing their data to a gateway, which takes care of distributing the messages further.
Since the sensor nodes should be as simple as possible, I anticipate to implement QOS -1 only.

# Implementation

The anticipated implementation is using AVR MCUs and RFM69W modules for the sensor nodes.
The gateway will be implemented using a Raspberry Pi B with a simple RFM69W shield.

# References
  * [MQTT-SN V1.2 specification](http://mqtt.org/new/wp-content/uploads/2009/06/MQTT-SN_spec_v1.2.pdf)
