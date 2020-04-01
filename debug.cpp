#include "debug.h"
#include "qwiic_twist_lib.h"

//--- variables used for testing
int16_t test_knob_status_old = 0;
int16_t test_knob_status_current = 0;

uint8_t test_btn_status = 0;

//--- DEBUG methods

void testCheckForRegisterCorrection()
{
  Serial.println("__--DEBUG LOG--__ // TESTING -- checkForDefects method");

  int amountOfTimes = random(1, 3); // value to randomize the amount of times this method gets repeated
  int count = sizeof(brightnessCheckArray) / sizeof(brightnessCheckArray[0]); // gives the amount of items in the list

  for (int i = 0 ; i <= amountOfTimes; i++)
  {
    int arrayNumber = random(0, count); // value to randomize which register must be changed.
    uint8_t value = random(1, 255); // value to change the registers with (with min of 1 and max of 255)

    writeToRegister(brightnessCheckArray[arrayNumber], value); // writes the random value to the given register
  }
}

void testReadRegister()
{
  Serial.println("__--DEBUG LOG--__ // TESTING -- testReadRegister method");

  int16_t data_status = 0;
  //readFromRegister(0x01, data_status);
  //  Serial.print("MSB:");
  readFromRegisterInt16(TWIST_GREEN_BRIGHTNESS_KNOB_CONTROL_LSB, data_status);
  Serial.println(data_status);
  //readFromRegister(TWIST_GREEN_BRIGHTNESS_KNOB_CONTROL_MSB, data_status);
}

void testResetKnobColorRegisters()
{
  Serial.println("__--DEBUG LOG--__ // TESTING -- testResetKnobColorRegisters method");

  int amountOfTimes = random(1, 2); // value to randomize the amount of times this method gets repeated
  int count = sizeof(colorRegisterArray) / sizeof(colorRegisterArray[0]); // gives the amount of items in the list

  for (int i = 0 ; i <= amountOfTimes; i++)
  {
    int arrayNumber = random(0, count); // value to randomize which register must be changed.
    uint8_t value = random(1, 255); // value to change the registers with (with min of 1 and max of 255)

    writeToRegister(colorRegisterArray[arrayNumber], value); // writes the random value to the given register
    delay(1000);
  }
  printColorRegisterValues();
}

void testKnobTwisted()
{
  Serial.println("__--DEBUG LOG--__ // TESTING -- testKnobTwisted method");

  readFromRegisterUInt16(TWIST_ENCODER_COUNT, test_knob_status_current);

  if (test_knob_status_current > test_knob_status_old)
  {
    Serial.println("Knob twisted to the right");
  }
  else if (test_knob_status_current < test_knob_status_old)
  {
    Serial.println("Knob twisted to the left");
  }
  else
  {
    Serial.println("Knob is idle");
  }

  Serial.println("");
  Serial.print("DATA current: ");
  Serial.println(test_knob_status_current);
  Serial.print("DATA old: ");
  Serial.println(test_knob_status_old);

  delay(100); // delay of 100 ms to test

  test_knob_status_old = test_knob_status_current; // switches the value of the current value to the old
}

void testButtonState()
{
  Serial.println("__--DEBUG LOG--__ // TESTING -- testButtonState method");

  readFromRegisterUInt8(TWIST_BTN_STATUS, test_btn_status); // read from the register

  Serial.print("BTN STATE: ");
  Serial.println(test_btn_status);

  writeToRegister(TWIST_BTN_STATUS, 0); //
}

//--- UTIL methods
