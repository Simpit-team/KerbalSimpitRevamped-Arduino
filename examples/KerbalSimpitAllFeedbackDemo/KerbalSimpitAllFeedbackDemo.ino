/* KerbalSimpitAllTheFeedback
  By Theresa - Tetopia - tetopia.de

    A demonstration of how to receive all the data that Simpit can send. 
    Once you connected your controller, you will be led through all the available features. 
    The test is split into several steps. At each step, pressing the 'continue' button will go to the next step.

    Connect Pin 11 to a button (connect one wire of the button to the arduino pin and the other to GND).
*/

#include "KerbalSimpit.h"

const int CONTINUE_TEST_PIN = 11;  // the pin used for going forward to the next step
bool lastButtonState = HIGH;       // To find when the button changes state

KerbalSimpit mySimpit(Serial);  // Declare a KerbalSimpit object that will communicate using the "Serial" device.

const int NUMBER_OF_STEPS = 54;     //The selectionIndex will be reset to 0 after reaching NUMBER_OF_STEPS-1
int selectionIndex = 0;             // increased when pushing the continue button to display different Values
unsigned long timestampLastSent;    // When was the last time something was sent to print on screen
const int SENDING_INTERVAL = 1000;  // in milliseconds. How often to print data to screen

// Use variables to store the information you get from the messages to do stuff with them in loop()
//All the values we keep track of
bool echoReceived = false;
float myAltitudeSeaLevel;
float myAltitudeSurface;
int32_t myTime1;
int32_t myTime2;
byte myCurrentSASMode;
int16_t mySASModeAvailability;
bool myCustomActionGroups[10];
byte myAdvancedActionGroups[10];
// You probably want a different variable for each value you use, just like in the echo and altitutde example above.
// Because here only a handful of variables are used at the same time, 
// they get shared between different messages (depending on my selectionIndex)
// to make the code a bit shorter.
float myResourceTotalShip;
float myResourceAvailableShip;
float myResourceTotalStage;
float myResourceAvailableStage;
float myFloatStorage1;
float myFloatStorage2;
float myFloatStorage3;
float myFloatStorage4;
float myFloatStorage5;
float myFloatStorage6;
float myFloatStorage7;
float myFloatStorage8;
int16_t myAxis1;
int16_t myAxis2;
int16_t myAxis3;
byte myByte1;
byte myByte2;
byte myByte3;
byte myByte4;
byte myByte5;
byte myByte6;
byte myByte7;
byte myByte8;
bool myBool1;
bool myBool2;
bool myBool3;
bool myBool4;
bool myBool5;
String myString;

void setup() {

  // |-------|
  // | Setup |
  // |-------|
  // Open the serial connection.
  Serial.begin(115200);

  // Set initial pin states
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(CONTINUE_TEST_PIN, INPUT_PULLUP);

  // Turn on the built-in to indicate the start of the handshake process
  digitalWrite(LED_BUILTIN, HIGH);

  // |------------------------------------------------------------|
  // | Initialize Simpit and register all* available channels     |
  // | *all Channels as of version 2.3.0, there might be new ones |
  // |------------------------------------------------------------|

  // This loop continually attempts to handshake with the plugin.
  // It will keep retrying until it gets a successful handshake.
  while (!mySimpit.init()) {
    delay(100);
  }
  // Turn off the built-in LED to indicate handshaking is complete.
  digitalWrite(LED_BUILTIN, LOW);
  // Display a message on screen in KSP
  if (mySimpit.connectedToKSP2()) mySimpit.printToKSP(F("Connected to KSP2"), PRINT_TO_SCREEN);
  else mySimpit.printToKSP(F("Connected to KSP1"), PRINT_TO_SCREEN);

  // Sets our callback function. The KerbalSimpit library will
  // call this function every time a packet is received.
  mySimpit.inboundHandler(messageHandler);
  //Register all the available channels
  // | Propulsion Resources |
  mySimpit.registerChannel(LF_MESSAGE);
  mySimpit.registerChannel(LF_STAGE_MESSAGE);
  mySimpit.registerChannel(OX_MESSAGE);
  mySimpit.registerChannel(OX_STAGE_MESSAGE);
  mySimpit.registerChannel(SF_MESSAGE);
  mySimpit.registerChannel(SF_STAGE_MESSAGE);
  mySimpit.registerChannel(XENON_GAS_MESSAGE);
  mySimpit.registerChannel(XENON_GAS_STAGE_MESSAGE);
  mySimpit.registerChannel(MONO_MESSAGE);
  mySimpit.registerChannel(EVA_MESSAGE);
  // | KSP2 only Resources |
  mySimpit.registerChannel(INTAKE_AIR_MESSAGE);
  mySimpit.registerChannel(HYDROGEN_MESSAGE);
  mySimpit.registerChannel(HYDROGEN_STAGE_MESSAGE);
  mySimpit.registerChannel(URANIUM_MESSAGE);
  // | Vessel Resources |
  mySimpit.registerChannel(ELECTRIC_MESSAGE);
  mySimpit.registerChannel(ORE_MESSAGE);
  mySimpit.registerChannel(AB_MESSAGE);
  mySimpit.registerChannel(AB_STAGE_MESSAGE);
  mySimpit.registerChannel(TACLS_RESOURCE_MESSAGE);
  mySimpit.registerChannel(TACLS_WASTE_MESSAGE);
  mySimpit.registerChannel(CUSTOM_RESOURCE_1_MESSAGE);
  mySimpit.registerChannel(CUSTOM_RESOURCE_2_MESSAGE);
  // | Vessel Movement/Position |
  mySimpit.registerChannel(ALTITUDE_MESSAGE);
  mySimpit.registerChannel(VELOCITY_MESSAGE);
  mySimpit.registerChannel(AIRSPEED_MESSAGE);
  mySimpit.registerChannel(APSIDES_MESSAGE);
  mySimpit.registerChannel(APSIDESTIME_MESSAGE);
  mySimpit.registerChannel(MANEUVER_MESSAGE);
  mySimpit.registerChannel(SAS_MODE_INFO_MESSAGE);
  mySimpit.registerChannel(ORBIT_MESSAGE);
  mySimpit.registerChannel(ROTATION_DATA_MESSAGE);
  mySimpit.registerChannel(ROTATION_CMD_MESSAGE);
  mySimpit.registerChannel(TRANSLATION_CMD_MESSAGE);
  mySimpit.registerChannel(WHEEL_CMD_MESSAGE);
  mySimpit.registerChannel(THROTTLE_CMD_MESSAGE);
  // | Vessel Details |
  mySimpit.registerChannel(ACTIONSTATUS_MESSAGE);
  mySimpit.registerChannel(DELTAV_MESSAGE);
  mySimpit.registerChannel(DELTAVENV_MESSAGE);
  mySimpit.registerChannel(BURNTIME_MESSAGE);
  mySimpit.registerChannel(CAGSTATUS_MESSAGE);
  mySimpit.registerChannel(TEMP_LIMIT_MESSAGE);
  mySimpit.registerChannel(ADVANCED_ACTIONSTATUS_MESSAGE);
  mySimpit.registerChannel(ADVANCED_CAGSTATUS_MESSAGE);
  // | External Environment |
  mySimpit.registerChannel(TARGETINFO_MESSAGE);
  mySimpit.registerChannel(SOI_MESSAGE);
  mySimpit.registerChannel(SCENE_CHANGE_MESSAGE);
  mySimpit.registerChannel(FLIGHT_STATUS_MESSAGE);
  mySimpit.registerChannel(ATMO_CONDITIONS_MESSAGE);
  mySimpit.registerChannel(VESSEL_NAME_MESSAGE);
  mySimpit.registerChannel(VESSEL_CHANGE_MESSAGE);
  mySimpit.registerChannel(INTERSECTS_MESSAGE);

  // |---------------------|
  // | Echo & Echo Request |
  // |---------------------|
  //Send an Echo Request Message and wait for an answer
  mySimpit.printToKSP(F("Test Echo"), PRINT_TO_SCREEN);
  while (!echoReceived)  //The echoReceived get's set in the messageHandler() down below
  {
    mySimpit.send(ECHO_REQ_MESSAGE, "", 1);
    mySimpit.printToKSP("Waiting for echo response", PRINT_TO_SCREEN);
    delay(SENDING_INTERVAL);
    // Check for new serial messages.
    mySimpit.update();
  }
  mySimpit.printToKSP(F("Echo received"), PRINT_TO_SCREEN);
  //Deregister the echo response channel
  mySimpit.deregisterChannel(ECHO_RESP_MESSAGE);
}

