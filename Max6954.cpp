/***************
* NTP Clock
* 
* Brad Hines
* Feb 2020
*/

#include "Max6954.h"

#include <SPI.h>


/***************************************
* tMax6954 constructor
*
* INPUTS:
*   
*/

tMax6954::tMax6954()
{
  _u8ConfigRegisterValue = 0;
}


/***************************************
* tMax6954::SetupSPI 
*
* Prepares the SPI bus for operation.
*
* INPUTS:
*   
*/

void tMax6954::_SetupSPI()
{
  SPI.begin();

  // Device wants MSB then LSB
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.setHwCs(true);  // Needed so that ESP8266 manages CS itself

  Serial.println(F("SPI for Mx 6954 Online\n"));
}


/***************************************
* tMax6954::Init 
*
* INPUTS:
*   
*/

void tMax6954::Init(uint8_t NumDigits)
{
  _SetupSPI();

  //Serial.println(F("Reading Port Configuration Register"))

  Serial.println(F("Leaving shutdown, clearing digits\n"));
  WriteConfig(MAX6954_CFG_GLOBAL_CLEAR_DIGIT_DATA);

  // Set brightness to 50%
  Serial.println(F("Set brightness to 50%\n"));
  SetBrightness(7);

  // Set number of digits to two.  Ths is the "Scan-Limit" register
  Serial.println(F("Set Scan Limit to 2 digits\n"));
  SetScanLimit(NumDigits);

  // Configure for two 16-segment unencoded digits, aka two 7-segment digit pairs, aka
  // four 7-segment monocolor digits.
  SetDigitType(MAX6954_DIGIT_TYPE_16_AND_16, MAX6954_DIGIT_TYPE_16_AND_16,
               MAX6954_DIGIT_TYPE_16_AND_16, MAX6954_DIGIT_TYPE_16_AND_16);
  SetDecodeMode(0); // No decoding for all 8 digit pairs
  
  // The recommended value of RSET (56K) also sets the peak current to 40mA, which makes the
  // segment current adjustable from 2.5mA to 37.5mA in 2.5mA steps.  Per the datasheet for my
  // device, at 50% duty cycle, We want to peak at 22.5 mA.
  // So for my device, we choose RSET = 40/22.5 * 56K = 100K

  Serial.print(F("Display Test..."));
  DisplayTest(true);
  delay(2000);
  DisplayTest(false);
  Serial.println(F("Done."));
}


/***************************************
* tMax6954::WriteCmd 
*
* INPUTS:
*   
*/
void tMax6954::WriteCmd(uint8_t Register, uint8_t Data);
{
  uin16_t cmd = Register;
  cmd         = cmd << 8 | Data;

  SPI.write16(cmd);
}


/***************************************
* tMax6954::DisplayTest 
*
* INPUTS:
*   
*/

void tMax6954::DisplayTest(bool bEnable)
{
  WriteCmd(MAX6954_REG_DisplayTest, bEnable);
}


/***************************************
* tMax6954::SetScanLimit 
*
* Pass in the number of digits in your design.  Must be between 1 and 8
*/

void tMax6954::SetScanLimit(uint8_t NumDigits)
{
  WriteCmd(MAX6954_REG_ScanLimit, NumDigits-1);
}


/***************************************
* tMax6954::WriteConfig();
*
* INPUTS:
*   
*/

void tMax6954::WriteConfig(uint8_t u8Flags)
{
  WriteCmd(MAX6954_REG_Configuration, u8Flags);
  _u8ConfigRegisterValue = u8Flags;
}


/***************************************
* tMax6954::SetBrightness 
*
* Goes from 0 => 1/16 to 14 => 15/16.  A value of 15 also gives 15/16.
*
* INPUT:
*   A value from 0 to 15
*/

void tMax6954::SetBrightness(uint8_t uiBrightness)
{
  WriteCmd(MAX6954_REG_GlobalIntensity, uiBrightness);
}


