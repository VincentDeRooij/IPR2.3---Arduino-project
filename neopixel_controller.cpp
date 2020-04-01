//--- #includes

#include "neopixel_controller.h"

// NOTE - FastLED.show(); must always be called upon changing the color value.
// This tells the controller to apply the said changes to the given led(s).

//--- METHODS

//--- INIT method
void initializeLEDS()
{
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // Setup the leds
}

void clearAllLeds() // clears the leds of their color
{
  for (int i = 0; i < 8; i++) // range up to the 8 different leds
  {
    leds[i] = CRGB::Black;
    FastLED.show();
  }
}

//--- PLAYCYCLES method
void playCycle_1() // RGB build-up, displays a certain color range of RED, GREEN and BLUE
{
  for (int i = 0; i < 8; i++) // range up to the 8 different leds and fill with first line of colors (RED focussed)
  {
    leds[i] = CRGB(255, 0 + (i * 20), 0 + (i * 20));
    FastLED.show();
    delay(75);
  }
  for (int i = 8; i > 0; i--) // fade old color with new line of colors (BLUE focussed)
  {
    leds[i] = CRGB(0 + (i * 20), 0 + (i * 20), 255);
    FastLED.show();
    delay(75);
  }
  for (int i = 0; i < 8; i++)  // range up to the 8 different leds and fill with colors (GREEN focussed)
  {
    leds[i] = CRGB(0 + (i * 20), 255, 0 + (i * 20));
    FastLED.show();
    delay(75);
  }
  for (int i = 8; i >= 0; i--) // fade out the colors to black (colors off)
  {
    leds[i] = CRGB(0, 0, 0);
    FastLED.show();
    delay(75);
  }
}

void playCycle_2() // police siren, plays a police siren light
{
  for (int i = 0; i < 4; i++) // first 4 parts of the leds color to red
  {
    leds[i] = CRGB::Red;
    FastLED.show();
  }
  delay(75);
  for (int i = 0; i < 4; i++) // first 4 parts turn off the red color
  {
    leds[i] = CRGB::Black;
    FastLED.show();
  }
  for (int i = 4; i < 8; i++)
  {
    leds[i] = CRGB::Blue; // second 4 parts of the leds color to blue
    FastLED.show();
  }
  delay(75);
  for (int i = 4; i < 8; i++) // second 4 parts turn off the blue color
  {
    leds[i] = CRGB::Black;
    FastLED.show();
  }
}

void playCycle_3() // foamy beach waves, "simulates" beach waves with added foam effect
{
  int randomBlueLvl = random(120, 255);
  int randomFoamLvl = random(4, 8);
  int randomPowerLvl = random(1, 4);
  int currentValue = 0;
  for (int i = 0; i < randomFoamLvl; i++)
  {
    currentValue = randomBlueLvl;
    leds[i] = CRGB(0, 0, currentValue);
    FastLED.show();
    currentValue += 20;
    delay(50);
  }
  for (int i = randomFoamLvl; i < 8; i++)
  {
    leds[i] = CRGB(100, 100, 100);
    FastLED.show();
    delay(60);
  }
  for (int i = 8; i > randomPowerLvl; i--)
  {
    leds[i] = CRGB(0, 0, 0);
    FastLED.show();
    delay(100);
  }
}

void playCycle_4() // Color palet generator, generates a line of 8 random colors
{
  for (int i = 8; i > 0; i--) // reverse fade out
  {
    leds[i] = CRGB::Black; // standard color value
    FastLED.show();
    delay(150);
  }

  int randomRedLvl = 0;
  int randomBlueLvl = 0;
  int randomGreenLvl = 0;
  for (int i = 0; i < 8; i++) // color build up
  {
    randomRedLvl = random(0, 255);
    randomGreenLvl = random(0, 255);
    randomBlueLvl = random(0, 255);

    leds[i] = CRGB(randomRedLvl, randomGreenLvl, randomBlueLvl);
    FastLED.show();
    delay(150);
  }
}
