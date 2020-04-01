// debug library, mostly for testing the functionality of certain methods

#include "Arduino.h"

// Debug method prototypes ----------------------------------------------------------------------------------------------

/**
   checkForDefectsTest()

   Method to change register values to randomized values, to test the method checkForDefects()
   --Debug method
*/
void testCheckForRegisterCorrection();

/**
   testReadRegister()

   Method to test if a register can be read
   --Debug method
*/
void testReadRegister();

/**
   testResetKnobColorRegisters()

   Method to test if colors registers are reset
   --Debug method
*/
void testResetKnobColorRegisters();

/**
   testKnobTwisted()

   Method to test if knob is twisted and which way
   --Debug method
*/
void testKnobTwisted();

/**
   testButtonState()

   Method to test if button is pressed
   --Debug method
*/
void testButtonState();

// Util methods ---------------------------------------------------------------------------------------------------
