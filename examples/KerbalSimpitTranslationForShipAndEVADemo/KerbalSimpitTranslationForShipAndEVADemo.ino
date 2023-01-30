/* KerbalSimpitTranslationForShipAndEVADemo
   By Theresa - tetopia.de
   
   A demonstration of how to use KerbalSimPit and the Keyboard library together to use a joystick for translation.
   It uses analog values while piloting a ship and keypresses while flying a kerbal during EVA.
   The interaction between Simpit and Keyboard.h is not officially suported but happens to work (for now).

   THIS DEMO ONLY WORKS WITH A MICROCONTROLLER THAT CAN EMULATE A KEYBOARD
   
   This demo works with the Arduino Leonardo, the Arduino Micro and the Arduino Due. All Arduinos using an ATmega32U4 should work.
   It does not work on an Arduino Uno, Arduino Nano or Arduino Mega. (At least not out of the box)

   Assume A0, A1, A2 are plugged on a potentiometer / joystick. Each potentiometer will be used
   to control one of the three translational axes of you rocket.
   See these links for basic descriptions of how to hook up a potentiometer:
     - https://www.arduino.cc/en/tutorial/potentiometer
     - https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v40/circuit-1b-potentiometer
   Assume digital pin 4 is plugged on a switch connected to ground on the other side
   Assume digital pin 5 is plugged on a pushbutton (normally open) connected to ground on the other side
*/

#include "KerbalSimpit.h"
#include <Keyboard.h>

//Use this pin for a switch to disable all keyboard input
#define PIN_SWITCH_ENABLE 4

//Use this pin for the button to toggle rcs
#define PIN_RCS_TOGGLE 5

//Use these pins for the analog inputs of the translational joystick
#define PIN_TRANSLATION_X A0
#define PIN_TRANSLATION_Y A1
#define PIN_TRANSLATION_Z A2

//Define values for the deadzones on the joystick
#define THC_X_DEADZONE_MIN (511 - 20)
#define THC_X_DEADZONE_MAX (511 + 20)
#define THC_Y_DEADZONE_MIN (511 - 20)
#define THC_Y_DEADZONE_MAX (511 + 20)
#define THC_Z_DEADZONE_MIN (511 - 20)
#define THC_Z_DEADZONE_MAX (511 + 20)

// Declare a KerbalSimpit object that will communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);

//A flag wether the current "vessel" is a normal vessel or a kerbal during EVA to change controls to eva mode
bool inEva; 

//A flag for if the rcs button was pressed previously
bool oldRcsButtonValue; 

void setup() 
{	
	Serial.begin(115200);
	
	pinMode(PIN_SWITCH_ENABLE, INPUT_PULLUP);
	pinMode(PIN_RCS_TOGGLE, INPUT_PULLUP);
	pinMode(PIN_TRANSLATION_X, INPUT);
	pinMode(PIN_TRANSLATION_Y, INPUT);
	pinMode(PIN_TRANSLATION_Z, INPUT);

	//Use this to test the inputs by looking on the Serial Monitor / Serial Plotter in the Arduino IDE (or any other serial console)
	//The Simpit will not start when using this to debug your inputs
	/*
	while(true)
	{
		Serial.print(analogRead(PIN_TRANSLATION_X));
		Serial.print(" ");
		Serial.print(analogRead(PIN_TRANSLATION_Y));
		Serial.print(" ");
		Serial.print(analogRead(PIN_TRANSLATION_Z));
		Serial.print(" ");
		Serial.print(digitalRead(PIN_SWITCH_ENABLE));
		Serial.print(" ");
		Serial.print(digitalRead(PIN_RCS_TOGGLE));
		Serial.println(" ");

		delay(100);
	}
	*/
	
	while (!mySimpit.init()) 
	{
		delay(100);
	}
	
	mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);
	mySimpit.inboundHandler(messageHandler);
	mySimpit.registerChannel(FLIGHT_STATUS_MESSAGE);
	
	Keyboard.begin();
}

