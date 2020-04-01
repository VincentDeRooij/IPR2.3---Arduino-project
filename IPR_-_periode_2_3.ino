/** -- Project Version 1.0
   NOTE to examiner: Alle code hier is door mij gemaakt, behalve de bibliotheek van FastLED(Deze gebruik ik om 8 Neopixel balkje aan te sturen) en de included Arduino Wire lib (voor i2c, deze dient voor de draaiknop)
   Deze staat in de neopixel_controller.h (die wel weer door mij is gemaakt).

   Debug.h is een bibliotheek om het testen/debuggen appart te houden

   De code is waar dit mogelijk is in C geschreven, maar ik heb helaas ook C++ code moeten gebruiken/maken
   Dit moest vooral omdat de Wire.h bibliotheek niet wilden compilen zonder dat deze op C++ staat.

   Code is zo veel mogelijk voorzien van commentaar, de uitleg van het commentaar is vooral te vinden in de header files.
*/

// #includes
#include "neopixel_controller.h" // pre-created library to utilize the NeoPixel LED strip (8 x 5050)
#include "qwiic_twist_lib.h" // self created library has Wire.h and Arduino.h included, responsible for the sparkfun qwiic_twist knob

// #defines
#define UPDATE_DELAY 20 // a set delay for the dataUpdate method

// method prototypes
void dataUpdate(void); // for updating the current states of the knob and button

// variables
int knob_state = 0; // returns 1 if knob is turned right, returns -1 if knob is turned left, returns 0 if knob is idle
int currentState = 0; // keeps track of the current menu state value

void setup() // setup method, all variables and other things which are required to be setup are put inside of this method
{
  // Setup Serial communication (used in printing and debugging) if nothing gets printed, Serial wasn't established correctly and the program won't function
  Serial.begin(115200);
  Serial.println("Setting up SERIAL....");
  while (!Serial); // wait for serial monitor to be completely initialized
  Serial.println("Setting up SERIAL complete");

  // Setup Wire i2c communication
  Serial.println("Setting up WIRE....");
  Wire.begin();
  Serial.println("Setting up WIRE complete");

  // Setup/check the registers of the knob
  Serial.println("Checking Knob registers for errors");
  checkForRegisterCorrection(); // this method is called once to check all the registers which bind setting the color to the rotary knob
  Serial.println("Registers check complete");

  // Reset the color registers of the knob
  Serial.println("Resetting registers color on the knob");
  resetKnobColorRegisters(false); // reset all the color registers
  Serial.println("Resetting complete!");
  Serial.println("Check complete!");

  // Setup the NeoPixels
  Serial.println("Setting up NeoPixels....");
  initializeLEDS(); // init method to initialize the NeoPixels
  FastLED.setBrightness(10); // setting the brightness to acceptable levels (Higher than 25 is not recommended)
  Serial.println("Setting up NeoPixels complete");

  // Execute main program
  Serial.println(); // blank line to divide the logging
  Serial.println("Setup complete! executing main program");
}

// To play the different states - press the button a couple times
// To exit hold the button and release (because of no interrupts handling this was solved this way)
//
void loop()
{
  dataUpdate(); // Update needed when user is outside of the switch case and needs the constant updates
  Serial.print("Current state: ");
  Serial.println(currentState); // current menu value of the turn knob

  if (buttonState() == 1) // button state of 1 (=Button pressed down) to enter the state menu
  {
    switch (currentState) // this is the switch case which swaps between the different RGB methods
    {
      case 0:

        break;

      case 1:
        Serial.println("Playing RGB build-up");
        while (buttonState() == 0)
        {
          playCycle_1();
          dataUpdate();
        }
        break;
      case 2:
        Serial.println("Playing police siren");
        while (buttonState() == 0)
        {
          playCycle_2();
          dataUpdate();
        }
        break;
      case 3:
        Serial.println("Playing foamy beach waves ");
        while (buttonState() == 0)
        {
          playCycle_3();
          dataUpdate();
        }
        break;
      case 4:
        Serial.println("Playing color palet generator");
        while (buttonState() == 0)
        {
          playCycle_4();
          dataUpdate();
        }
        break;

      case 5:
        if (buttonState() == 1)
        {
          clearAllLeds();
          Serial.println("Clears all colors");
        }
        break;

      default:
        Serial.println("This mode hasn't been implemented yet!"); // fail safe it the dataUpdate fails to lock the variable in place to 4
    }
  }
}

/**
    void dataUpdate()

    this method is used to periodically update the data gathered from the qwiic twist, mainly the turn knob,
    The button state is updated inside the while loops of the different states.

*/
void dataUpdate(void)
{
  knob_state = knobTwisted(); // returns either 1(right), -1(left) or 0(idle) depending on the knob direction return value

  currentState += knob_state; // updates the menu value of the currentState
  if (currentState > 5) // locks the currentState in place, when the currentState gets higher than 7
  {
    currentState = 5;
  }
  else if (currentState < 0) // locks the currentState in place, when the currentState gets lower or equal to 1
  {
    currentState = 0;
  }
}
