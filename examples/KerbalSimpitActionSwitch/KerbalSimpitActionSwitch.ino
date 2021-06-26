/* KerbalSimpitActionSwitch
   A demonstration of using two switches to command both SAS and RCS.
   In this example, the KSP state is monitored to update the SAS and RCS
   to match the switch state. This ensure that when using the keyboard or when
   switching vessels, the game will still match the switch positions.
   In addition, the LED_BUILTIN matches the SAS state.

*/
#include "KerbalSimpit.h"

const int SAS_SWITCH_PIN = 3; // the pin used for controlling SAS.
const int RCS_SWITCH_PIN = 4; // the pin used for controlling RCS.

//Store the current action status, as recevied by simpit.
byte currentActionStatus = 0;

// Declare a KerbalSimpit object that will communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);

void setup() {
  // Open the serial connection.
  Serial.begin(115200);

  // Set up the build in LED, and turn it on.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Set up the two switches with builtin pullup.
  pinMode(SAS_SWITCH_PIN, INPUT_PULLUP);
  pinMode(RCS_SWITCH_PIN, INPUT_PULLUP);

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
  // Send a message to the plugin registering for the Action status channel.
  // The plugin will now regularly send Action status  messages while the
  // flight scene is active in-game.
  mySimpit.registerChannel(ACTIONSTATUS_MESSAGE);
}

void loop() {
  // Check for new serial messages.
  mySimpit.update();

  // Get the SAS switch state
  bool sas_switch_state = digitalRead(SAS_SWITCH_PIN);

  // Update the SAS to match the state, only if a change is needed to avoid
  // spamming commands.
  if(sas_switch_state && !(currentActionStatus & SAS_ACTION)){
    mySimpit.printToKSP("Activate SAS!");
    mySimpit.activateAction(SAS_ACTION);
  }
  if(!sas_switch_state && (currentActionStatus & SAS_ACTION)){
    mySimpit.printToKSP("Desactivate SAS!");
    mySimpit.deactivateAction(SAS_ACTION);
  }

  // Get the RCS switch state
  bool rcs_switch_state = digitalRead(RCS_SWITCH_PIN);

  // Update the RCS to match the state, only if a change is needed to avoid
  // spamming commands.
  if(rcs_switch_state && !(currentActionStatus & RCS_ACTION)){
    mySimpit.printToKSP("Activate RCS!");
    mySimpit.activateAction(RCS_ACTION);
  }
  if(!rcs_switch_state && (currentActionStatus & RCS_ACTION)){
    mySimpit.printToKSP("Desactivate RCS!");
    mySimpit.deactivateAction(RCS_ACTION);
  }
}

void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch(messageType) {
  case ACTIONSTATUS_MESSAGE:
    // Checking if the message is the size we expect is a very basic
    // way to confirm if the message was received properly.
    if (msgSize == 1) {
      currentActionStatus = msg[0];

      //Let the LED_BUILIN match the current SAS state
      if(currentActionStatus & SAS_ACTION){
        digitalWrite(LED_BUILTIN, HIGH);
      } else {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
    break;
  }
}
