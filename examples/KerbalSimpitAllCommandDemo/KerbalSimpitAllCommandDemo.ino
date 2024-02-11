/* KerbalSimpitAllCommandDemo
  By Theresa - Tetopia - tetopia.de

    A demonstration of how to send all the data that Simpit can send. (Exception: the REQUEST_MESSAGE message)
    Once you connected your controller, you will be led through all the available features. 
    The test is split into several steps. At each step, pressing the 'continue' button will go to the next step while pressing the 'action' button will iterate on the different command for this given step.
    For instance during the 'Throttle' step, throttle will be set to the value read from the analog pin. During the Timewarp speed test, pression the 'action' button will change the timewarp speed
  
    Connect Pin 10 and Pin 11 each to a button (connect one wire of the button to the arduino pin and the other to GND).
    Pin 10 is used for the 'action' button while Pin 11 is used for the 'continue' button.
    Connect Pin A0 to a potentiometer. 
    See these links for basic descriptions of how to hook up a potentiometer:
     - https://www.arduino.cc/en/tutorial/potentiometer
     - https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v40/circuit-1b-potentiometer
*/

#include "KerbalSimpit.h"

// Set the pin numbers:
const int ANALOG_INPUT_PIN = A0; // the pin used for simulating any analog input
const int ACTION_INPUT_PIN = 10; // the pin used for simulating any button or switch
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
  pinMode(ACTION_INPUT_PIN, INPUT_PULLUP);
  pinMode(CONTINUE_TEST_PIN, INPUT_PULLUP);

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
  mySimpit.printToKSP(F("Starting test sequence"), PRINT_TO_SCREEN);
  // Write to log
  mySimpit.printToKSP(F("Verbose Logging"), VERBOSE_ONLY);
  mySimpit.printToKSP(F("No Header"), NO_HEADER); 
  delay(1000);

  // |---------------------------------------------------------------------------|
  // | Echo Request & Response, subscribing and unsubscribing to / from messages |
  // |---------------------------------------------------------------------------|
  mySimpit.printToKSP(F("Test Echo"), PRINT_TO_SCREEN);
  //Register the echo response channel
	mySimpit.registerChannel(ECHO_RESP_MESSAGE);
  delay(1000);
  //Send an Echo Request Message and wait for an answer
  while(!echoReceived) //The echoReceived get's set in the messageHandler() down below
  {
    mySimpit.send(ECHO_REQ_MESSAGE, "", 1);
    mySimpit.printToKSP(F("Waiting for echo response"), PRINT_TO_SCREEN);
    delay(1000);
    // Check for new serial messages.
    mySimpit.update();
  }
  mySimpit.printToKSP(F("Echo received"), PRINT_TO_SCREEN);
  //Deregister the echo response channel
	mySimpit.deregisterChannel(ECHO_RESP_MESSAGE);

  delay(1000);
  mySimpit.printToKSP(F("Press continue button"), PRINT_TO_SCREEN);
  //Wait for the continue button to be pressed and released.
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH);
  WaitForContinueButtonReleased();

  // |------------------------------------------------------|
  // | Activate, Deactivate and Toggle Custom Action Groups |
  // |------------------------------------------------------|
  /*  Old way of activating and deactivating action groups. 
      Use SETSINGLE_CAG_MESSAGE instead as shown below.

  mySimpit.printToKSP(F("Step : Actv&Deac CAG 1&2"), PRINT_TO_SCREEN);
  mySimpit.printToKSP(F("Use Action Button to act/deac"), PRINT_TO_SCREEN);
  //Wait for the continue button to be pressed
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    //Note: The activation and deactivation is better suited for a lever / switch than a button. But a button is what we have right now, so let's use it
    //Wait for the change from LOW to HIGH or from HIGH to LOW
    if(digitalRead(ACTION_INPUT_PIN) == LOW && lastInputButtonState == HIGH) //Button got pressed down
    {
      lastInputButtonState = LOW;
      mySimpit.activateCAG(1);
      mySimpit.activateCAG(2);
      delay(100); //Debounce the button. Note: Delay is not a good option because it pauses the program. Doesn't matter here though, because we're not doing anything else than waiting anyways
    }
    else if(digitalRead(ACTION_INPUT_PIN) == HIGH && lastInputButtonState == LOW) //Button was released
    {
      lastInputButtonState = HIGH;
      mySimpit.deactivateCAG(1);
      mySimpit.deactivateCAG(2);
      delay(100);
    }
  }
  WaitForContinueButtonReleased();

  mySimpit.printToKSP(F("Step : Toggle CAG 1&2"), PRINT_TO_SCREEN);
  mySimpit.printToKSP(F("Use Action Button to toggle CAG1&2"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      mySimpit.toggleCAG(1);
      mySimpit.toggleCAG(2);
    }
    WaitForActionButtonReleased();
  }
  WaitForContinueButtonReleased();
  */

  mySimpit.printToKSP(F("Step : Actv&Deac CAG 1&2"), PRINT_TO_SCREEN);
  mySimpit.printToKSP(F("Use Action Button to act/deac"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    //Note: The activation and deactivation is better suited for a lever / switch than a button. But a button is what we have right now, so let's use it
    //Wait for the change from LOW to HIGH or from HIGH to LOW
    if(digitalRead(ACTION_INPUT_PIN) == LOW && lastInputButtonState == HIGH) //Button got pressed down
    {
      lastInputButtonState = LOW;
      setSingleActionGroupMessage ag_msg(1, AG_ACTION_ACTIVATE);
      mySimpit.send(SETSINGLE_CAG_MESSAGE, ag_msg);
      setSingleActionGroupMessage ag_msg2(2, AG_ACTION_ACTIVATE);
      mySimpit.send(SETSINGLE_CAG_MESSAGE, ag_msg2);
      delay(100);
    }
    else if(digitalRead(ACTION_INPUT_PIN) == HIGH && lastInputButtonState == LOW) //Button was released
    {
      lastInputButtonState = HIGH;
      setSingleActionGroupMessage ag_msg(1, AG_ACTION_DEACTIVATE);
      mySimpit.send(SETSINGLE_CAG_MESSAGE, ag_msg);
      setSingleActionGroupMessage ag_msg2(2, AG_ACTION_DEACTIVATE);
      mySimpit.send(SETSINGLE_CAG_MESSAGE, ag_msg2);
      delay(100);
    }
  }
  WaitForContinueButtonReleased();

  mySimpit.printToKSP(F("Step : Toggle CAG 1&2"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      setSingleActionGroupMessage ag_msg(1, AG_ACTION_TOGGLE);
      mySimpit.send(SETSINGLE_CAG_MESSAGE, ag_msg);
      setSingleActionGroupMessage ag_msg2(2, AG_ACTION_TOGGLE);
      mySimpit.send(SETSINGLE_CAG_MESSAGE, ag_msg2);
    }
    WaitForActionButtonReleased();
  }
  WaitForContinueButtonReleased();

  // |-----------------|
  // | Stage and Abort |
  // |-----------------|
  mySimpit.printToKSP(F("Step : Activate Stage"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      mySimpit.activateAction(STAGE_ACTION);
    }
    WaitForActionButtonReleased();
  }
  WaitForContinueButtonReleased();

  mySimpit.printToKSP(F("Step : Activate Abort"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      mySimpit.activateAction(ABORT_ACTION);
    }
    WaitForActionButtonReleased();
  }
  WaitForContinueButtonReleased();

  // |------------------------------------------------------------|
  // | Other default Action Groups: Gear, Light, SAS, RCS, Breaks |
  // |------------------------------------------------------------|
  /*  Old way of activating and deactivating action groups. 
      Does not support all Action Groups for KSP2. 
      Use SETSINGLE_AG_MESSAGE instead as shown below.

  mySimpit.printToKSP(F("Step : Act & Deact Gear"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW && lastInputButtonState == HIGH) //Button got pressed down
    {
      lastInputButtonState = LOW;
      mySimpit.activateAction(GEAR_ACTION);
      delay(100);
    }
    else if(digitalRead(ACTION_INPUT_PIN) == HIGH && lastInputButtonState == LOW) //Button was released
    {
      lastInputButtonState = HIGH;
      mySimpit.deactivateAction(GEAR_ACTION);
      delay(100);
    }
  }
  WaitForContinueButtonReleased();
  */
  
  mySimpit.printToKSP(F("Step : Act & Deact Gear"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW && lastInputButtonState == HIGH) //Button got pressed down
    {
      lastInputButtonState = LOW;
      setSingleActionGroupMessage ag_msg(ADVANCED_GEAR_ACTION, AG_ACTION_ACTIVATE);
      mySimpit.send(SETSINGLE_AG_MESSAGE, ag_msg);
      delay(100);
    }
    else if(digitalRead(ACTION_INPUT_PIN) == HIGH && lastInputButtonState == LOW) //Button was released
    {
      lastInputButtonState = HIGH;
      setSingleActionGroupMessage ag_msg(ADVANCED_GEAR_ACTION, AG_ACTION_DEACTIVATE);
      mySimpit.send(SETSINGLE_AG_MESSAGE, ag_msg);
      delay(100);
    }
  }
  WaitForContinueButtonReleased();

  /*  Old way of toggling action groups. 
      Does not support all Action Groups for KSP2. 
      Use SETSINGLE_AG_MESSAGE instead as shown below.

  mySimpit.printToKSP(F("Step : Toggle Gear"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      mySimpit.toggleAction(GEAR_ACTION);
    }
    WaitForActionButtonReleased();
  }
  WaitForContinueButtonReleased();
  */

  mySimpit.printToKSP(F("Step : Toggle Gear"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      setSingleActionGroupMessage ag_msg(ADVANCED_GEAR_ACTION, AG_ACTION_TOGGLE);
      mySimpit.send(SETSINGLE_AG_MESSAGE, ag_msg);
    }
    WaitForActionButtonReleased();
  }
  WaitForContinueButtonReleased();

  //The others work the same, to make the code shorter they are in this separate function
  TestActionGroup("Light", ADVANCED_LIGHT_ACTION);
  TestActionGroup("SAS", ADVANCED_SAS_ACTION);
  TestActionGroup("RCS", ADVANCED_RCS_ACTION);
  TestActionGroup("Brakes", ADVANCED_BRAKES_ACTION);
  TestActionGroup("Solar", ADVANCED_SOLAR_ACTION);
  TestActionGroup("Radiators", ADVANCED_RADIATOR_ACTION);
  TestActionGroup("Science", ADVANCED_SCIENCE_ACTION);

  // |----------|
  // | Rotation |
  // |----------|
  mySimpit.printToKSP(F("Step : Rotation test (analog)"), PRINT_TO_SCREEN);
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
  WaitForContinueButtonReleased();

  // |-------------|
  // | Translation |
  // |-------------|
  mySimpit.printToKSP(F("Step : Translation (analog)"), PRINT_TO_SCREEN);
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
  WaitForContinueButtonReleased();

  // |--------|
  // | Wheels |
  // |--------|
  mySimpit.printToKSP(F("Step : Wheels test (analog)"), PRINT_TO_SCREEN);
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
  WaitForContinueButtonReleased();

  // |----------|
  // | Throttle |
  // |----------|
  mySimpit.printToKSP(F("Step : Throttle test (analog)"), PRINT_TO_SCREEN);
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
  WaitForContinueButtonReleased();
  
  // |-----------|
  // | SAS modes |
  // |-----------|
  mySimpit.printToKSP(F("Step : Set SAS Modes"), PRINT_TO_SCREEN);
  
  // Make sure SAS is activated before setting it.
  mySimpit.activateAction(SAS_ACTION);

  lastState = AP_MANEUVER;
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      switch (lastState)
      {
        case AP_MANEUVER:
            mySimpit.printToKSP(F("Stability assist"), PRINT_TO_SCREEN);
            mySimpit.setSASMode(AP_STABILITYASSIST);
            lastState = AP_STABILITYASSIST;
          break;
        case AP_STABILITYASSIST:
          //The other SAS modes work the same, to make the code shorter they are in this separate function
          TestSas(F("Progragde"), AP_PROGRADE);
          break;
        case AP_PROGRADE:
          TestSas(F("Retrograde"), AP_RETROGRADE);
          break;
        case AP_RETROGRADE:
          TestSas(F("Normal"), AP_NORMAL);
          break;
        case AP_NORMAL:
          TestSas(F("Antinormal"), AP_ANTINORMAL);
          break;
        case AP_ANTINORMAL:
          TestSas(F("Radial In"), AP_RADIALIN);
          break;
        case AP_RADIALIN:
          TestSas(F("Radial Out"), AP_RADIALOUT);
          break;
        case AP_RADIALOUT:
          TestSas(F("Target"), AP_TARGET);
          break;
        case AP_TARGET:
          TestSas(F("Antitarget"), AP_ANTITARGET);
          break;
        case AP_ANTITARGET:
          TestSas(F("Maneuver"), AP_MANEUVER);
          break;
      }
      WaitForActionButtonReleased();
    }
  }
  WaitForContinueButtonReleased();
  
  // |--------------|
  // | Camera modes |
  // |--------------|
  mySimpit.printToKSP(F("Step : Set Camera Modes"), PRINT_TO_SCREEN);
  delay(1000);
  lastState = FLIGHT_CAMERA_LOCKED;
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      switch (lastState)
      {
        case FLIGHT_CAMERA_LOCKED:
            mySimpit.printToKSP(F("Camera Flight Mode"), PRINT_TO_SCREEN);
            mySimpit.setCameraMode(CAMERA_MODE_FLIGHT);
            lastState = CAMERA_MODE_FLIGHT;
          break;
        case CAMERA_MODE_FLIGHT:
          //The other SAS modes work the same, to make the code shorter they are in this separate function
          TestCamera(F("Camera Auto Mode"), FLIGHT_CAMERA_AUTO);
          break;
        case FLIGHT_CAMERA_AUTO:
          TestCamera(F("Camera Free Mode"), FLIGHT_CAMERA_FREE);
          break;
        case FLIGHT_CAMERA_FREE:
          TestCamera(F("Camera Orbital Mode"), FLIGHT_CAMERA_ORBITAL);
          break;
        case FLIGHT_CAMERA_ORBITAL:
          TestCamera(F("Camera Chase Mode"), FLIGHT_CAMERA_CHASE);
          break;
        case FLIGHT_CAMERA_CHASE:
          TestCamera(F("Camera Locked Mode"), FLIGHT_CAMERA_LOCKED);
          break;
      }
      WaitForActionButtonReleased();
    }
  }
  WaitForContinueButtonReleased();
  mySimpit.printToKSP(F("Next Camera Mode"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW) mySimpit.setCameraMode(CAMERA_NEXT_MODE);
    WaitForActionButtonReleased();  
  }
  WaitForContinueButtonReleased();
  mySimpit.printToKSP(F("Previous Camera Mode"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW) mySimpit.setCameraMode(CAMERA_PREVIOUS_MODE);
    WaitForActionButtonReleased();  
  }
  WaitForContinueButtonReleased();

  // |-----------------|
  // | Camera Rotation |
  // |-----------------|
  mySimpit.printToKSP(F("Camera Rotation (analog)"), PRINT_TO_SCREEN);
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
  WaitForContinueButtonReleased();

  // |----------------------------|
  // | Set Timewarp Speed in KSP1 |
  // |----------------------------|
  if(!mySimpit.connectedToKSP2())
  {
    mySimpit.printToKSP(F("Step : Set TW outside Atmo"), PRINT_TO_SCREEN);
    lastState = TIMEWARP_X100000;
    while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
    {
      if(digitalRead(ACTION_INPUT_PIN) == LOW)
      {
        switch (lastState)
        {
          case TIMEWARP_X100000:
              mySimpit.printToKSP(F("TW KSP1 x1"), PRINT_TO_SCREEN);
              timewarpMessage tw_msg;
              tw_msg.command = TIMEWARP_X1;
              mySimpit.send(TIMEWARP_MESSAGE, tw_msg);
              lastState = TIMEWARP_X1;
            break;
          case TIMEWARP_X1:
            TestTimewarp(F("TW KSP1 x5"), TIMEWARP_X5);
            break;
          case TIMEWARP_X5:
            TestTimewarp(F("TW KSP1 x10"), TIMEWARP_X10);
            break;
          case TIMEWARP_X10:
            TestTimewarp(F("TW KSP1 x50"), TIMEWARP_X50);
            break;
          case TIMEWARP_X50:
            TestTimewarp(F("TW KSP1 x100"), TIMEWARP_X100);
            break;
          case TIMEWARP_X100:
            TestTimewarp(F("TW KSP1 x1 000"), TIMEWARP_X1000);
            break;
          case TIMEWARP_X1000:
            TestTimewarp(F("TW KSP1 x10 000"), TIMEWARP_X10000);
            break;
          case TIMEWARP_X10000:
            TestTimewarp(F("TW KSP1 x100 000"), TIMEWARP_X100000);
            break;
        }
        WaitForActionButtonReleased();
      }
    }
    WaitForContinueButtonReleased();

    mySimpit.printToKSP(F("Step : set phys TW in Atmo"), PRINT_TO_SCREEN);
    lastState = TIMEWARP_X4_PHYSICAL;
    while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
    {
      if(digitalRead(ACTION_INPUT_PIN) == LOW)
      {
        switch (lastState)
        {
          case TIMEWARP_X4_PHYSICAL:
            if(digitalRead(ACTION_INPUT_PIN) == LOW)
            {
              mySimpit.printToKSP(F("TW phys x1"), PRINT_TO_SCREEN);
              timewarpMessage tw_msg;
              tw_msg.command = TIMEWARP_X1_PHYSICAL;
              mySimpit.send(TIMEWARP_MESSAGE, tw_msg);
              lastState = TIMEWARP_X1_PHYSICAL;
            }
            break;
          case TIMEWARP_X1_PHYSICAL:
            if(digitalRead(ACTION_INPUT_PIN) == LOW) TestTimewarp("TW phys x2", TIMEWARP_X2_PHYSICAL);
            break;
          case TIMEWARP_X2_PHYSICAL:
            if(digitalRead(ACTION_INPUT_PIN) == LOW) TestTimewarp("TW phys x3", TIMEWARP_X3_PHYSICAL);
            break;
          case TIMEWARP_X3_PHYSICAL:
            if(digitalRead(ACTION_INPUT_PIN) == LOW) TestTimewarp("TW phys x4", TIMEWARP_X4_PHYSICAL);
            break;
        }
        WaitForActionButtonReleased();
      }
    }
    WaitForContinueButtonReleased();
  }

  // |----------------------------|
  // | Set Timewarp Speed in KSP2 |
  // |----------------------------|
  if(mySimpit.connectedToKSP2())
  {
    mySimpit.printToKSP(F("Step : Set TW"), PRINT_TO_SCREEN);
    lastState = TIMEWARP_KSP2_X10000000;
    while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
    {
      if(digitalRead(ACTION_INPUT_PIN) == LOW)
      {
        switch (lastState)
        {
          case TIMEWARP_KSP2_X10000000:
              mySimpit.printToKSP(F("TW KSP2 x1"), PRINT_TO_SCREEN);
              timewarpMessage tw_msg;
              tw_msg.command = TIMEWARP_X1;
              mySimpit.send(TIMEWARP_MESSAGE, tw_msg);
              lastState = TIMEWARP_KSP2_X1;
            break;
          case TIMEWARP_X1:
            TestTimewarp(F("TW KSP2 x2"), TIMEWARP_KSP2_X2);
            break;
          case TIMEWARP_KSP2_X2:
            TestTimewarp(F("TW KSP2 x4"), TIMEWARP_KSP2_X4);
            break;
          case TIMEWARP_KSP2_X4:
            TestTimewarp(F("TW KSP2 x10"), TIMEWARP_KSP2_X10);
            break;
          case TIMEWARP_KSP2_X10:
            TestTimewarp(F("TW KSP2 x50"), TIMEWARP_KSP2_X50);
            break;
          case TIMEWARP_KSP2_X50:
            TestTimewarp(F("TW KSP2 x100"), TIMEWARP_KSP2_X100);
            break;
          case TIMEWARP_KSP2_X100:
            TestTimewarp(F("TW KSP2 x1 000"), TIMEWARP_KSP2_X1000);
            break;
          case TIMEWARP_KSP2_X1000:
            TestTimewarp(F("TW KSP2 x10 000"), TIMEWARP_KSP2_X10000);
            break;
          case TIMEWARP_KSP2_X10000:
            TestTimewarp(F("TW KSP2 x100 000"), TIMEWARP_KSP2_X100000);
            break;
          case TIMEWARP_KSP2_X100000:
            TestTimewarp(F("TW KSP2 x1 000 000"), TIMEWARP_KSP2_X1000000);
            break;
          case TIMEWARP_KSP2_X1000000:
            TestTimewarp(F("TW KSP2 x10 000 000"), TIMEWARP_KSP2_X10000000);
            break;
        }
        WaitForActionButtonReleased();
      }
    }
    WaitForContinueButtonReleased();
  }

  // |-----------------------|
  // | Change Timewarp Speed |
  // |-----------------------|
  mySimpit.printToKSP(F("Step : TW up"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      timewarpMessage tw_msg;
      tw_msg.command = TIMEWARP_UP;
      mySimpit.send(TIMEWARP_MESSAGE, tw_msg);
    }
    WaitForActionButtonReleased();  
  }
  WaitForContinueButtonReleased();
  mySimpit.printToKSP(F("Step : TW down"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      timewarpMessage tw_msg;
      tw_msg.command = TIMEWARP_DOWN;
      mySimpit.send(TIMEWARP_MESSAGE, tw_msg);
    }
    WaitForActionButtonReleased();  
  }
  WaitForContinueButtonReleased();
  mySimpit.printToKSP(F("Step : TW stop"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      timewarpMessage tw_msg;
      tw_msg.command = TIMEWARP_CANCEL_AUTOWARP;
      mySimpit.send(TIMEWARP_MESSAGE, tw_msg);
    }
    WaitForActionButtonReleased();  
  }
  WaitForContinueButtonReleased();

  // |---------------------------|
  // | Timewarp To Point in Time |
  // |---------------------------|
  mySimpit.printToKSP(F("TW to now +5min"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      timewarpToMessage twTo_msg(TIMEWARP_TO_NOW, 5*60);
      mySimpit.send(TIMEWARP_TO_MESSAGE, twTo_msg);
    }
    WaitForActionButtonReleased();  
  }
  WaitForContinueButtonReleased();

  //The others work the same, to make the code shorter they are in this separate function
  TestTimewarpTo(F("TW to Mnvr -90sec"), TIMEWARP_TO_NEXT_MANEUVER, -90);
  TestTimewarpTo(F("TW to Burn - 10sec"), TIMEWARP_TO_NEXT_BURN, -10);
  TestTimewarpTo(F("TW to next SOI"), TIMEWARP_TO_NEXT_SOI, 0);
  TestTimewarpTo(F("TW to Ap -60sec"), TIMEWARP_TO_APOAPSIS, -60);
  TestTimewarpTo(F("TW to Pe -60sec"), TIMEWARP_TO_PERIAPSIS, -60);
  TestTimewarpTo(F("TW to 1h after morning"), TIMEWARP_TO_NEXT_MORNING, 60*60);

  // |--------------------|
  // | Keyboard emulation |
  // |--------------------|
  //For more details see the KerbalSimpitKeyboardDemo
  mySimpit.printToKSP(F("Step : Keyboard emulation (map)"), PRINT_TO_SCREEN);
  const int M_KEY = 0x4D;
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      keyboardEmulatorMessage msg(M_KEY);
      mySimpit.send(KEYBOARD_EMULATOR, msg);
    }
    WaitForActionButtonReleased();
  }
  WaitForContinueButtonReleased();

  // |-------------|
  // | Custom Axis |
  // |-------------|
  mySimpit.printToKSP(F("Step : Custom Axis (analog)"), PRINT_TO_SCREEN);
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
  WaitForContinueButtonReleased();

  // |--------------|
  // | NavBall Mode |
  // |--------------|
  mySimpit.printToKSP(F("Step : Cycle NavBall"), PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      mySimpit.cycleNavBallMode();
    }
    WaitForActionButtonReleased();
  }
  WaitForContinueButtonReleased();
  
  // |-------------------|
  // | Serial Connection |
  // |-------------------|
  mySimpit.printToKSP(F("Test completed. Closing."), PRINT_TO_SCREEN);
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

