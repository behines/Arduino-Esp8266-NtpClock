/***************
* NTP Clock
* 
* Brad Hines
* Feb 2020
*/

#include "MaximBitBangSpi.h"



/***************************************
* tMaximBitBangSpi constructor
*
* INPUTS:
*   The GPIO pins to use four each of the four SPI functions
*/

tMaximBitBangSpi::tMaximBitBangSpi(int ClkPin, int DinPin, int DoPin, int CsPin) :
  _ClkPin(ClkPin),
  _DinPin(DinPin),
  _DoPin (DoPin ),
  _CsPin (CsPin )
{
  pinMode(ClkPin, OUTPUT);
  pinMode(CsPin,  OUTPUT);
  pinMode(DoPin,  OUTPUT);
  pinMode(DinPin, INPUT);

  digitalWrite(CsPin, HIGH);

  // The device likes the clock to be low in its idle state
  digitalWrite(ClkPin, LOW);

  Serial.println(F("BitBang SPI for MAX6954 Online\n"));
}


/***************************************
* tMaximBitBangSpi::Write16 
*
* Follows these steps from p. 7 of the data sheet
* 1) Take CLK low.  (We leave CLK low as the idle state, no action required
* 2) Take CS low. This enables the internal 16-bit shift register.
* 3) Clock 16 bits of data into DIN, D15 first to D0 last, observing the setup and hold times.
*     Bit D15 is low, indicating a write command.
* 4) Take CS high (while CLK is still high after clocking in the last data bit).
* 5) Take CLK low.
*
* INPUTS:
*   
*/

uint16_t tMaximBitBangSpi::Write16(uint16_t x)
{
  int i;
  uint8_t BitIn;
  uint16_t response = 0;

  // CLK should already be low, but just in case it's not:
  digitalWrite(_ClkPin, LOW);
  MAXIM_LOGIC_DELAY;

  // Take CS low
  digitalWrite(_CsPin, LOW);
  MAXIM_LOGIC_DELAY;

  // Clock out bits 15..1
  for (i = 15; i >= 0; i--) {
    BitIn = _ClockOutBit((x >> i) & 0x01);
    response = response << 1 | BitIn;

    // If this was the last bit, take CS high before returning CLK to low
    if (i == 0) {
        digitalWrite(_CsPin, HIGH);
        MAXIM_LOGIC_DELAY;
    }

    digitalWrite(_ClkPin, LOW);
  }

  // Take the clock pin high again, so that we can take it low to start a new cycle
  MAXIM_LOGIC_DELAY;
  digitalWrite(_ClkPin, HIGH);

  MAXIM_LOGIC_DELAY;
  
  return response;
}


/***************************************
* tMaximBitBangSpi::_ClockOutBit
* 
* Puts the bit out on the output and then transitions CLK to high.
*
* Does not return CLK to low - that is the responsibility of the caller 
*
* RETURNS:
*   The bit in that was clocked out by the device for this cycle
*/

uint8_t tMaximBitBangSpi::_ClockOutBit(uint8_t bit)
{
  uint8_t BitIn;

  digitalWrite(_DoPin, bit);
  MAXIM_LOGIC_DELAY;

  digitalWrite(_ClkPin, HIGH);
  MAXIM_LOGIC_DELAY;

  BitIn = digitalRead(_DinPin);

  return BitIn;
}


/***************************************
* tMaximBitBangSpi::ReadRegister 
*
* Follows these steps from p. 8 of the data sheet
* 1) Take CLK low.  (We leave CLK low as the idle state, no action required
* 2) Take CS low. This enables the internal 16-bit shift register.
* 3) Clock 16 bits of data into DIN, D15 first to D0 last, observing the setup and hold times.
*     Bit D15 is high, indicating a write command.
*     D14 through D8 contain the address of the register to be read.
*     D7 through D0 are ignored.
* 4) Take CS high (while CLK is still high after clocking in the last data bit).
* 5) Take CLK low.
*
* INPUTS:
*   
*/

uint8_t tMaximBitBangSpi::ReadReg(uint8_t reg)
{
  uint16_t response;

  // The 0x8000 here indicates that this is a read operation
  Write16(reg << 8 | 0x8000);

  // Now get the response - send a NoOp command
  response = Write16(0);

  return response & 0xff;
}