void loop() 
{	
	//Don't do anything, when this switch is turned off (LOW-Active!) or not existing/open
	//This is to ensure that you can reprogram your controller if the Keyboard for some reason goes rampage
	if(digitalRead(PIN_SWITCH_ENABLE) == HIGH) 
	{
		Keyboard.releaseAll();
		//Leave the loop()
		return; 
	}

	//Test if the rcs button (LOW-Active) just got pressed down (cave: there is no debouncing)
	bool currentRcsButtonValue = digitalRead(PIN_RCS_TOGGLE);
	if(oldRcsButtonValue != currentRcsButtonValue && currentRcsButtonValue == LOW) 
	{
		//Press and release the R key during EVA
		if(inEva) Keyboard.write('r'); 
		//Use the RCS_ACTION when flying a normal ship
		else mySimpit.toggleAction(RCS_ACTION); 
	}
	oldRcsButtonValue = currentRcsButtonValue;

	if(inEva) //During EVA the analog translation and action groups are replaced by keypresses
	{
		if(analogRead(PIN_TRANSLATION_X) < THC_X_DEADZONE_MIN) Keyboard.press('d');
		else Keyboard.release('d');
		if(analogRead(PIN_TRANSLATION_X) > THC_X_DEADZONE_MAX) Keyboard.press('a');
		else Keyboard.release('a');

		if(analogRead(PIN_TRANSLATION_Y) < THC_Y_DEADZONE_MIN) Keyboard.press('s');
		else Keyboard.release('s');
		if(analogRead(PIN_TRANSLATION_Y) > THC_Y_DEADZONE_MAX) Keyboard.press('w');
		else Keyboard.release('w');

		if(analogRead(PIN_TRANSLATION_Z) < THC_Z_DEADZONE_MIN) Keyboard.press(KEY_LEFT_CTRL);
		else Keyboard.release(KEY_LEFT_CTRL);
		if(analogRead(PIN_TRANSLATION_Z) > THC_Z_DEADZONE_MAX) Keyboard.press(KEY_LEFT_SHIFT);
		else Keyboard.release(KEY_LEFT_SHIFT);
	}
	else //Normal ship translations
	{
		int analogInput = analogRead(PIN_TRANSLATION_X);
		int translationX = 0;
		//Map the analog input to it's according value between the min/max and the deadzone. Leave it 0 when in the deadzone
		if     (analogInput < THC_X_DEADZONE_MIN) translationX = map(analogInput, 0, THC_X_DEADZONE_MIN,      INT16_MIN, 0);
		else if(analogInput > THC_X_DEADZONE_MAX) translationX = map(analogInput, THC_X_DEADZONE_MAX, 1023,   0, INT16_MAX);
		
		analogInput = analogRead(PIN_TRANSLATION_Y);
		int translationY = 0;
		//Map the analog input to it's according value between the min/max and the deadzone. Leave it 0 when in the deadzone
		if     (analogInput < THC_Y_DEADZONE_MIN) translationY = map(analogInput, 0, THC_Y_DEADZONE_MIN,      INT16_MIN, 0);
		else if(analogInput > THC_Y_DEADZONE_MAX) translationY = map(analogInput, THC_Y_DEADZONE_MAX, 1023,   0, INT16_MAX);

		analogInput = analogRead(PIN_TRANSLATION_Z);
		int translationZ = 0;
		//Map the analog input to it's according value between the min/max and the deadzone. Leave it 0 when in the deadzone
		if     (analogInput < THC_Z_DEADZONE_MIN) translationZ = map(analogInput, 0, THC_Z_DEADZONE_MIN,      INT16_MIN, 0);
		else if(analogInput > THC_Z_DEADZONE_MAX) translationZ = map(analogInput, THC_Z_DEADZONE_MAX, 1023,   0, INT16_MAX);
		
		translationMessage translation_msg;
		translation_msg.setX(translationX);
		translation_msg.setY(translationY);
		translation_msg.setZ(translationZ);
		mySimpit.send(TRANSLATION_MESSAGE, translation_msg);
	}

	mySimpit.update();
}

void messageHandler(byte messageType, byte msg[], byte msgSize) {
	switch(messageType) 
	{
		case FLIGHT_STATUS_MESSAGE:
		{
			flightStatusMessage fsm = parseMessage<flightStatusMessage>(msg);
			inEva = fsm.isInEVA();
		} break;
	}
}
