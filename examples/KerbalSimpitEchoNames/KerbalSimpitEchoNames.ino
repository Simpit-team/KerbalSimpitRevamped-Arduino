/* KerbalSimpitEchoNames
   A demonstration of reading the SOI name and the vessel name
   and echo it back to KSP for debug purpose.
   It could also be displayed to a screen if one is available
   (not included in this example)

*/

#include "KerbalSimpit.h"


// Declare a KerbalSimpit object that will communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);

void setup() {
  // Open the serial connection.
  Serial.begin(115200);

  // Set up the build in LED, and turn it on.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // This loop continually attempts to handshake with the plugin.
  // It will keep retrying until it gets a successful handshake.
  while (!mySimpit.init()) {
    delay(100);
  }
  // Turn off the built-in LED to indicate handshaking is complete.
  digitalWrite(LED_BUILTIN, LOW);
  // Display a message in KSP to indicate handshaking is complete.
  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);
  // Sets our callback function. The KerbalSimpit library will
  // call this function every time a packet is received.
  mySimpit.inboundHandler(messageHandler);
  // Send a message to the plugin registering for the channels.
  mySimpit.registerChannel(SOI_MESSAGE);
  mySimpit.registerChannel(VESSEL_NAME_MESSAGE);
}

void loop() {
  // Check for new serial messages.
  mySimpit.update();
}

void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch(messageType) {
  case SOI_MESSAGE: {
      // Parse the SOI name
      String SOIName = (char *) msg;
      SOIName[msgSize] = '\0';

      // Echo it to KSP
      mySimpit.printToKSP("I'm orbitting " + SOIName, PRINT_TO_SCREEN);
    }
    break;
    case VESSEL_NAME_MESSAGE: {
      // Parse the vessel name
      String vesselName = (char *) msg;
      vesselName[msgSize] = '\0';

      // Echo it to KSP
      mySimpit.printToKSP("I'm " + vesselName, PRINT_TO_SCREEN);
    }
    break;
  }
}
