/* KerbalSimpitAllTheButtonsAndLevers
  By Theresa - Tetopia - tetopia.de

   A demonstration of how to send all the data that Simpit can send. (Exception: the REQUEST_MESSAGE message)
   Once you connected your controller, you will be led through all the available features.
   
   Connect Pin 10 and Pin 11 each to a button (connect one wire of the button to the arduino pin and the other to GND)
   Connect Pin A0 to a potentiometer. 
   See these links for basic descriptions of how to hook up a potentiometer:
     - https://www.arduino.cc/en/tutorial/potentiometer
     - https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v40/circuit-1b-potentiometer
*/

#include "KerbalSimpit.h"

// Set the pin numbers:
const int ANALOG_INPUT_PIN = A0; // the pin used for simulating any analog input
const int BUTTON_INPUT_PIN = 10; // the pin used for simulating any button or switch
const int CONTINUE_TEST_PIN = 11; // the pin used for going forward to the next step

// Declare a KerbalSimpit object that will
// communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);
bool echoReceived = false;
bool lastInputButtonState = HIGH; 
byte lastState; //This is for temporarily holding the state of a system, e.g. to know which SAS mode was selected last

void setup() {

  // |-------|
  // | Setup |
  // |-------|
  // Open the serial connection.
  Serial.begin(115200);

  // Set initial pin states  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ANALOG_INPUT_PIN, INPUT);
  pinMode(BUTTON_INPUT_PIN, INPUT_PULLUP);
  pinMode(CONTINUE_TEST_PIN, INPUT_PULLUP);
  
  //Ignore these two lines. TODO: Delete them
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW); 

  // Turn on the built-in to indicate the start of the handshake process
  digitalWrite(LED_BUILTIN, HIGH); 

  // This loop continually attempts to handshake with the plugin.
  // It will keep retrying until it gets a successful handshake.
  while (!mySimpit.init()) {
    delay(100);
  }
	mySimpit.inboundHandler(messageHandler);

  // Turn off the built-in LED to indicate handshaking is complete.
  digitalWrite(LED_BUILTIN, LOW);
  // |--------------------------------------|
  // | Display text on screen, write to log |
  // |--------------------------------------|
  // Display a message on screen in KSP and write to the log
  mySimpit.printToKSP("Starting test sequence", PRINT_TO_SCREEN);
  // Write to log
  mySimpit.printToKSP("Verbose Logging", VERBOSE_ONLY);
  mySimpit.printToKSP("No Header", NO_HEADER); 
  delay(1000);

  // |---------------------------------------------------------------------------|
  // | Echo Request & Response, subscribing and unsubscribing to / from messages |
  // |---------------------------------------------------------------------------|
  mySimpit.printToKSP("Test Echo", PRINT_TO_SCREEN);
  //Register the echo response channel
	mySimpit.registerChannel(ECHO_RESP_MESSAGE);
  delay(1000);
  //Send an Echo Request Message and wait for an answer
  while(!echoReceived) //The echoReceived get's set in the messageHandler() down below
  {
    mySimpit.send(ECHO_REQ_MESSAGE, "", 1);
    mySimpit.printToKSP("Waiting for echo response", PRINT_TO_SCREEN);
    delay(1000);
    // Check for new serial messages.
    mySimpit.update();
  }
  mySimpit.printToKSP("Echo received", PRINT_TO_SCREEN);
  //Deregister the echo response channel
	mySimpit.deregisterChannel(ECHO_RESP_MESSAGE);

  delay(1000);
  mySimpit.printToKSP("Press continue button (Pin 11)", PRINT_TO_SCREEN);
  //Wait for the continue button to be pressed and released.
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH);
  WaitForContinueButton();

  // |------------------------------------------------------|
  // | Activate, Deactivate and Toggle Custom Action Groups |
  // |------------------------------------------------------|
  mySimpit.printToKSP("Actv&Deac CustomActionGrp 1&2", PRINT_TO_SCREEN);
  mySimpit.printToKSP("Use Button on Pin 10", PRINT_TO_SCREEN);
  //Wait for the continue button to be pressed
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    //Note: The activation and deactivation is better suited for a lever / switch than a button. But a button is what we have right now, so let's use it
    //Wait for the change from LOW to HIGH or from HIGH to LOW
    if(digitalRead(BUTTON_INPUT_PIN) == LOW && lastInputButtonState == HIGH) //Button got pressed down
    {
      lastInputButtonState = LOW;
      mySimpit.activateCAG(1);
      mySimpit.activateCAG(2);
      delay(100); //Debounce the button. Note: Delay is not a good option because it pauses the program. Doesn't matter here though, because we're not doing anything else than waiting anyways
    }
    else if(digitalRead(BUTTON_INPUT_PIN) == HIGH && lastInputButtonState == LOW) //Button was released
    {
      lastInputButtonState = HIGH;
      mySimpit.deactivateCAG(1);
      mySimpit.deactivateCAG(2);
      delay(100);
    }
  }
  WaitForContinueButton();
  mySimpit.printToKSP("Tgl CustomActionGroup 1 & 2", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW)
    {
      mySimpit.toggleCAG(1);
      mySimpit.toggleCAG(2);
    }
    WaitForTestButton();
  }
  WaitForContinueButton();

  // |-----------------|
  // | Stage and Abort |
  // |-----------------|
  mySimpit.printToKSP("Activate Stage", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW)
    {
      mySimpit.activateAction(STAGE_ACTION);
    }
    WaitForTestButton();
  }
  WaitForContinueButton();
  mySimpit.printToKSP("Activate Abort", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW)
    {
      mySimpit.activateAction(ABORT_ACTION);
    }
    WaitForTestButton();
  }
  WaitForContinueButton();

  // |------------------------------------------------------------|
  // | Other default Action Groups: Gear, Light, SAS, RCS, BRAKES |
  // |------------------------------------------------------------|
  mySimpit.printToKSP("Activate & Deactivate Gear", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW && lastInputButtonState == HIGH) //Button got pressed down
    {
      lastInputButtonState = LOW;
      mySimpit.activateAction(GEAR_ACTION);
      delay(100);
    }
    else if(digitalRead(BUTTON_INPUT_PIN) == HIGH && lastInputButtonState == LOW) //Button was released
    {
      lastInputButtonState = HIGH;
      mySimpit.deactivateAction(GEAR_ACTION);
      delay(100);
    }
  }
  WaitForContinueButton();
  mySimpit.printToKSP("Tgl Gear", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW)
    {
      mySimpit.toggleAction(GEAR_ACTION);
    }
    WaitForTestButton();
  }

  //The others work the same, to make the code shorter they are in this separate function
  TestActionGroup("Light", LIGHT_ACTION);
  TestActionGroup("RCS", RCS_ACTION);
  TestActionGroup("SAS", SAS_ACTION);
  TestActionGroup("Brakes", BRAKES_ACTION);
  WaitForContinueButton();

  // |----------|
  // | Rotation |
  // |----------|
  mySimpit.printToKSP("Rotation test (Pin A0)", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    rotationMessage rot_msg;
    // Read the values of the potentiometers (here all axis read from the same potentiometer, because only one pot connected. You would want each of them on one axis of your joystick.)
    int reading_pitch = analogRead(ANALOG_INPUT_PIN);
    int reading_roll = analogRead(ANALOG_INPUT_PIN);
    int reading_yaw = analogRead(ANALOG_INPUT_PIN);
    // Convert them in KerbalSimpit range
    int16_t pitch = map(reading_pitch, 0, 1023, INT16_MIN, INT16_MAX);
    int16_t roll = map(reading_roll, 0, 1023, INT16_MIN, INT16_MAX);
    int16_t yaw = map(reading_roll, 0, 1023, INT16_MIN, INT16_MAX);
    // Put those values in the message one at a time, e.g. if you only want to set one or two axis
    rot_msg.setPitch(pitch);
    rot_msg.setRoll(roll);
    rot_msg.setYaw(yaw);
    mySimpit.send(ROTATION_MESSAGE, rot_msg);   
  }
  //Reset the rotation, so the game can take over again
  rotationMessage rot_msg;
  rot_msg.setPitchRollYaw(0, 0, 0); // Put the values in the message all at one time, for if you want to set all axis
  mySimpit.send(ROTATION_MESSAGE, rot_msg); 
  WaitForContinueButton();

  // |-------------|
  // | Translation |
  // |-------------|
  mySimpit.printToKSP("Translation test (Pin A0)", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    translationMessage trans_msg;
    int reading_x = analogRead(ANALOG_INPUT_PIN);
    int reading_y = analogRead(ANALOG_INPUT_PIN);
    int reading_z = analogRead(ANALOG_INPUT_PIN);
    trans_msg.setX(map(reading_x, 0, 1023, INT16_MIN, INT16_MAX));
    trans_msg.setY(map(reading_y, 0, 1023, INT16_MIN, INT16_MAX));
    trans_msg.setZ(map(reading_z, 0, 1023, INT16_MIN, INT16_MAX));
    mySimpit.send(TRANSLATION_MESSAGE, trans_msg);   
  }
  translationMessage trans_msg;
  trans_msg.setXYZ(0, 0, 0);
  mySimpit.send(TRANSLATION_MESSAGE, trans_msg); 
  WaitForContinueButton();

  // |--------|
  // | Wheels |
  // |--------|
  mySimpit.printToKSP("Wheels test (Pin A0)", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    wheelMessage wheel_msg;
    int reading_throttle = analogRead(ANALOG_INPUT_PIN);
    int reading_steering = analogRead(ANALOG_INPUT_PIN);
    wheel_msg.setSteer(map(reading_steering, 0, 1023, INT16_MIN, INT16_MAX));
    wheel_msg.setThrottle(map(reading_throttle, 0, 1023, INT16_MIN, INT16_MAX));
    mySimpit.send(WHEEL_MESSAGE, wheel_msg);   
  }
  wheelMessage wheel_msg;
  wheel_msg.setSteerThrottle(0, 0);
  mySimpit.send(WHEEL_MESSAGE, wheel_msg); 
  WaitForContinueButton();

  // |----------|
  // | Throttle |
  // |----------|
  mySimpit.printToKSP("Throttle test (Pin A0)", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    throttleMessage throttle_msg;
    int reading = analogRead(ANALOG_INPUT_PIN);
    throttle_msg.throttle = map(reading, 0, 1023, 0, INT16_MAX);
    mySimpit.send(THROTTLE_MESSAGE, throttle_msg);
  }
  throttleMessage throttle_msg;
  throttle_msg.throttle = 0;
  mySimpit.send(THROTTLE_MESSAGE, throttle_msg);
  WaitForContinueButton();
  
  // |-----------|
  // | SAS modes |
  // |-----------|
  mySimpit.printToKSP("Set SAS Modes (Pin 11)", PRINT_TO_SCREEN);
  delay(1000);
  lastState = AP_MANEUVER;
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW)
    {
      switch (lastState)
      {
        case AP_MANEUVER:
            mySimpit.printToKSP("Stability assist", PRINT_TO_SCREEN);
            mySimpit.setSASMode(AP_STABILITYASSIST);
            lastState = AP_STABILITYASSIST;
          break;
        case AP_STABILITYASSIST:
          //The other SAS modes work the same, to make the code shorter they are in this separate function
          TestSas("Progragde", AP_PROGRADE);
          break;
        case AP_PROGRADE:
          TestSas("Retrograde", AP_RETROGRADE);
          break;
        case AP_RETROGRADE:
          TestSas("Normal", AP_NORMAL);
          break;
        case AP_NORMAL:
          TestSas("Antinormal", AP_ANTINORMAL);
          break;
        case AP_ANTINORMAL:
          TestSas("Radial In", AP_RADIALIN);
          break;
        case AP_RADIALIN:
          TestSas("Radial Out", AP_RADIALOUT);
          break;
        case AP_RADIALOUT:
          TestSas("Target", AP_TARGET);
          break;
        case AP_TARGET:
          TestSas("Antitarget", AP_ANTITARGET);
          break;
        case AP_ANTITARGET:
          TestSas("Maneuver", AP_MANEUVER);
          break;
      }
      WaitForTestButton();
    }
  }
  WaitForContinueButton();
  
  // |--------------|
  // | Camera modes |
  // |--------------|
  mySimpit.printToKSP("Set Camera Modes (Pin 11)", PRINT_TO_SCREEN);
  delay(1000);
  lastState = FLIGHT_CAMERA_LOCKED;
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW)
    {
      switch (lastState)
      {
        case FLIGHT_CAMERA_LOCKED:
            mySimpit.printToKSP("Camera Flight Mode", PRINT_TO_SCREEN);
            mySimpit.setCameraMode(CAMERA_MODE_FLIGHT);
            lastState = CAMERA_MODE_FLIGHT;
          break;
        case CAMERA_MODE_FLIGHT:
          //The other SAS modes work the same, to make the code shorter they are in this separate function
          TestCamera("Camera Auto Mode", FLIGHT_CAMERA_AUTO);
          break;
        case FLIGHT_CAMERA_AUTO:
          TestCamera("Camera Free Mode", FLIGHT_CAMERA_FREE);
          break;
        case FLIGHT_CAMERA_FREE:
          TestCamera("Camera Orbital Mode", FLIGHT_CAMERA_ORBITAL);
          break;
        case FLIGHT_CAMERA_ORBITAL:
          TestCamera("Camera Chase Mode", FLIGHT_CAMERA_CHASE);
          break;
        case FLIGHT_CAMERA_CHASE:
          TestCamera("Camera Locked Mode", FLIGHT_CAMERA_LOCKED);
          break;
      }
      WaitForTestButton();
    }
  }
  WaitForContinueButton();
  mySimpit.printToKSP("Next Camera Mode (Pin 11)", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW) mySimpit.setCameraMode(CAMERA_NEXT_MODE);
    WaitForTestButton();  
  }
  WaitForContinueButton();
  mySimpit.printToKSP("Previous Camera Mode (Pin 11)", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW) mySimpit.setCameraMode(CAMERA_PREVIOUS_MODE);
    WaitForTestButton();  
  }
  WaitForContinueButton();

  // |------------------|
  // | Camera Switching |
  // |------------------|
  mySimpit.printToKSP("Next Camera (Pin 11)", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW) mySimpit.setCameraMode(CAMERA_NEXT);
    WaitForTestButton();  
  }
  WaitForContinueButton();
  mySimpit.printToKSP("Previous Camera (Pin 11)", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW) mySimpit.setCameraMode(CAMERA_PREVIOUS);
    WaitForTestButton();  
  }
  WaitForContinueButton();

  // |-----------------|
  // | Camera Rotation |
  // |-----------------|
  mySimpit.printToKSP("Camera Rotation (Pin A0)", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    cameraRotationMessage camRot_msg;
    int reading_pitch = analogRead(ANALOG_INPUT_PIN);
    int reading_roll = analogRead(ANALOG_INPUT_PIN);
    int reading_yaw = analogRead(ANALOG_INPUT_PIN);
    int reading_zoom = analogRead(ANALOG_INPUT_PIN);
    camRot_msg.setPitch(map(reading_pitch, 0, 1023, INT16_MIN, INT16_MAX));
    camRot_msg.setRoll(map(reading_roll, 0, 1023, INT16_MIN, INT16_MAX));
    camRot_msg.setYaw(map(reading_yaw, 0, 1023, INT16_MIN, INT16_MAX));
    camRot_msg.setZoom(map(reading_zoom, 0, 1023, INT16_MIN, INT16_MAX));
    mySimpit.send(CAMERA_ROTATION_MESSAGE, camRot_msg);
  }
  cameraRotationMessage camRot_msg;
  camRot_msg.setPitchRollYawZoom(0, 0, 0, 0);
  mySimpit.send(CAMERA_ROTATION_MESSAGE, camRot_msg);
  WaitForContinueButton();

  // |--------------------|
  // | Set Timewarp Speed |
  // |--------------------|
  mySimpit.printToKSP("Set Timewarp outside Athmo", PRINT_TO_SCREEN);
  lastState = TIMEWARP_X100000;
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW)
    {
      switch (lastState)
      {
        case TIMEWARP_X100000:
            mySimpit.printToKSP("TW x1", PRINT_TO_SCREEN);
            timewarpMessage tw_msg;
            tw_msg.command = TIMEWARP_X1;
            mySimpit.send(TIMEWARP_MESSAGE, tw_msg);
            lastState = TIMEWARP_X1;
          break;
        case TIMEWARP_X1:
          TestTimewarp("TW x5", TIMEWARP_X5);
          break;
        case TIMEWARP_X5:
          TestTimewarp("TW x10", TIMEWARP_X10);
          break;
        case TIMEWARP_X10:
          TestTimewarp("TW x50", TIMEWARP_X50);
          break;
        case TIMEWARP_X50:
          TestTimewarp("TW x100", TIMEWARP_X100);
          break;
        case TIMEWARP_X100:
          TestTimewarp("TW x1000", TIMEWARP_X1000);
          break;
        case TIMEWARP_X1000:
          TestTimewarp("TW x10000", TIMEWARP_X10000);
          break;
        case TIMEWARP_X10000:
          TestTimewarp("TW x100000", TIMEWARP_X100000);
          break;
      }
      WaitForTestButton();
    }
  }
  WaitForContinueButton();

  mySimpit.printToKSP("Set phys Timewarp in Athmo", PRINT_TO_SCREEN);
  lastState = TIMEWARP_X4_PHYSICAL;
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    switch (lastState)
    {
      case TIMEWARP_X4_PHYSICAL:
        if(digitalRead(BUTTON_INPUT_PIN) == LOW)
        {
          mySimpit.printToKSP("TW phys x1", PRINT_TO_SCREEN);
          timewarpMessage tw_msg;
          tw_msg.command = TIMEWARP_X1_PHYSICAL;
          mySimpit.send(TIMEWARP_MESSAGE, tw_msg);
          lastState = TIMEWARP_X1_PHYSICAL;
          WaitForTestButton();
        }
        break;
      case TIMEWARP_X1_PHYSICAL:
        if(digitalRead(BUTTON_INPUT_PIN) == LOW) TestTimewarp("TW phys x2", TIMEWARP_X2_PHYSICAL);
        break;
      case TIMEWARP_X2_PHYSICAL:
        if(digitalRead(BUTTON_INPUT_PIN) == LOW) TestTimewarp("TW phys x3", TIMEWARP_X3_PHYSICAL);
        break;
      case TIMEWARP_X3_PHYSICAL:
        if(digitalRead(BUTTON_INPUT_PIN) == LOW) TestTimewarp("TW phys x4", TIMEWARP_X4_PHYSICAL);
        break;
    }
  }
  WaitForContinueButton();

  // |-----------------------|
  // | Change Timewarp Speed |
  // |-----------------------|
  mySimpit.printToKSP("TW up (Pin 11)", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW)
    {
      timewarpMessage tw_msg;
      tw_msg.command = TIMEWARP_UP;
      mySimpit.send(TIMEWARP_MESSAGE, tw_msg);
    }
    WaitForTestButton();  
  }
  WaitForContinueButton();
  mySimpit.printToKSP("TW down (Pin 11)", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW)
    {
      timewarpMessage tw_msg;
      tw_msg.command = TIMEWARP_DOWN;
      mySimpit.send(TIMEWARP_MESSAGE, tw_msg);
    }
    WaitForTestButton();  
  }
  WaitForContinueButton();
  mySimpit.printToKSP("TW stop (Pin 11)", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW)
    {
      timewarpMessage tw_msg;
      tw_msg.command = TIMEWARP_CANCEL_AUTOWARP;
      mySimpit.send(TIMEWARP_MESSAGE, tw_msg);
    }
    WaitForTestButton();  
  }
  WaitForContinueButton();

  // |---------------------------|
  // | Timewarp To Point in Time |
  // |---------------------------|
  mySimpit.printToKSP("TW to now +5min", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW)
    {
      timewarpToMessage twTo_msg(TIMEWARP_TO_NOW, 5*60);
      mySimpit.send(TIMEWARP_TO_MESSAGE, twTo_msg);
    }
    WaitForTestButton();  
  }
  WaitForContinueButton();

  //The others work the same, to make the code shorter they are in this separate function
  TestTimewarpTo("TW to Mnvr -90sec", TIMEWARP_TO_NEXT_MANEUVER, -90);
  TestTimewarpTo("TW to Burn - 10sec", TIMEWARP_TO_NEXT_BURN, -10);
  TestTimewarpTo("TW to next SOI", TIMEWARP_TO_NEXT_SOI, 0);
  TestTimewarpTo("TW to Ap -60sec", TIMEWARP_TO_APOAPSIS, -60);
  TestTimewarpTo("TW to Pe -60sec", TIMEWARP_TO_PERIAPSIS, -60);
  TestTimewarpTo("TW to 1h after morning", TIMEWARP_TO_NEXT_MORNING, 60*60);

  // |--------------------|
  // | Keyboard emulation |
  // |--------------------|
  //For more details see the KerbalSimpitKeyboardDemo
  mySimpit.printToKSP("Increase Throttle", PRINT_TO_SCREEN);
  const int LSHIFT_KEY = 0xA0;
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW && lastInputButtonState == HIGH) //Button got pressed down
    {
      lastInputButtonState = LOW;
      keyboardEmulatorMessage msg(LSHIFT_KEY, KEY_DOWN_MOD);
      mySimpit.send(KEYBOARD_EMULATOR, msg);
    }
    else if(digitalRead(BUTTON_INPUT_PIN) == HIGH && lastInputButtonState == LOW) //Button was released
    {
      lastInputButtonState = HIGH;
      keyboardEmulatorMessage msg(LSHIFT_KEY, KEY_UP_MOD);
      mySimpit.send(KEYBOARD_EMULATOR, msg);
      delay(100);
    }
  }
  WaitForContinueButton();
  
  mySimpit.printToKSP("Tgl Map", PRINT_TO_SCREEN);
  const int M_KEY = 0x4D;
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW && lastInputButtonState == HIGH) //Button got pressed down
    {
      lastInputButtonState = LOW;
      keyboardEmulatorMessage msg(M_KEY, KEY_DOWN_MOD);
      mySimpit.send(KEYBOARD_EMULATOR, msg);
      delay(100);
    }
    else if(digitalRead(BUTTON_INPUT_PIN) == HIGH && lastInputButtonState == LOW) //Button was released
    {
      lastInputButtonState = HIGH;
      keyboardEmulatorMessage msg(M_KEY, KEY_UP_MOD);
      mySimpit.send(KEYBOARD_EMULATOR, msg);
      delay(100);
    }
  }
  WaitForContinueButton();

  // |-------------|
  // | Custom Axis |
  // |-------------|
  mySimpit.printToKSP("Custom Axis Test (Pin A0)", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    customAxisMessage customAxis_msg;
    int reading_a = analogRead(ANALOG_INPUT_PIN);
    int reading_b = analogRead(ANALOG_INPUT_PIN);
    int reading_c = analogRead(ANALOG_INPUT_PIN);
    int reading_d = analogRead(ANALOG_INPUT_PIN);
    customAxis_msg.setCustom1(map(reading_a, 0, 1023, INT16_MIN, INT16_MAX));
    customAxis_msg.setCustom2(map(reading_b, 0, 1023, INT16_MIN, INT16_MAX));
    customAxis_msg.setCustom3(map(reading_c, 0, 1023, INT16_MIN, INT16_MAX));
    customAxis_msg.setCustom4(map(reading_d, 0, 1023, INT16_MIN, INT16_MAX));
    mySimpit.send(CUSTOM_AXIS_MESSAGE, customAxis_msg);   
  }
  customAxisMessage customAxis_msg;
  customAxis_msg.setAllCustom(0, 0, 0, 0);
  mySimpit.send(CUSTOM_AXIS_MESSAGE, customAxis_msg); 
  WaitForContinueButton();

  // |--------------|
  // | NavBall Mode |
  // |--------------|
  mySimpit.printToKSP("Cycle NavBall (Pin 11)", PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW)
    {
      mySimpit.cycleNavBallMode();
    }
    WaitForTestButton();
  }
  WaitForContinueButton();
  
  // |-------------------|
  // | Serial Connection |
  // |-------------------|
  mySimpit.printToKSP("Test completed. Closing.", PRINT_TO_SCREEN);
  mySimpit.closeSerialConnection();

}