void loop() 
{
  // |------------------------------------|
  // | Print gained information on Screen |
  // |------------------------------------|
  if (millis() - timestampLastSent > SENDING_INTERVAL) 
  {
    timestampLastSent = millis();
    switch (selectionIndex) 
    {
      case 0: { //Liquid Fuel / Methane (Note: For this to work on KSP1 this needs the mod ARP to be installed)
          mySimpit.printToKSP("LF " + String(myResourceAvailableShip, 2) + "/" + String(myResourceTotalShip, 2) + " " + String(myResourceAvailableStage, 2) + "/" + String(myResourceTotalStage, 2), PRINT_TO_SCREEN);
        } break;
      case 1: { //Oxidiser (Note: For this to work on KSP1 this needs the mod ARP to be installed)
          mySimpit.printToKSP("OX " + String(myResourceAvailableShip, 2) + "/" + String(myResourceTotalShip, 2) + " " + String(myResourceAvailableStage, 2) + "/" + String(myResourceTotalStage, 2), PRINT_TO_SCREEN);
        } break;
      case 2: { //Solid Fuel (Note: For this to work on KSP1 this needs the mod ARP to be installed)
          mySimpit.printToKSP("SF " + String(myResourceAvailableShip, 2) + "/" + String(myResourceTotalShip, 2) + " " + String(myResourceAvailableStage, 2) + "/" + String(myResourceTotalStage, 2), PRINT_TO_SCREEN);
        } break;
      case 3: { //Xenon (Note: For this to work on KSP1 this needs the mod ARP to be installed)
          mySimpit.printToKSP("XE " + String(myResourceAvailableShip, 2) + "/" + String(myResourceTotalShip, 2) + " " + String(myResourceAvailableStage, 2) + "/" + String(myResourceTotalStage, 2), PRINT_TO_SCREEN);
        } break;
      case 4: { //Monopropellant (Note: For this to work on KSP1 this needs the mod ARP to be installed)
          mySimpit.printToKSP("MP " + String(myResourceAvailableShip, 2) + "/" + String(myResourceTotalShip, 2), PRINT_TO_SCREEN);
        } break;
      case 5: { //EVA propellant (Note: Only works on KSP1. This needs the mod ARP to be installed)
          mySimpit.printToKSP("EVA MP " + String(myResourceAvailableShip, 2) + "/" + String(myResourceTotalShip, 2), PRINT_TO_SCREEN);
        } break;
      case 6: { //Electric Charge (Note: For this to work on KSP1 this needs the mod ARP to be installed)
          mySimpit.printToKSP("EC " + String(myResourceAvailableShip, 2) + "/" + String(myResourceTotalShip, 2), PRINT_TO_SCREEN);
        } break;
      case 7: { //Ore (Note: Only works on KSP1. This needs the mod ARP to be installed)
          mySimpit.printToKSP("Ore " + String(myResourceAvailableShip, 2) + "/" + String(myResourceTotalShip, 2), PRINT_TO_SCREEN);
        } break;
      case 8: { //Ablator (Note: For this to work on KSP1 this needs the mod ARP to be installed)
          mySimpit.printToKSP("Abl " + String(myResourceAvailableShip, 2) + "/" + String(myResourceTotalShip, 2) + " " + String(myResourceAvailableStage, 2) + "/" + String(myResourceTotalStage, 2), PRINT_TO_SCREEN);
        } break;
      // There is also the following messages, but they will not be discussed in this example:
      // - TACLS Resource: currentFood, maxFood, currentWater, maxWater, currentOxygen, maxOxygen
      // - TACLS Waste: currentwaste, maxWaste, currentLiquidWaste, maxLiquidWaste, currentCO2, maxCO2
      // - Custom Resource 1: currentResource1 to currentResource4 and maxResource1 to maxResource4
      // - Custom Resource 2: currentResource1 to currentResource4 and maxResource1 to maxResource4

      case 9: { //Altitude
          mySimpit.printToKSP("Altitude Sea " + String(myAltitudeSeaLevel, 0) + " Surf " + String(myAltitudeSurface, 0), PRINT_TO_SCREEN);
        } break;
      case 10: { //Velocity
          mySimpit.printToKSP("Vel Orb " + String(myFloatStorage1, 0) + " Srf " + String(myFloatStorage2, 0) + " Vrt " + String(myFloatStorage3, 0), PRINT_TO_SCREEN);
        } break;
      case 11: { //Airspeed
          mySimpit.printToKSP("Airsp IAS " + String(myFloatStorage1, 0) + " Mach " + String(myFloatStorage2, 1) + " G " + String(myFloatStorage3, 1), PRINT_TO_SCREEN);
        } break;
      case 12: { //Apsides
          mySimpit.printToKSP("Apoapsis " + String(myFloatStorage2, 0) + " Periapsis " + String(myFloatStorage1, 0), PRINT_TO_SCREEN);
        } break;
      case 13: { //Apsides Times
          mySimpit.printToKSP("Time to Ap " + String(myTime1) + " Time to Pe " + String(myTime2), PRINT_TO_SCREEN);
        } break;
      case 14: { //Maneuver part 1
          mySimpit.printToKSP("Mnv Time " + String(myFloatStorage1, 0) + " dvNxt " + String(myFloatStorage2, 0) + " dur " + String(myFloatStorage3, 0), PRINT_TO_SCREEN);
        } break;
      case 15: { //Maneuver part 2
          mySimpit.printToKSP("Mnv dvTtl " + String(myFloatStorage4, 0) + " HDG " + String(myFloatStorage5, 0) + " Ptch " + String(myFloatStorage6, 0), PRINT_TO_SCREEN);
        } break;
      case 16: { //SAS
          String myCurrentSasString = "SAS "; //One or two letters for the current mode will get added
          String myAvailableSasString = " ( "; //A chain of all available SAS modes will get added
          switch(myCurrentSASMode)
          {
            case 255:
              myCurrentSasString += "-";
              break;
            case AP_STABILITYASSIST:
              myCurrentSasString += "S";
              break;
            case AP_PROGRADE:
              myCurrentSasString += "P";
              break;
            case AP_RETROGRADE:
              myCurrentSasString += "R";
              break;
            case AP_NORMAL:
              myCurrentSasString += "N";
              break;
            case AP_ANTINORMAL:
              myCurrentSasString += "AN";
              break;
            case AP_RADIALIN:
              myCurrentSasString += "I";
              break;
            case AP_RADIALOUT:
              myCurrentSasString += "O";
              break;
            case AP_TARGET:
              myCurrentSasString += "T";
              break;
            case AP_ANTITARGET:
              myCurrentSasString += "AT";
              break;
            case AP_MANEUVER:
              myCurrentSasString += "M";
              break;
          }
          if(mySASModeAvailability & ((uint16_t)1 << AP_STABILITYASSIST)) myAvailableSasString += "S ";
          if(mySASModeAvailability & ((uint16_t)1 << AP_PROGRADE)) myAvailableSasString += "P ";
          if(mySASModeAvailability & ((uint16_t)1 << AP_RETROGRADE)) myAvailableSasString += "R ";
          if(mySASModeAvailability & ((uint16_t)1 << AP_NORMAL)) myAvailableSasString += "N ";
          if(mySASModeAvailability & ((uint16_t)1 << AP_ANTINORMAL)) myAvailableSasString += "AN ";
          if(mySASModeAvailability & ((uint16_t)1 << AP_RADIALIN)) myAvailableSasString += "I ";
          if(mySASModeAvailability & ((uint16_t)1 << AP_RADIALOUT)) myAvailableSasString += "O ";
          if(mySASModeAvailability & ((uint16_t)1 << AP_TARGET)) myAvailableSasString += "T ";
          if(mySASModeAvailability & ((uint16_t)1 << AP_ANTITARGET)) myAvailableSasString += "AT ";
          if(mySASModeAvailability & ((uint16_t)1 << AP_MANEUVER)) myAvailableSasString += "M ";
          myAvailableSasString += ")";
          mySimpit.printToKSP(myCurrentSasString + myAvailableSasString, PRINT_TO_SCREEN);
        } break;
      case 17: { //Orbit Info part 1
          mySimpit.printToKSP("Ecc " + String(myFloatStorage1, 1) + " SMA " + String(myFloatStorage2, 0) + " Inc " + String(myFloatStorage3, 1), PRINT_TO_SCREEN);
        } break;
      case 18: { //Orbit Info part 2
          mySimpit.printToKSP("LAN " + String(myFloatStorage4, 0) + " ArgPe " + String(myFloatStorage5, 0) + " T " + String(myFloatStorage8, 0), PRINT_TO_SCREEN);
        } break;
      case 19: { //Orbit Info part 3
          mySimpit.printToKSP("trueAn " + String(myFloatStorage6, 2) + " meanAn " + String(myFloatStorage7, 2), PRINT_TO_SCREEN);
        } break;
      case 20: { //Vessel orientation / pointing part 1
          mySimpit.printToKSP("Vessel HDG " + String(myFloatStorage1, 0) + " PTCH " + String(myFloatStorage2, 0) + " RLL " + String(myFloatStorage3, 0), PRINT_TO_SCREEN);
        } break;
      case 21: { //Vessel orientation / pointing part 2
          mySimpit.printToKSP("Orbtl Vel HDG" + String(myFloatStorage4, 0) + " PTCH " + String(myFloatStorage5, 0), PRINT_TO_SCREEN);
        } break;
      case 22: { //Vessel orientation / pointing part 3
          mySimpit.printToKSP("Srf Vel HDG " + String(myFloatStorage6, 0) + " PTCH " + String(myFloatStorage7, 0), PRINT_TO_SCREEN);
        } break;
      case 23: { //Rotation after applying the Simpit Commands
          mySimpit.printToKSP("PTCH " + String(myAxis1) + " RLL " + String(myAxis2) + " YAW " + String(myAxis3), PRINT_TO_SCREEN);
        } break;
      case 24: { //Translation after applying the Simpit Commands
          mySimpit.printToKSP("Trsl X " + String(myAxis1) + " Y " + String(myAxis2) + " Z " + String(myAxis3), PRINT_TO_SCREEN);
        } break;
      case 25: { //Wheels after applying the Simpit Commands
          mySimpit.printToKSP("Wheels Steer " + String(myAxis1) + " Thrtl " + String(myAxis2), PRINT_TO_SCREEN);
        } break;
      case 26: { //Throttle after applying the Simpit Commands
          mySimpit.printToKSP("Throttle " + String(myAxis1), PRINT_TO_SCREEN);
        } break;
      case 27: { //Action group status Message
          String gearStatus = "0";
          if(myByte1&GEAR_ACTION) gearStatus = "1";
          String lightStatus = "0";
          if(myByte1&LIGHT_ACTION) lightStatus = "1";
          String rcsStatus = "0";
          if(myByte1&RCS_ACTION) rcsStatus = "1";
          String sasStatus = "0";
          if(myByte1&SAS_ACTION) sasStatus = "1";
          String brakesStatus = "0";
          if(myByte1&BRAKES_ACTION) brakesStatus = "1";
          mySimpit.printToKSP("Gr " + gearStatus + " Lgt " + lightStatus + " Br " + brakesStatus + " SAS " + sasStatus + " RCS " + rcsStatus, PRINT_TO_SCREEN);
        } break;
      case 28: { //Current delta v
          mySimpit.printToKSP("Delta v Stage " + String(myFloatStorage1, 0) + " Total " + String(myFloatStorage2, 0), PRINT_TO_SCREEN);
        } break;
      case 29: { //Delta v at sea level
          mySimpit.printToKSP("dv ASL Stage " + String(myFloatStorage1, 0) + " Total " + String(myFloatStorage2, 0), PRINT_TO_SCREEN);
        } break;
      case 30: { //Delta v in vacuum
          mySimpit.printToKSP("dv VAC Stage " + String(myFloatStorage3, 0) + " Total " + String(myFloatStorage4, 0), PRINT_TO_SCREEN);
        } break;
      case 31: { //Burntime
          mySimpit.printToKSP("BurnTime Stage " + String(myFloatStorage1, 0) + " Total " + String(myFloatStorage2, 0), PRINT_TO_SCREEN);
        } break;
      case 32: { //Custom Action Groups
          //Print a 0 for every action group that is off and a 1 for every action group that is on
          String cagString = "CAG";
          for (int i = 0; i < 10; i++)
          {
            if(myCustomActionGroups[i]) cagString += " 1";
            else cagString += " 0";
          }
          mySimpit.printToKSP(cagString, PRINT_TO_SCREEN);
        } break;
      case 33: { //Temperatrue info
          mySimpit.printToKSP("Temp " + String((int)myByte1) + "% Skin " + String((int)myByte2) + "%", PRINT_TO_SCREEN);
        } break;
      case 34: { //Target info part 1
          mySimpit.printToKSP("Target dist " + String(myFloatStorage1, 0) + " vel " + String(myFloatStorage2, 0), PRINT_TO_SCREEN);
        } break;
      case 35: { //Target info part 2
          mySimpit.printToKSP("Target HDG " + String(myFloatStorage2, 0) + " PTCH " + String(myFloatStorage3, 0), PRINT_TO_SCREEN);
        } break;
      case 36: { //Target info part 3
          mySimpit.printToKSP("Target v HDG " + String(myFloatStorage4, 0) + " v PTCH " + String(myFloatStorage5, 0), PRINT_TO_SCREEN);
        } break;
      case 37: { //SOI
          mySimpit.printToKSP("SOI: '" + myString + "'", PRINT_TO_SCREEN);
        } break;
      case 38: { //Scene or Vessel Change
          if(myBool1)
          {
            if(myByte1 == 0) mySimpit.printToKSP(F("Scene Changed to flight"), PRINT_TO_SCREEN);
            else mySimpit.printToKSP(F("Scene Changed leaving flight"), PRINT_TO_SCREEN);
          }
          if(myBool2) mySimpit.printToKSP(F("Vessel Changed"), PRINT_TO_SCREEN);
          myBool1 = false;
          myBool2 = false;
        } break;
      case 39: { //Flight status part 1
          mySimpit.printToKSP("InFlight:" + String(myBool1) + " EVA:" + String(myBool2) + " Rec:" + String(myBool3), PRINT_TO_SCREEN);
        } break;
      case 40: { //Flight status part 2
          mySimpit.printToKSP("AtmoTW:" + String(myBool4) + " hasTarget:" + String(myBool5) + " TW:" + String(myByte3), PRINT_TO_SCREEN);
        } break;
      case 41: { //Flight status part 3
          String controlLevelString = "No info";
          if(mySimpit.connectedToKSP2())
          {
            switch(myByte1)
            {
              case 0: controlLevelString = "NoControl"; break;
              case 1: controlLevelString = "NoCommNet"; break;
              case 2: controlLevelString = "FullCtrllHibernate"; break;
              case 3: controlLevelString = "FullControl"; break;
            }
          }
          else
          {
            switch(myByte1)
            {
              case 0: controlLevelString = "NoControl"; break;
              case 1: controlLevelString = "PartiallyUnmanned"; break;
              case 2: controlLevelString = "PartiallyManned"; break;
              case 3: controlLevelString = "FullControl"; break;
            }
          }
          mySimpit.printToKSP("CtrlLvl: " + controlLevelString, PRINT_TO_SCREEN);
        } break;
      case 42: { //Flight status part 4
          mySimpit.printToKSP("Situation:" + String((int)myByte2) + " Crew:" + String((int)myByte5) + "/" + String((int)myByte4), PRINT_TO_SCREEN);
        } break;
      case 43: { //Flight status part 5
          mySimpit.printToKSP("Signal:" + String((int)myByte6) + "% Stg:" + String((int)myByte7) + " Typ:" + String((int)myByte8), PRINT_TO_SCREEN);
        } break;
      case 44: { //Athmospheric Conditions
          mySimpit.printToKSP("Athmo:" + String(myBool1) + " Ox:" + String(myBool2) + " InAtm:" + String(myBool3), PRINT_TO_SCREEN);
        } break;
      case 45: { //Athmospheric Conditions
          mySimpit.printToKSP("Dens:" + String(myFloatStorage1, 2) + " Temp:" + String(myFloatStorage2, 0) + " Pres:" + String(myFloatStorage3, 0), PRINT_TO_SCREEN);
        } break;
      case 46: { //Vessel Name
          mySimpit.printToKSP("Vessel Name: '" + myString + "'", PRINT_TO_SCREEN);
        } break;
      case 47: { //Intake Air
          mySimpit.printToKSP("IntakeAir " + String(myResourceAvailableShip, 2) + "/" + String(myResourceTotalShip, 2), PRINT_TO_SCREEN);
        } break;
      case 48: { //Hydrogen
          mySimpit.printToKSP("Hydr " + String(myResourceAvailableShip, 2) + "/" + String(myResourceTotalShip, 2) + " " + String(myResourceAvailableStage, 2) + "/" + String(myResourceTotalStage, 2), PRINT_TO_SCREEN);
        } break;
      case 49: { //Uranium
          mySimpit.printToKSP("Uranium " + String(myResourceAvailableShip, 3) + "/" + String(myResourceTotalShip, 3), PRINT_TO_SCREEN);
        } break;
      case 50: { //Advanced Action Group State Info
          mySimpit.printToKSP(
            "B" + String((int)myAdvancedActionGroups[ADVANCED_BRAKES_ACTION]) + 
            " L" + String((int)myAdvancedActionGroups[ADVANCED_LIGHT_ACTION]) + 
            " G" + String((int)myAdvancedActionGroups[ADVANCED_GEAR_ACTION]) + 
            " S" + String((int)myAdvancedActionGroups[ADVANCED_SOLAR_ACTION]) + 
            " A" + String((int)myAdvancedActionGroups[ADVANCED_ABORT_ACTION]) + 
            " R" + String((int)myAdvancedActionGroups[ADVANCED_RADIATOR_ACTION]) + 
            " SA" + String((int)myAdvancedActionGroups[ADVANCED_SAS_ACTION]) + 
            " RC" + String((int)myAdvancedActionGroups[ADVANCED_RCS_ACTION]) + 
            " Sc" + String((int)myAdvancedActionGroups[ADVANCED_SCIENCE_ACTION]), PRINT_TO_SCREEN);
        } break;
      case 51: { //Advanced Custom Action Group State Info
          mySimpit.printToKSP(
            "1:"  + String((int)myAdvancedActionGroups[0]) + 
            " 2:" + String((int)myAdvancedActionGroups[1]) + 
            " 3:" + String((int)myAdvancedActionGroups[2]) + 
            " 4:" + String((int)myAdvancedActionGroups[3]) + 
            " 5:" + String((int)myAdvancedActionGroups[4]), PRINT_TO_SCREEN);
            //And so on for all 10 action groups
        } break;
      case 52: { //Intersect info first intersect
          mySimpit.printToKSP("Intsct1 d " + String(myFloatStorage1, 0) + " v " + String(myFloatStorage2, 0) + " t " + String(myTime1), PRINT_TO_SCREEN);
        } break;
      case 53: { //Intersect info second intersect
          mySimpit.printToKSP("Intsct2 d " + String(myFloatStorage3, 0) + " v " + String(myFloatStorage4, 0) + " t " + String(myTime2), PRINT_TO_SCREEN);
        } break;
      default: {
          //mySimpit.printToKSP(F("Unknown selectionIndex"), PRINT_TO_SCREEN);
        } break;
    }

    //Report if any packets got dropped
    if(mySimpit.packetDroppedNbr > 0)
    {
      mySimpit.printToKSP("Number of lost msg " + String(mySimpit.packetDroppedNbr));
      mySimpit.packetDroppedNbr = 0;
    }
  }

  // |----------------------------|
  // | Button Press, Reset values |
  // |----------------------------|
  //Check if button got pressed to switch between what's printed to KSP
  bool buttonState = digitalRead(CONTINUE_TEST_PIN);
  if (lastButtonState != buttonState) 
  {
    lastButtonState = buttonState;
    delay(10); //This is for debouncing the button. Note: delay is not a good option, as it pauses your program. Better to check against a timestamp like where stuff is printing to KSP
    if (buttonState == LOW)  //Button got pressed
    {
      selectionIndex++; //Count up the index
      if (selectionIndex >= NUMBER_OF_STEPS) selectionIndex = 0;  //Reset the index if maximum is reached

      //Reset some values we don't want to carry over between the different steps
      myResourceAvailableShip = -1.0f;
      myResourceTotalShip = -1.0f;
      myResourceAvailableStage = -1.0f;
      myResourceTotalStage = -1.0f;

      myFloatStorage1 = -1.0f;
      myFloatStorage2 = -1.0f;
      myFloatStorage3 = -1.0f;
      myFloatStorage4 = -1.0f;
      myFloatStorage5 = -1.0f;
      myFloatStorage6 = -1.0f;
      myFloatStorage7 = -1.0f;
      myFloatStorage8 = -1.0f;

      myAxis1 = 0;
      myAxis2 = 0;
      myAxis3 = 0;

      myByte1 = 0xFF;
      myByte2 = 0xFF;
      myByte3 = 0xFF;
      myByte4 = 0xFF;
      myByte5 = 0xFF;
      myByte6 = 0xFF;
      myByte7 = 0xFF;
      myByte8 = 0xFF;

      myString = "xxx";

      for(int i = 0; i < sizeof(myAdvancedActionGroups)/sizeof(myAdvancedActionGroups[0]); i++) myAdvancedActionGroups[i] = 0;

      //Request the channel we are currently looking at to get an updated value
      switch(selectionIndex)
      {
        case 0: 
          mySimpit.requestMessageOnChannel(LF_MESSAGE); 
          mySimpit.requestMessageOnChannel(LF_STAGE_MESSAGE); 
          break;
        case 1: 
          mySimpit.requestMessageOnChannel(OX_MESSAGE);
          mySimpit.requestMessageOnChannel(OX_STAGE_MESSAGE);
          break;
        case 2: 
          mySimpit.requestMessageOnChannel(SF_MESSAGE);
          mySimpit.requestMessageOnChannel(SF_STAGE_MESSAGE);
          break;
        case 3: 
          mySimpit.requestMessageOnChannel(XENON_GAS_MESSAGE);
          mySimpit.requestMessageOnChannel(XENON_GAS_STAGE_MESSAGE);
          break;
        case 4: 
          mySimpit.requestMessageOnChannel(MONO_MESSAGE);
          break;
        case 5: 
          mySimpit.requestMessageOnChannel(EVA_MESSAGE);
          break;
        case 6: 
          mySimpit.requestMessageOnChannel(ELECTRIC_MESSAGE);
          break;
        case 7: 
          mySimpit.requestMessageOnChannel(ORE_MESSAGE);
          break;
        case 8: 
          mySimpit.requestMessageOnChannel(AB_MESSAGE);
          mySimpit.requestMessageOnChannel(AB_STAGE_MESSAGE);
          break;
        case 9: 
          mySimpit.requestMessageOnChannel(ALTITUDE_MESSAGE);
          break;
        case 10: 
          mySimpit.requestMessageOnChannel(VELOCITY_MESSAGE);
          break;
        case 11: 
          mySimpit.requestMessageOnChannel(AIRSPEED_MESSAGE);
          break;
        case 12: 
          mySimpit.requestMessageOnChannel(APSIDES_MESSAGE);
          break;
        case 13: 
          mySimpit.requestMessageOnChannel(APSIDESTIME_MESSAGE);
          break;
        case 14: 
        case 15: 
          mySimpit.requestMessageOnChannel(MANEUVER_MESSAGE);
          break;
        case 16: 
          mySimpit.requestMessageOnChannel(SAS_MODE_INFO_MESSAGE);
          break;
        case 17: 
        case 18: 
        case 19: 
          mySimpit.requestMessageOnChannel(ORBIT_MESSAGE);
          break;
        case 20: 
        case 21: 
        case 22: 
          mySimpit.requestMessageOnChannel(ROTATION_DATA_MESSAGE);
          break;
        case 23: 
          mySimpit.requestMessageOnChannel(ROTATION_CMD_MESSAGE);
          break;
        case 24: 
          mySimpit.requestMessageOnChannel(TRANSLATION_CMD_MESSAGE);
          break;
        case 25: 
          mySimpit.requestMessageOnChannel(WHEEL_CMD_MESSAGE);
          break;
        case 26: 
          mySimpit.requestMessageOnChannel(THROTTLE_CMD_MESSAGE);
          break;
        case 27: 
          mySimpit.requestMessageOnChannel(ACTIONSTATUS_MESSAGE);
          break;
        case 28: 
          mySimpit.requestMessageOnChannel(DELTAV_MESSAGE);
          break;
        case 29: 
        case 30: 
          mySimpit.requestMessageOnChannel(DELTAVENV_MESSAGE);
          break;
        case 31: 
          mySimpit.requestMessageOnChannel(BURNTIME_MESSAGE);
          break;
        case 32: 
          mySimpit.requestMessageOnChannel(CAGSTATUS_MESSAGE);
          break;
        case 33: 
          mySimpit.requestMessageOnChannel(TEMP_LIMIT_MESSAGE);
          break;
        case 34: 
        case 35: 
        case 36: 
          mySimpit.requestMessageOnChannel(TARGETINFO_MESSAGE);
          break;
        case 37: 
          mySimpit.requestMessageOnChannel(SOI_MESSAGE);
          break;
        case 38: 
          mySimpit.requestMessageOnChannel(SCENE_CHANGE_MESSAGE);
          mySimpit.requestMessageOnChannel(VESSEL_CHANGE_MESSAGE);
          mySimpit.printToKSP(F("Only printing events now"), PRINT_TO_SCREEN);
          break;
        case 39: 
        case 40: 
        case 41: 
        case 42: 
        case 43: 
          mySimpit.requestMessageOnChannel(FLIGHT_STATUS_MESSAGE);
          break;
        case 44: 
        case 45: 
          mySimpit.requestMessageOnChannel(ATMO_CONDITIONS_MESSAGE);
          break;
        case 46: 
          mySimpit.requestMessageOnChannel(VESSEL_NAME_MESSAGE);
          break;
        case 47: 
          mySimpit.requestMessageOnChannel(INTAKE_AIR_MESSAGE); 
          break;
        case 48: 
          mySimpit.requestMessageOnChannel(HYDROGEN_MESSAGE); 
          mySimpit.requestMessageOnChannel(HYDROGEN_STAGE_MESSAGE); 
          break;
        case 49: 
          mySimpit.requestMessageOnChannel(URANIUM_MESSAGE); 
          break;
        case 50: 
          mySimpit.requestMessageOnChannel(ADVANCED_ACTIONSTATUS_MESSAGE); 
          break;
        case 51: 
          mySimpit.requestMessageOnChannel(ADVANCED_CAGSTATUS_MESSAGE); 
          break;
        case 52: 
        case 53: 
          mySimpit.requestMessageOnChannel(INTERSECTS_MESSAGE); 
          break;
      }
    }
  }

  // Check for new serial messages.
  mySimpit.update();
}

