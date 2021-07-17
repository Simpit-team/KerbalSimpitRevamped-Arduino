/* KerbalSimpitKeyboardDemo

   A demonstration of how to use KerbalSimPit to emulate keypress.
   This feature relies on a Windows-only library, so this feature will only be available on Windows

   Assume pin 3 and 4 are plugged into a momentary switch.
   Pin 3 and 4 will be used to control respectively EVA up and EVA down.

   To improve this exampe, the channel FLIGHT_STATUS_MESSAGE can be used to check if we are currently in EVA before emulating 
   Shift and Control press to not mess with throttle values.

   For momentary actions (such as quicksave, opening the map, etc.), Simpit can emulate keypress without having to split
   it in key up and key down.
*/

#include "KerbalSimpit.h"

// Set the pin numbers:
const int EVA_UP_PIN = 3; // the pin used for controlling EVA up (by emulating a left shift press)
const int EVA_DOWN_PIN = 4; // the pin used for controlling EVA down (by emulating a left ctrl press)

// Set the key values
const int LSHIFT_KEY = 0xA0; // For some reason using the SHIFT virtual key is not enough, and the left shift must be used.
const int LCTRL_KEY = 0xA2; // For some reason using the CONTROL virtual key is not enough, and the left control must be used.

bool lshift_pressed = false, lctrl_pressed = false;

// Declare a KerbalSimpit object that will
// communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);

void setup() {
  // Open the serial connection.
  Serial.begin(115200);

  // Set initial pin states  
  pinMode(EVA_UP_PIN, INPUT_PULLUP);
  pinMode(EVA_DOWN_PIN, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);
  
  // Turn on the built-in to indicate the start of the handshake process
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
}

void loop() {
  mySimpit.update();

  // Deal with the LSHIFT emulation
  bool button_state = digitalRead(EVA_UP_PIN);
  if(!lshift_pressed && !button_state){
    // Send a LSHIFT keydown
    keyboardEmulatorMessage msg(LSHIFT_KEY, KEY_DOWN_MOD);
    mySimpit.send(KEYBOARD_EMULATOR, msg); 
    lshift_pressed = true;
  }
  if(lshift_pressed && button_state){
    // Send a LSHIFT keyup
    keyboardEmulatorMessage msg(LSHIFT_KEY, KEY_UP_MOD);
    mySimpit.send(KEYBOARD_EMULATOR, msg); 
    lshift_pressed = false;
  }

  // Deal with the CTRL emulation
  button_state = digitalRead(EVA_DOWN_PIN);
  if(!lctrl_pressed && !button_state){
    // Send a CTRL keydown
    keyboardEmulatorMessage msg(LCTRL_KEY, KEY_DOWN_MOD);
    mySimpit.send(KEYBOARD_EMULATOR, msg); 
    lctrl_pressed = true;
  }
  if(lctrl_pressed && button_state){
    // Send a CTRL keyup
    keyboardEmulatorMessage msg(LCTRL_KEY, KEY_UP_MOD);
    mySimpit.send(KEYBOARD_EMULATOR, msg); 
    lctrl_pressed = false;
  }
  
}