// Function called when the Continue button has just been pressed. Will debounce it and wait for it to be released.
void WaitForContinueButtonReleased()
{
  //The delay is to not skip ahead if the button bounces (Debounce the button) 
  //Note: A delay is usually not a good option to debounce buttons, because it pauses the program.
  delay(100);
  //Wait for the continue button to be released
  while (digitalRead(CONTINUE_TEST_PIN) == LOW);
  delay(100);
}

void WaitForActionButtonReleased()
{
  delay(10);
  while (digitalRead(ACTION_INPUT_PIN) == LOW);
  delay(100);
}

void TestActionGroup(String name, byte actionGroupIndex)
{
  mySimpit.printToKSP("Step : Act/Deact " + name, PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW && lastInputButtonState == HIGH) //Button got pressed down
    {
      lastInputButtonState = LOW;
      setSingleActionGroupMessage ag_msg(actionGroupIndex, AG_ACTION_ACTIVATE);
      mySimpit.send(SETSINGLE_AG_MESSAGE, ag_msg);
      delay(100);
    }
    else if(digitalRead(ACTION_INPUT_PIN) == HIGH && lastInputButtonState == LOW) //Button was released
    {
      lastInputButtonState = HIGH;
      setSingleActionGroupMessage ag_msg(actionGroupIndex, AG_ACTION_DEACTIVATE);
      mySimpit.send(SETSINGLE_AG_MESSAGE, ag_msg);
      delay(100);
    }
  }
  WaitForContinueButtonReleased();

  mySimpit.printToKSP("Step : Toggle " + name, PRINT_TO_SCREEN);
  while (digitalRead(CONTINUE_TEST_PIN) == HIGH)
  {
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      setSingleActionGroupMessage ag_msg(actionGroupIndex, AG_ACTION_TOGGLE);
      mySimpit.send(SETSINGLE_AG_MESSAGE, ag_msg);
    }
    WaitForActionButtonReleased();
  }
  WaitForContinueButtonReleased();
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
    if(digitalRead(ACTION_INPUT_PIN) == LOW)
    {
      timewarpToMessage twTo_msg(pointInTime, delay);
      mySimpit.send(TIMEWARP_TO_MESSAGE, twTo_msg);
    }
    WaitForActionButtonReleased();
  }
  WaitForContinueButtonReleased();  
}