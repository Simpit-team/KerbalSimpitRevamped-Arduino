/* KerbalSimpitStressTest
   A demonstration of subscribing to all channel and check what is the impact
   of taking too long to call mysimpit.update.
*/
#include "KerbalSimpit.h"

// Declare a KerbalSimpit object that will
// communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);

// Store last time the log was sent to KSP
unsigned long lastTimeLogging = 0;
const long intervalLogging = 5000;

// Store number of messages received
int msgNbr = 0;

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

  // Here you can register all the channels you want to use.
  // mySimpit.registerChannel(ALTITUDE_MESSAGE);
  // ...
  
  // By default, register all available channels (even some non existant ones)
  for(byte b = 0; b <= 128; b++){
    mySimpit.registerChannel(b);
  }
}

void loop() {
  // Check for new serial messages.
  mySimpit.update();

  // Emulate a time consuming treatement. Increase/descrease this value to see the impact on the number of recevied/lost messages.
  delay(10);

  unsigned long currentMillis = millis();
  if (currentMillis - lastTimeLogging >= intervalLogging) {
    lastTimeLogging = currentMillis;

    mySimpit.printToKSP("Number of msg " + String(msgNbr));
    msgNbr = 0;
    
    mySimpit.printToKSP("Number of lost msg " + String(mySimpit.packetDroppedNbr));
    mySimpit.packetDroppedNbr = 0;
  }
}

void messageHandler(byte messageType, byte msg[], byte msgSize) {
  //Ignore all messages, just count them
  msgNbr ++;
}
