// Library created for the Sparkfun Qwicc_Twist - C/C++

//--- #includes
#include "qwiic_twist_lib.h"

//--- variables
int16_t knob_status_old = 0;
int16_t knob_status_current = 0;

uint8_t btn_status = 0;


//--- COLOR Change methods
// Color set methods
void setColorRed(uint8_t redColor)
{
  writeToRegister(TWIST_RED, redColor);
}

void setColorBlue(uint8_t blueColor)
{
  writeToRegister(TWIST_BLUE, blueColor);
}

void setColorGreen(uint8_t greenColor)
{
  writeToRegister(TWIST_GREEN, greenColor);
}

// Color get methods
void getColorRed(uint8_t &color_data)
{
  readFromRegisterUInt8(TWIST_RED, color_data);
  Serial.println(color_data);
}

void getColorBlue(uint8_t &color_data)
{
  readFromRegisterUInt8(TWIST_BLUE, color_data);
  Serial.println(color_data);
}

void getColorGreen(uint8_t &color_data)
{
  readFromRegisterUInt8(TWIST_GREEN, color_data);
  Serial.println(color_data);
}

//--- BUTTON methods

int buttonState()
{
  readFromRegisterUInt8(TWIST_BTN_STATUS, btn_status); // read from the register

  btn_status = btn_status >> 1; // bit shift all values to the left so only the pressed event gets shown (value of 1)

  writeToRegister(TWIST_BTN_STATUS, 0); // reset the register by writing the value of 0 to it

  if (btn_status == 1)
  {
    return 1; // return true = 1
  }
  return 0; // return false = 0
}

//--- KNOB methods

int knobTwisted()
{
  readFromRegisterUInt16(TWIST_ENCODER_COUNT, knob_status_current); // read from the register

  if (knob_status_current > knob_status_old)
  {
    knob_status_old = knob_status_current; // switches the value of the current value to the old
    return 1; // knob is twisted right, return 1
  }
  else if (knob_status_current < knob_status_old)
  {
    knob_status_old = knob_status_current; // switches the value of the current value to the old
    return -1; // knob is twisted left, return -1
  }
  else
  {
    knob_status_old = knob_status_current; // switches the value of the current value to the old
    return 0; // knob is idle, return 0
  }

}

//--- REGISTER methods

void clearStatusRegister(uint8_t register_address)
{
  Wire.beginTransmission(QWIIC_TWIST_ADDRESS); // transmit to device #99 (0x63)
}

void writeToRegister(uint8_t addr, uint8_t val)
{
  Wire.beginTransmission(QWIIC_TWIST_ADDRESS); // transmit to device #99 (0x63)
  Wire.write(addr);
  Wire.write(val);
  Wire.endTransmission();
}

void readFromRegisterInt16(uint8_t addr, int16_t &data)
{
  Wire.beginTransmission(QWIIC_TWIST_ADDRESS); // transmit to device #99 (0x63)
  Wire.write(byte(addr));
  Wire.endTransmission();

  Wire.requestFrom( QWIIC_TWIST_ADDRESS, 1 );

  if (Wire.available() <= 1 )
  { // slave may send less than requested
    data = Wire.read(); // receive a byte as character
  }
}

void readFromRegisterUInt8(uint8_t addr, uint8_t &data)
{
  Wire.beginTransmission(QWIIC_TWIST_ADDRESS); // transmit to device #99 (0x63)
  Wire.write(byte(addr));
  Wire.endTransmission();

  Wire.requestFrom( QWIIC_TWIST_ADDRESS, 1 );

  if (Wire.available() <= 1 )
  { // slave may send less than requested
    data = Wire.read(); // receive a byte as character
  }
}

void readFromRegisterUInt16(uint8_t addr, int16_t &data)
{
  Wire.beginTransmission(QWIIC_TWIST_ADDRESS); // transmit to device #99 (0x63)
  Wire.write(addr);
  Wire.endTransmission();

  Wire.requestFrom( QWIIC_TWIST_ADDRESS, 2 ); // put in "2" to give a signal to read 2 bytes of data

  if (Wire.available() <= 2 )
  { // slave may send less than requested

    uint8_t dataLSB = Wire.read(); // receive the LSB of data
    uint8_t dataMSB = Wire.read(); // receive the MSB of data

    data = ((int16_t) dataMSB << 8 | dataLSB); //
  }
}

//--- PRINT methods

void printColorRegisterValues()
{
  uint8_t data_status = 0;
  Serial.print("RED value: ");
  getColorRed(data_status);
  Serial.print("BLUE value: ");
  getColorBlue(data_status);
  Serial.print("GREEN value: ");
  getColorGreen(data_status);
}

//--- FIX/INIT methods

void checkForRegisterCorrection()
{
  int16_t data = 0; // call by reference variable

  int count = sizeof(brightnessCheckArray) / sizeof(brightnessCheckArray[0]); // gives the amount of items in the list
  for (int i = 0; i < count; i++)
  {
    readFromRegisterInt16(brightnessCheckArray[i], data);
    Serial.print("Checking register: ");
    Serial.print(brightnessCheckArray[i], HEX);
    Serial.print("\nDATA: ");
    Serial.println(data);

    if (data != 0) // if register i has value not equal to 0, then proceeds to write a correction value of 0 to the register of i.
    {
      Serial.print("Value of register: ");
      Serial.println(brightnessCheckArray[i], HEX);
      Serial.println("Correcting value for start-up");
      writeToRegister(brightnessCheckArray[i], 0); // writes the correction value of 0, to register i
      readFromRegisterInt16(brightnessCheckArray[i], data); // proceeds to check if value has been corrected as second check
      if (data != 0) // if register i has value not equal to 0, prints a error to notify the user an error has occured which could'nt be fixed
      {
        Serial.println("ERROR value could not be corrected!");
        Serial.print("--Of register: ");
        Serial.println(brightnessCheckArray[i], HEX);
      }
      else // the value is corrected and the user is notified of the correction
      {
        Serial.println("Value corrected!");
        Serial.print("--Of register: ");
        Serial.println(brightnessCheckArray[i], HEX);
      }
    }
    else // value unchanged and not in need of correction, notifies user of status
    {
      Serial.println("Value is already correct, no change is needed");
    }
    Serial.println(); // extra line for the logging
  }
}

void resetKnobColorRegisters(bool setOverride)
{
  int amount = sizeof(colorRegisterArray) / sizeof(colorRegisterArray[0]);

  if (!setOverride)
  {
    Serial.println("Resetting colors...");
    for (int i = 0; i < amount; i++)
    {
      writeToRegister(colorRegisterArray[i], 0);
    }
  }
  else
  {
    Serial.println("Skipping color reset");
  }
  Serial.println("Color setup completed");
  printColorRegisterValues();
}
