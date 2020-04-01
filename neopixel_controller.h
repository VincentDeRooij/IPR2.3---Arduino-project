// controller library created for the NeoPixel (origin unknown), functions the same as the module:
// Adafruit NeoPixel Stick - 8 x 5050 RGBW LEDs -> https://www.adafruit.com/product/2868

// #includes //

#include "FastLED.h"

// #defines //

#define NUM_LEDS 8 // the number of leds, which are on the 8x5050 chip. In this case 8 NeoPixels, so 8 NUM_LEDS
#define DATA_PIN 7 // data pin on which to sent the data from

// variables //

static CRGB leds[NUM_LEDS]; // static to

// method prototypes

/**
    initializeLEDS()

    Initializes the leds, gets called by the main setup method
*/
void initializeLEDS();

/**
    clearAllLeds()

    clears all the leds (turns them "off"/to the color black)"
*/
void clearAllLeds();

/**
    playCycle_[X]()

    plays the corresponding led cycles, every method has a different pattern and id(X) (see method name)
    for a description see method on the *.cpp file of this header
*/
void playCycle_1();
void playCycle_2();
void playCycle_3();
void playCycle_4();
