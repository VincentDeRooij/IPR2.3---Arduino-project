// Library created for the Sparkfun Qwicc_Twist - C/C++
// https://www.sparkfun.com/products/15083 - link to the qwiic twist module

// #includes //
#include "Wire.h"
#include "Arduino.h"

// #defines //
#define QWIIC_TWIST_ADDRESS 0x63 // must be changed to the corresponding address for it to work (7-bits address)

// variables //
static uint8_t brightnessCheckArray[] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15};
static uint8_t colorRegisterArray[] = {0x0D, 0x0E, 0x0F};

// enum of the sparkfun qwiic twist knob registers
// all the register values can be found at: https://cdn.sparkfun.com/assets/learn_tutorials/8/4/6/Qwiic_Twist_Register_Map_-_Fixed_Endianness.pdf
enum twist_registers
{
  TWIST_RED = 0x0D, // Address of the red color register (with value of 1 to 255)
  TWIST_GREEN = 0x0E, // Address of the green color register (with value of 1 to 255)
  TWIST_BLUE = 0x0F, // Address of the blue color register (with value of 1 to 255)
  TWIST_BTN_STATUS = 0x01, // Address which keeps track of the button status (value goes from 0 to 3) see "further explanations" for extended information
  TWIST_ENCODER_COUNT = 0x05, // Address of the knob twisted count value of the encoder (normally a 16 bits value which goes from 1 to 32767)

  TWIST_RED_BRIGHTNESS_KNOB_CONTROL_LSB = 0x10, // register to set the brightness value changed when turning the knob for the red color (LSB) (value of -255 to 255)
  TWIST_RED_BRIGHTNESS_KNOB_CONTROL_MSB = 0x11, // register to set the brightness value changed when turning the knob for the red color (MSB) (value of -255 to 255)
  TWIST_GREEN_BRIGHTNESS_KNOB_CONTROL_LSB = 0x12, // register to set the brightness value changed when turning the knob for the green color (LSB) (value of -255 to 255)
  TWIST_GREEN_BRIGHTNESS_KNOB_CONTROL_MSB = 0x13, // register to set the brightness value changed when turning the knob for the green color (MSB) (value of -255 to 255)
  TWIST_BLUE_BRIGHTNESS_KNOB_CONTROL_LSB = 0x14, // register to set the brightness value changed when turning the knob for the blue color (LSB) (value of -255 to 255)
  TWIST_BLUE_BRIGHTNESS_KNOB_CONTROL_MSB = 0x15, // register to set the brightness value changed when turning the knob for the blue color (MSB) (value of -255 to 255)
};

// method prototypes //

// Color set methods --------------------------------------------------------------------------------------------
/**
    getColor[COLOR]()

    @param @value = the value which to set color to (max value of 1-255)
*/
void setColorRed(uint8_t value);
void setColorBlue(uint8_t value);
void setColorGreen(uint8_t value);

// Color get methods --------------------------------------------------------------------------------------------
/**
    getColor[COLOR]()

    @param color_data = call by reference variable
*/
void getColorRed(uint8_t &color_data);
void getColorBlue(uint8_t &color_data);
void getColorGreen(uint8_t &color_data);

// Button methods ------------------------------------------------------------------------------------------------
/**
    buttonPressed()

    @param reg_address = a register address from the Twist_Knob register values (see enum twist_registers)
*/
int buttonState();

// Knob methods ---------------------------------------------------------------------------------------------------

/**
    knobTwisted()

*/
int knobTwisted();

// Register methods ---------------------------------------------------------------------------------------------------

/**
    clearStatusRegister()

    //

    @param reg_address = a register address from the Twist_Knob register values (see enum twist_registers)
*/
void clearStatusRegister(uint8_t reg_address);

/**
    writeToRegister()

    writes a value to a given register

    @param addr = a register address from the Twist_Knob register values (see enum twist_registers)
    @param value = value you want to write to the register (max value of 1-255)
*/
void writeToRegister(uint8_t reg_address, uint8_t value); // method to write a value to a register

/**
   readFromRegisterInt16()

   Reads a given register for its value

   @param addr = a register address from the Twist_Knob register values (see enum twist_registers)
   @param reg_value = call by reference variable
*/
void readFromRegisterInt16(uint8_t addr, int16_t &reg_value);

/**
   readFromRegisterUInt8()

   Reads a given register for its value, used for the color registers

   @param addr = a register address from the Twist_Knob register values (see enum twist_registers)
   @param reg_value = call by reference variable
*/
void readFromRegisterUInt8(uint8_t addr, uint8_t &reg_value);

/**
   readFromRegisterUInt16()

   Reads a given register for its value, used for the twist knob registers

   @param addr = a register address from the Twist_Knob register values (see enum twist_registers)
   @param reg_value = call by reference variable
*/
void readFromRegisterUInt16(uint8_t addr, int16_t &data);

// Print methods -------------------------------------------------------------------------------------------------

/**
   printColorRegisterValues()

   Prints the RGB values of the color registers
   --Print method
*/
void printColorRegisterValues();

// Fix/init methods -------------------------------------------------------------------------------------------------

/**
   checkForRegisterCorrection()

   Method to check if certain register values of the Qwiic Twist Knob start with 0, if not writes the value 0 to them
   --Setup method
*/
void checkForRegisterCorrection();

/**
   resetKnobColorRegisters()

   Method to reset all color registers of the knob by setting the registers value to 0

    @param setOverride = a boolean value to override the option to reset the colors.
   --Setup method
*/
void resetKnobColorRegisters(bool setOverride);

// --------------------------------------------------------------------------------------------------- //

// further explanations //

// TWIST_BTN_STATUS == 0 means the encoder gives an interrupt trigger
// TWIST_BTN_STATUS == 1 means the button gives an interrupt trigger
// TWIST_BTN_STATUS == 2 means the button is clicked (so on button release when pressed)
// TWIST_BTN_STATUS == 3 means the button is pressed

// Also note the TWIST_BTN_STATUS register must be manually cleared (already happens inside of the button pressed method ( void buttonState(); ))