/***************************************
* tMax6954::SetDecodeMode 
*
* Goes from 0 => 1/16 to 14 => 15/16.  A value of 15 also gives 15/16.
*
* INPUT:
*   A value from 0 to 15
*/

void tMax6954::SetDecodeMode(uint8_t uiBrightness)
{
  WriteCmd(MAX6954_REG_DecodeMode, uiBrightness);
}


/***************************************
* tMax6954::NoOp 
*
* Does nothing, but is used as part of the register read process to cause 
* bits to shift out of the device.
*
* INPUTS:
*   None.
*
* SIDE EFFECTS:
*   Causes bits to be shifted towards DOUT.
*
*/

void tMax6954::NoOp()
{
  WriteCmd(MAX6954_REG_NoOp, 0);
}


/***************************************
* tMax6954::SetDigitTypes 
*
* This is a bit complex.  The digit-type register controls the digit types for four
* slots.  Slot 1 corresponds to Common-Cathodes 0 and 1, Slot 2 corresponds to 2 and 3,
* and so on.
*
* The digit types for the four slots are each configured by two bits of the byte you send to the 
* regiser, with the 8 bits mapping into the byte like this:
*   S4_1 S4_0 S3_1 S3_0 S2_1 S2_0
*
* For my application, I want to drive 16 segments using CC0 and CC1 according to my own plan.
* That puts us in Configuration '00', the gray column of Table 37 in the datasheet.
*
* For my display, I only need 14 of the 16 'segments', so I use only O5-O18 of the table.
*
* INPUTS:
*   The arguments below are the 2-bit digit type codes for each pair of cathodes, CC7/CC6,
*   CC5/CC4, CC3/CC2, CC1/CC0.
*
* SIDE EFFECTS:
*   Causes bits to be shifted towards DOUT.
*
*/

void tMax6954::SetDigitTypes(uint8_t DigitTypes76, uint8_t DigitTypes54,
                             uint8_t DigitTypes32, uint8_t DigitTypes10)
{
  uint8_t DigitTypeCode = DigitTypes76 << 6 |
                          DigitTypes54 << 4 |
                          DigitTypes32 << 2 |
                          DigitTypes10;
  WriteCmd(MAX6954_REG_DigitType,);
}


/***************************************
* tMax6954::SetDecodeMode
*
* The decoding mode for digits can be set in pairs.  There at 8 digit pairs, numbered 0 
* through 7.  Each bit of the decode mode register controls a pair of digits.  Bit 0 
* controls digit pair 0, bit 1 controls digit pair 1, and so on.
*
* The decode mode can be either "no decode" (bit value 0), or "hexadecimal decode" (bit 
* value 1).
*
* So if you wish to decode digit pair N, you pass in 1<<N.  So this expression would 
* call for decoding of digit pairs 3 and 5 (meaning digits 6,7,10, and 11, where the first
* digit is digit number 0): u8DecodeFlags = 1<<3 | 1<<5;
*
* INPUTS:
*   u8DecodeFlags, as described above.
*/

void tMax6954::SetDecodeMode(uint8_t u8DecodeFlags)
{
  WriteCmd(MAX6954_REG_DecodeMode, u8DecodeFlags);
}


/***************************************
* tMax6954::WriteDigit
* 
*
* INPUTS:
*   Digit Number (0-15)
*   Which Planes (Plane 0, Plane 1, or both).  Must be the OR of the planes you want to
*     write, MAX6954_REG_PLANE0 or MAX6954_REG_PLANE1 or both
*   8-bit value  (Not all bits meaningful if decoding is in use)
*/

void tMax6954::WriteDigit(uint8_t u8Digit, uint8_t u8Planes uint8_t u8Value)
{
  // Compute the register number
  uint8_t RegNum = u8Planes + u8Digit;

  WriteCmd(RegNum, u8Value);
}