// |-----------------|
// | Message Handler |
// |-----------------|
void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch (messageType) {
    case ECHO_RESP_MESSAGE: {
        echoReceived = true;
      } break;
    case LF_MESSAGE: { // (Note: For this to work on KSP1 this needs the mod ARP to be installed)
        // If the message sizes don't match up (could be a bad message) 
        // or we are not at a selectionIndex of 0 (we are currently not interested in liquid fuel),
        // then we ignore the LF_MESSAGE
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 0)
        {
          resourceMessage liquidFuelMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalShip = liquidFuelMsg.total;
          myResourceAvailableShip = liquidFuelMsg.available;
        }
      } break;
    case LF_STAGE_MESSAGE: { // (Note: For this to work on KSP1 this needs the mod ARP to be installed)
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 0)
        {
          resourceMessage liquidFuelStageMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalStage = liquidFuelStageMsg.total;
          myResourceAvailableStage = liquidFuelStageMsg.available;
        }
      } break;
    case OX_MESSAGE: { // (Note: For this to work on KSP1 this needs the mod ARP to be installed)
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 1)
        {
          resourceMessage oxidizerMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalShip = oxidizerMsg.total;
          myResourceAvailableShip = oxidizerMsg.available;
        }
      } break;
    case OX_STAGE_MESSAGE: { // (Note: For this to work on KSP1 this needs the mod ARP to be installed)
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 1)
        {
          resourceMessage oxidizerStageMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalStage = oxidizerStageMsg.total;
          myResourceAvailableStage = oxidizerStageMsg.available;
        }
      } break;
    case SF_MESSAGE: { // (Note: For this to work on KSP1 this needs the mod ARP to be installed)
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 2)
        {
          resourceMessage solidFuelMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalShip = solidFuelMsg.total;
          myResourceAvailableShip = solidFuelMsg.available;
        }
      } break;
    case SF_STAGE_MESSAGE: { // (Note: For this to work on KSP1 this needs the mod ARP to be installed)
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 2)
        {
          resourceMessage solidFuelStageMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalStage = solidFuelStageMsg.total;
          myResourceAvailableStage = solidFuelStageMsg.available;
        }
      } break;
    case XENON_GAS_MESSAGE: { // (Note: For this to work on KSP1 this needs the mod ARP to be installed)
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 3)
        {
          resourceMessage xenonGasMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalShip = xenonGasMsg.total;
          myResourceAvailableShip = xenonGasMsg.available;
        }
      } break;
    case XENON_GAS_STAGE_MESSAGE: { // (Note: For this to work on KSP1 this needs the mod ARP to be installed)
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 3)
        {
          resourceMessage xenonGasStageMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalStage = xenonGasStageMsg.total;
          myResourceAvailableStage = xenonGasStageMsg.available;
        }
      } break;
    case MONO_MESSAGE: { // (Note: For this to work on KSP1 this needs the mod ARP to be installed)
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 4)
        {
          resourceMessage monopropellantMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalShip = monopropellantMsg.total;
          myResourceAvailableShip = monopropellantMsg.available;
        }
      } break;
    case EVA_MESSAGE: { // (Note: For this to work on KSP1 this needs the mod ARP to be installed)
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 5)
        {
          resourceMessage evaMonopropellantMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalShip = evaMonopropellantMsg.total;
          myResourceAvailableShip = evaMonopropellantMsg.available;
        }
      } break;



    case INTAKE_AIR_MESSAGE: { //Only works on KSP2
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 47)
        {
          resourceMessage resourceMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalShip = resourceMsg.total;
          myResourceAvailableShip = resourceMsg.available;
        }
      } break;
    case HYDROGEN_MESSAGE: { //Only works on KSP2
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 48)
        {
          resourceMessage resourceMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalShip = resourceMsg.total;
          myResourceAvailableShip = resourceMsg.available;
        }
      } break;
    case HYDROGEN_STAGE_MESSAGE: { //Only works on KSP2
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 48)
        {
          resourceMessage resourceMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalStage = resourceMsg.total;
          myResourceAvailableStage = resourceMsg.available;
        }
      } break;
    case URANIUM_MESSAGE: { //Only works on KSP2
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 49)
        {
          resourceMessage resourceMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalShip = resourceMsg.total;
          myResourceAvailableShip = resourceMsg.available;
        }
      } break;


    case ELECTRIC_MESSAGE: { // (Note: For this to work on KSP1 this needs the mod ARP to be installed)
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 6)
        {
          resourceMessage electricityMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalShip = electricityMsg.total;
          myResourceAvailableShip = electricityMsg.available;
        }
      } break;
    case ORE_MESSAGE: { // (Note: For this to work on KSP1 this needs the mod ARP to be installed)
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 7)
        {
          resourceMessage oreMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalShip = oreMsg.total;
          myResourceAvailableShip = oreMsg.available;
        }
      } break;
    case AB_MESSAGE: { // (Note: For this to work on KSP1 this needs the mod ARP to be installed)
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 8)
        {
          resourceMessage ablatorMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalShip = ablatorMsg.total;
          myResourceAvailableShip = ablatorMsg.available;
        }
      } break;
    case AB_STAGE_MESSAGE: { // (Note: For this to work on KSP1 this needs the mod ARP to be installed)
        if (msgSize == sizeof(resourceMessage) && selectionIndex == 8)
        {
          resourceMessage ablatorStageMsg = parseMessage<resourceMessage>(msg);
          myResourceTotalStage = ablatorStageMsg.total;
          myResourceAvailableStage = ablatorStageMsg.available;
        }
      } break;
    /* These messages are also available, but won't be further discussed in this example
    case TACLS_RESOURCE_MESSAGE: { // (Note: Only works on KSP1. This needs the mod ARP and TAC Life Support to be installed)
        if (msgSize == sizeof(TACLSRessourceMessage))
        {
          TACLSRessourceMessage taclsResourceMsg = parseMessage<TACLSRessourceMessage>(msg);
          //e.g. myCurrentFood = taclsResourceMsg.currentFood;
          //... and so on. For all available resources look at the TACLSRessourceMessage
        }
      } break;
    case TACLS_WASTE_MESSAGE: { // (Note: Only works on KSP1. This needs the mod ARP and TAC Life Support to be installed)
        if (msgSize == sizeof(TACLSWasteMessage))
        {
          TACLSWasteMessage taclsWasteMsg = parseMessage<TACLSWasteMessage>(msg);
          //e.g. myCurrentWaste = taclsWasteMsg.currentWaste;
          //... and so on. For all available resources look at the TACLSWasteMessage
        }
      } break;
    case CUSTOM_RESOURCE_1_MESSAGE: { // (Note: Only works on KSP1. This needs the mod ARP and Community Resource Pack to be installed)
        if (msgSize == sizeof(CustomResourceMessage))
        {
          CustomResourceMessage customResource1Msg = parseMessage<CustomResourceMessage>(msg);
          myResourceTotalShip = customResource1Msg.maxResource1;
          myResourceAvailableShip = customResource1Msg.currentResource1;
          //... and so on. There are 4 resources in the CustomResourceMessage
        }
      } break;
    case CUSTOM_RESOURCE_2_MESSAGE: { // (Note: Only works on KSP1. This needs the mod ARP and Community Resource Pack to be installed)
        if (msgSize == sizeof(CustomResourceMessage))
        {
          CustomResourceMessage customResource2Msg = parseMessage<CustomResourceMessage>(msg);
          myResourceTotalShip = customResource2Msg.maxResource1;
          myResourceAvailableShip = customResource2Msg.currentResource1;
          //... and so on. There are 4 resources in the CustomResourceMessage
        }
      } break;
    */


    case ALTITUDE_MESSAGE: {
        if (msgSize == sizeof(altitudeMessage) && selectionIndex == 9)
        {
          altitudeMessage altitudeMsg = parseMessage<altitudeMessage>(msg);
          myAltitudeSeaLevel = altitudeMsg.sealevel;
          myAltitudeSurface = altitudeMsg.surface;
          //If you only need one parameter, you can abbreviate like this:
          //myAltitudeSeaLevel = parseMessage<altitudeMessage>(msg).sealevel;
        }
      } break;
    case VELOCITY_MESSAGE: {
        if (msgSize == sizeof(velocityMessage) && selectionIndex == 10)
        {
          velocityMessage velocityMsg = parseMessage<velocityMessage>(msg);
          myFloatStorage1 = velocityMsg.orbital;
          myFloatStorage2 = velocityMsg.surface;
          myFloatStorage3 = velocityMsg.vertical;
        }
      } break;
    case AIRSPEED_MESSAGE: {
        if (msgSize == sizeof(airspeedMessage) && selectionIndex == 11)
        {
          airspeedMessage airspeedMsg = (airspeedMessage)parseMessage<airspeedMessage>(msg);
          myFloatStorage1 = airspeedMsg.IAS;
          myFloatStorage2 = airspeedMsg.mach;
          myFloatStorage3 = airspeedMsg.gForces;
        }
      } break;
    case APSIDES_MESSAGE: {
        if (msgSize == sizeof(apsidesMessage) && selectionIndex == 12)
        {
          apsidesMessage apsidesMsg = parseMessage<apsidesMessage>(msg);
          myFloatStorage1 = apsidesMsg.periapsis;
          myFloatStorage2 = apsidesMsg.apoapsis;
        }
      } break;
    case APSIDESTIME_MESSAGE: {
        if (msgSize == sizeof(apsidesTimeMessage) && selectionIndex == 13)
        {
          apsidesTimeMessage apsidesTimeMsg = parseMessage<apsidesTimeMessage>(msg);
          myTime1 = apsidesTimeMsg.apoapsis;
          myTime2 = apsidesTimeMsg.periapsis;
        }
      } break;
    case MANEUVER_MESSAGE: {
        if (msgSize == sizeof(maneuverMessage) && (selectionIndex == 14 || selectionIndex == 15))
        {
          maneuverMessage maneuverMsg = parseMessage<maneuverMessage>(msg);
          myFloatStorage1 = maneuverMsg.timeToNextManeuver;
          myFloatStorage2 = maneuverMsg.deltaVNextManeuver;
          myFloatStorage3 = maneuverMsg.durationNextManeuver;
          myFloatStorage4 = maneuverMsg.deltaVTotal;
          myFloatStorage5 = maneuverMsg.headingNextManeuver;
          myFloatStorage6 = maneuverMsg.pitchNextManeuver;
        }
      } break;
    case SAS_MODE_INFO_MESSAGE: {
        if (msgSize == sizeof(SASInfoMessage) && selectionIndex == 16)
        {
          SASInfoMessage sasInfoMsg = parseMessage<SASInfoMessage>(msg);
          myCurrentSASMode = sasInfoMsg.currentSASMode;
          mySASModeAvailability = sasInfoMsg.SASModeAvailability;
        }
      } break;
    case ORBIT_MESSAGE: {
        if (msgSize == sizeof(orbitInfoMessage) && (selectionIndex == 17 || selectionIndex == 18 || selectionIndex == 19))
        {
          orbitInfoMessage orbitInfoMsg = parseMessage<orbitInfoMessage>(msg);
          myFloatStorage1 = orbitInfoMsg.eccentricity;
          myFloatStorage2 = orbitInfoMsg.semiMajorAxis;
          myFloatStorage3 = orbitInfoMsg.inclination;
          myFloatStorage4 = orbitInfoMsg.longAscendingNode;
          myFloatStorage5 = orbitInfoMsg.argPeriapsis;
          myFloatStorage6 = orbitInfoMsg.trueAnomaly;
          myFloatStorage7 = orbitInfoMsg.meanAnomaly;
          myFloatStorage8 = orbitInfoMsg.period;
        }
      } break;
    case ROTATION_DATA_MESSAGE: {
        if (msgSize == sizeof(vesselPointingMessage) && (selectionIndex == 20 || selectionIndex == 21 || selectionIndex == 22))
        {
          vesselPointingMessage vesselPointingMsg = parseMessage<vesselPointingMessage>(msg);
          myFloatStorage1 = vesselPointingMsg.heading;
          myFloatStorage2 = vesselPointingMsg.pitch;
          myFloatStorage3 = vesselPointingMsg.roll;
          myFloatStorage4 = vesselPointingMsg.orbitalVelocityHeading;
          myFloatStorage5 = vesselPointingMsg.orbitalVelocityPitch;
          myFloatStorage6 = vesselPointingMsg.surfaceVelocityHeading;
          myFloatStorage7 = vesselPointingMsg.surfaceVelocityPitch;
        }
      } break;
    case ADVANCED_ACTIONSTATUS_MESSAGE: {
        if (msgSize == sizeof(advancedActionStatusMessage) && selectionIndex == 50)
        {
          advancedActionStatusMessage actionStatusMsg = parseMessage<advancedActionStatusMessage>(msg);
          for(int i = 0; i < 10; i++) //There are 9 action groups (stage, gear, ...), they are listed in the AdvancedActionGroupIndexes enum. Some are only available for KSP2
          {
            myAdvancedActionGroups[i] = actionStatusMsg.getActionStatus(i);
          }
        }
      } break;
    case ADVANCED_CAGSTATUS_MESSAGE: {
        if (msgSize == sizeof(advancedActionStatusMessage) && selectionIndex == 51)
        {
          advancedActionStatusMessage cagStatusMsg = parseMessage<advancedActionStatusMessage>(msg);
          for(int i = 0; i < 10; i++)
          {
            myAdvancedActionGroups[i] = cagStatusMsg.getActionStatus(i+1); //the myAdvancedActionGroups array starts counting at 0, but the custom action groups are addressed from 1 to 10. That's why there is (i+1) 
          }
        }
      } break;


    case ROTATION_CMD_MESSAGE: {
        if (msgSize == sizeof(rotationMessage) && selectionIndex == 23) 
        {
          rotationMessage rotationMsg = parseMessage<rotationMessage>(msg);
          myAxis1 = rotationMsg.pitch;
          myAxis2 = rotationMsg.roll;
          myAxis3 = rotationMsg.yaw;
        }
      } break;
    case TRANSLATION_CMD_MESSAGE: {
        if (msgSize == sizeof(translationMessage) && selectionIndex == 24) 
        {
          translationMessage translationMsg = parseMessage<translationMessage>(msg);
          myAxis1 = translationMsg.X;
          myAxis2 = translationMsg.Y;
          myAxis3 = translationMsg.Z;
        }
      } break;
    case WHEEL_CMD_MESSAGE: {
        if (msgSize == sizeof(wheelMessage) && selectionIndex == 25) 
        {
          wheelMessage wheelMsg = parseMessage<wheelMessage>(msg);
          myAxis1 = wheelMsg.steer;
          myAxis2 = wheelMsg.throttle;
        }
      } break;
    case THROTTLE_CMD_MESSAGE: {
        if (msgSize == sizeof(throttleMessage) && selectionIndex == 26) 
        {
          throttleMessage throttleMsg = parseMessage<throttleMessage>(msg);
          myAxis1 = throttleMsg.throttle;
        }
      } break;





    case ACTIONSTATUS_MESSAGE: {
        if (msgSize == 1 && selectionIndex == 27) 
        {
          myByte1 = msg[0];
        }
      } break;
    case DELTAV_MESSAGE: {
        if (msgSize == sizeof(deltaVMessage) && selectionIndex == 28) 
        {
          deltaVMessage deltaVMsg = parseMessage<deltaVMessage>(msg);
          myFloatStorage1 = deltaVMsg.stageDeltaV;
          myFloatStorage2 = deltaVMsg.totalDeltaV;
        }
      } break;
    case DELTAVENV_MESSAGE: {
        if (msgSize == sizeof(deltaVEnvMessage) && (selectionIndex == 29 || selectionIndex == 30))
        {
          deltaVEnvMessage deltaVEnvMsg = parseMessage<deltaVEnvMessage>(msg);
          myFloatStorage1 = deltaVEnvMsg.stageDeltaVASL;
          myFloatStorage2 = deltaVEnvMsg.totalDeltaVASL;
          myFloatStorage3 = deltaVEnvMsg.stageDeltaVVac;
          myFloatStorage4 = deltaVEnvMsg.totalDeltaVVac;
        }
      } break;
    case BURNTIME_MESSAGE: {
        if (msgSize == sizeof(burnTimeMessage) && selectionIndex == 31) 
        {
          burnTimeMessage burnTimeMsg = parseMessage<burnTimeMessage>(msg);
          myFloatStorage1 = burnTimeMsg.stageBurnTime;
          myFloatStorage2 = burnTimeMsg.totalBurnTime;
        }
      } break;
    case CAGSTATUS_MESSAGE: {
        if (msgSize == sizeof(cagStatusMessage) && selectionIndex == 32) 
        {
          cagStatusMessage cagStatusMsg = parseMessage<cagStatusMessage>(msg);
          for(int i = 0; i < 10; i++)
          {
            myCustomActionGroups[i] = cagStatusMsg.is_action_activated(i+1); //the myAdvancedActionGroups array starts counting at 0, but the custom action groups are addressed from 1 to 10. That's why there is (i+1) 
          }
        }
      } break;
    case TEMP_LIMIT_MESSAGE: {
        if (msgSize == sizeof(tempLimitMessage) && selectionIndex == 33) 
        {
          tempLimitMessage tempLimitMsg = parseMessage<tempLimitMessage>(msg);
          myByte1 = tempLimitMsg.tempLimitPercentage;
          myByte2 = tempLimitMsg.skinTempLimitPercentage;
        }
      } break;





    case TARGETINFO_MESSAGE: {
        if (msgSize == sizeof(targetMessage) && (selectionIndex == 34 || selectionIndex == 35 || selectionIndex == 36)) 
        {
          targetMessage targetMsg = parseMessage<targetMessage>(msg);
          myFloatStorage1 = targetMsg.distance;
          myFloatStorage2 = targetMsg.velocity;
          myFloatStorage3 = targetMsg.heading;
          myFloatStorage4 = targetMsg.pitch;
          myFloatStorage5 = targetMsg.velocityHeading;
          myFloatStorage6 = targetMsg.velocityPitch;
        }
      } break;
    case SOI_MESSAGE: {
        if (selectionIndex == 37) 
        {
          myString = (char *) msg;
          myString[msgSize] = '\0';
        }
    } break;
    case SCENE_CHANGE_MESSAGE: {
        if (selectionIndex == 38) 
        {
          myBool1 = true;
          myByte1 = msg[0];
        }
    } break;
    case FLIGHT_STATUS_MESSAGE: {
        if (msgSize == sizeof(flightStatusMessage) && (selectionIndex == 39 || selectionIndex == 40 || selectionIndex == 41 || selectionIndex == 42 || selectionIndex == 43)) 
        {
          flightStatusMessage flightStatusMsg = parseMessage<flightStatusMessage>(msg);
          myBool1 = flightStatusMsg.isInFlight();
          myBool2 = flightStatusMsg.isInEVA();
          myBool3 = flightStatusMsg.isRecoverable();
          myBool4 = flightStatusMsg.isInAtmoTW();
          myByte1 = flightStatusMsg.getControlLevel();
          myBool5 = flightStatusMsg.hasTarget();

          myByte2 = flightStatusMsg.vesselSituation;
          myByte3 = flightStatusMsg.currentTWIndex;
          myByte4 = flightStatusMsg.crewCapacity;
          myByte5 = flightStatusMsg.crewCount;
          myByte6 = flightStatusMsg.commNetSignalStrenghPercentage;
          myByte7 = flightStatusMsg.currentStage;
          myByte8 = flightStatusMsg.vesselType;
        }
      } break;
    case ATMO_CONDITIONS_MESSAGE: {
        if (msgSize == sizeof(atmoConditionsMessage) && (selectionIndex == 44 || selectionIndex == 45)) 
        {
          atmoConditionsMessage atmoConditionsMsg = parseMessage<atmoConditionsMessage>(msg);
          myBool1 = atmoConditionsMsg.hasAtmosphere();
          myBool2 = atmoConditionsMsg.hasOxygen();
          myBool3 = atmoConditionsMsg.isVesselInAtmosphere();
          myFloatStorage1 = atmoConditionsMsg.airDensity;
          myFloatStorage2 = atmoConditionsMsg.temperature;
          myFloatStorage3 = atmoConditionsMsg.pressure;
        }
      } break;
    case VESSEL_NAME_MESSAGE: {
        if (selectionIndex == 46) 
        {
          myString = (char *) msg;
          myString[msgSize] = '\0';
        }

      } break;
    case VESSEL_CHANGE_MESSAGE: {
        if (selectionIndex == 38) 
        {
          myBool2 = true;
        }
      } break;
    case INTERSECTS_MESSAGE: {
        if (msgSize == sizeof(intersectsMessage) && (selectionIndex == 52 || selectionIndex == 53)) 
        {
          intersectsMessage intersectsMsg = parseMessage<intersectsMessage>(msg);
          myFloatStorage1 = intersectsMsg.distanceAtIntersect1;
          myFloatStorage2 = intersectsMsg.velocityAtIntersect1;
          myTime1 = intersectsMsg.timeToIntersect1;
          myFloatStorage3 = intersectsMsg.distanceAtIntersect2;
          myFloatStorage4 = intersectsMsg.velocityAtIntersect2;
          myTime2 = intersectsMsg.timeToIntersect2;
        }
      } break;
    default: {

      } break;
  }
}