void loop() 
{
}

  // |-----------------|
  // | Message Handler |
  // |-----------------|
void messageHandler(byte messageType, byte msg[], byte msgSize) 
{
	switch(messageType) 
  {
		case ECHO_RESP_MESSAGE:
		{			
			echoReceived = true;
		} break;
  }
}

  // |----------------------------------|
  // | Helper functions to shorten code |
  // |----------------------------------|

void WaitForContinueButton()
{
  //The delay is to not skip ahead if the button bounces (Debounce the button) 
  //Note: A delay is usually not a good option to debounce buttons, because it pauses the program.
  delay(100);
  //Wait for the continue button to be released
  while (digitalRead(CONTINUE_TEST_PIN) == LOW);
  delay(100);
}

void WaitForTestButton()
{
  delay(10);
  while (digitalRead(BUTTON_INPUT_PIN) == LOW);
  delay(100);
}

void TestActionGroup(String name, byte actionGroupIndex)
{
  WaitForContinueButton();
  String stringToprint = "Activate & Deactivate " + name;
  mySimpit.printToKSP(stringToprint, PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW && lastInputButtonState == HIGH) //Button got pressed down
    {
      lastInputButtonState = LOW;
      mySimpit.activateAction(actionGroupIndex);
      delay(100);
    }
    else if(digitalRead(BUTTON_INPUT_PIN) == HIGH && lastInputButtonState == LOW) //Button was released
    {
      lastInputButtonState = HIGH;
      mySimpit.deactivateAction(actionGroupIndex);
      delay(100);
    }
  }
  WaitForContinueButton();
  stringToprint = "Tgl " + name;
  mySimpit.printToKSP(stringToprint, PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW)
    {
      mySimpit.toggleAction(actionGroupIndex);
    }
    WaitForTestButton();
  }
}

void TestSas(String description, byte sasMode)
{
  mySimpit.printToKSP(description, PRINT_TO_SCREEN);
  mySimpit.setSASMode(sasMode);
  lastState = sasMode;
}

void TestCamera(String description, byte cameraMode)
{
  mySimpit.printToKSP(description, PRINT_TO_SCREEN);
  mySimpit.setCameraMode(cameraMode);
  lastState = cameraMode;
}

void TestTimewarp(String description, byte twCommand)
{
  mySimpit.printToKSP(description, PRINT_TO_SCREEN);
  timewarpMessage tw_msg;
  tw_msg.command = twCommand;
  mySimpit.send(TIMEWARP_MESSAGE, tw_msg);
  lastState = twCommand;
}

void TestTimewarpTo(String description, byte pointInTime, float delay)
{
  mySimpit.printToKSP(description, PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(BUTTON_INPUT_PIN) == LOW)
    {
      timewarpToMessage twTo_msg(pointInTime, delay);
      mySimpit.send(TIMEWARP_TO_MESSAGE, twTo_msg);
    }
  }
  WaitForContinueButton();  
}
