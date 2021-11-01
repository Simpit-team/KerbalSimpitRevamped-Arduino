Quickstart Guide
================

Minimal Arduino sketch
----------------------

.. highlight:: c++

A minimal Kerbal Simpit sketch looks like this::

  #include <KerbalSimpit.h>

  KerbalSimpit mySimpit(Serial);

  void setup() {
    Serial.begin(115200);
    while (!mySimpit.init());
  }

  void loop() {
    mySimpit.update();
  }

There are a few parts that are required:

#. Include the library::

     #include <KerbalSimpit.h>
#. Create a KerbalSimpit object. The constructor requires one argument, the
   serial device that we will use. In most cases this is "Serial"::

     KerbalSimpit mySimpit(Serial);
#. Initialise the serial connection. Kerbal Simpit does not attempt to open
   the serial device, so the sketch should do so in its ``setup()`` function.
   The speed should match that specified by the plugin in its config file::

     Serial.begin(115200);
#. Initialise the KerbalSimpit object. The ``init()`` function performs a
   three-way handshake with the Kerbal Simpit plugin. It returns a boolean
   indicating handshake status, so it's easiest to just call this in a loop
   until a successful handshake is performed::

     while (!mySimpit.init());
#. The ``update()`` function is receives new data from the serial connection.
   It should be called regularly in the sketch ``loop()``.

Message channels
----------------

Every message in to and out of the Kerbal Simpit plugin has a channel ID.
Each channel is dedicated to a specific type of information, and the messages
this library supports are documented in :any:`messagetypes`.

Sending data
------------

The low-level ``send()`` function can send arbitrary data to the plugin::

  mySimpit.send(messageType, message[], messageSize);

* ``messageType`` is a byte representing the channel this message is on. The
  library provides constants for all supported message channels, see
  :any:`messagetypes` for a full list.
* ``message[]`` is a byte array representing the message. The library enforces
  a hard limit on message size, ``MAX_PAYLOAD_SIZE``, which defaults to 32
  bytes.
* ``messageSize`` is a byte representing the size of the message.

The library provides higher-level functions encapsulating most known message
channels. For example, both of these lines activate the standard Brakes
action group::

  mySimpit.send(CAGACTIVATE_MESSAGE, &BRAKES_ACTION, 1);

  mySimpit.activateAction(BRAKES_ACTION);

Refer to :any:`kerbalsimpit` for full documentation.

Receiving data
--------------

To receive data from the plugin, use the ``inboundHandler()`` function to
register a callback handler with the library::

  mySimpit.inboundHandler(myCallbackHandler);

And define the callback handler::

  void myCallbackHandler(byte messageType, byte message[], byte messageSize) {
    switch(messageType) {
    case MESSAGE_TYPE_1:
      // Handle the first type of message.
      break;
    case MESSAGE_TYPE_2:
      // Handle the second type of message.
      break;
    }
  }

Most messages from the plugin consist of several pieces of information. The
library includes structs and helper functions to assist with working with
these. For example, here's a
basic callback handler for dealing with altitude information from the plugin::

  void myCallbackHandler(byte messageType, byte mesesage[], byte messageSize) {
    switch(messageType) {
    case ALTITUDE_MESSAGE:
      if (msgSize == sizeof(altitudeMessage)) {
        altitudeMessage myAltitude;
        myAltitude = parseMessage<altitudeMessage>(msg);
        // further processing of altitude data in myAltitude here
      }
      break;
    }
  }

For a full list of available structs and helper functions, refer to
:any:`payloadstructs`.

Going further
-------------

The examples_ directory of the library contains several example sketches
that demonstrate the different functionality of the library.

.. _examples: https://bitbucket.org/pjhardy/kerbalsimpit-arduino/src/master/examples/
