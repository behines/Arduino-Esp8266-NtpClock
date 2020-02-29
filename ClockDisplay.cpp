/***************
* NTP Clock
* 
*
* Brad Hines
* Feb 2020
*/

#include "ClockDisplay.h"
#include "SevenSegment.h"

/*
* The particular clock we are making consists of four digits, but they are organized into two
* common-cathode groups, with each cathode wired to segments across all four digits.
*
* The way we deal with this is to tell the MAX6954 that we have two 16-segment "digits", or more
* specifically, that we have two "7-segment digit pairs", with each pair comprising segments a-g
* and a decimal point.
*
* This bit of fiction means that there is some translation required between our clock's actual 
* segment ID's and the names that they end up having as far as tha MAX6954 is concerned.
*
*
* Now, for the MAX6954, we'll be illuminating segments attached to outputs O5-O18.  Referring 
* to Table 4 of the datasheet, we see that these correspond to the following segments for each
* "digit pair":
*    O5  O6  O7   O8  O9  O10 O11 O12 O13 O14 O15 O16 O17 O18
*    1c  1d  1dp  1e  1f  1g  2a  2b  2c  2d  2e  2f  2g  2dp
*
* Next, referring to Table 10 of the datasheet, we see that each of these segments has a bit
* value in the corresponding Digit Register.
*    D7 D6 D5 D4 D3 D2 D1 D0
*    dp  a  b  c  d  e  f  g
*
* Given this, for each of the O(n) outputs, we can assign a Digit Number and a Bit Value
*
*   Output           O5  O6  O7   O8  O9  O10 O11 O12 O13 O14 O15 O16 O17 O18
*   Digit             1   1   1    1   1   1   2   2   2   2   2   2   2   2
*   Max segment       c   d  dp    e   f   g   a   b   c   d   e   f   g  dp
*   Bit Value (hex)  10  08  80   04  02  01  40  20  10  08  04  02  01  80
*
* This pattern repeats twice, once for each of our pairs of digits.
*
* This table forms the first of two key mappings that this class maintains.  We keep two arrays that
* are indexed from 0 to 18 (with 0 through 4 being meaningless), that tell the digit number and the 
* bit value for each O(n). 
*/

const uint8_t tClockDisplay::_MaxDigitNumFromOutputNum[19] =
   { 0,0,0,0,0,     1,   1,   1,   1,   1,   1,      3,   3,   3,   3,   3,   3,   3,   3 };
const uint8_t tClockDisplay::_MaxBitValueFromOutputNum[19] = 
   { 0,0,0,0,0,  0x10,0x08,0x80,0x04,0x02,0x01,   0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x80 };


/*
* We then keep a second table that keeps the mappings from segments A-G of our 4 digits to their 
* corresponding O(n) outputs.  This is indexed as Digit 0-3 and then segment a-g.
*
* We also have to note which cathode the digit is attached to.  This will tell us whether the
* segment maps to MAX6954 digits 0 and 1 (cathode 1) and 2 and 3 (cathode 2).
*
* Note that digit 1 only has segments b and c.  We map the others to output 0. 
*/

const uint8_t tClockDisplay::_ClockDigitToOutputNum[CLOCK_NUM_DIGITS][DIGIT_NUM_SEGMENTS] = {
  // a   b   c   d   e   f   g
  {  0,  7,  6,  0,  0,  0,  0 },
  { 10,  5,  9,  9,  6, 10,  5 }, 
  { 11, 12, 13, 13, 14, 11, 12 },
  { 17, 15, 16, 16, 14, 17, 15 }
};
const uint8_t tClockDisplay::_ClockDigitToCathodeNum[CLOCK_NUM_DIGITS][DIGIT_NUM_SEGMENTS] = {
  {  0,  2,  2,  0,  0,  0,  0 },
  {  2,  2,  2,  1,  1,  1,  1 },
  {  1,  1,  1,  2,  2,  2,  2 },
  {  2,  2,  2,  1,  1,  1,  1 }
};


/*
* And lastly, we keep values for the outputs associated with the special annunciators
*
*/

const uint8_t tClockDisplay::_AnnunciatorToOutputNum[CLOCK_NUM_ANNUNCIATORS]  = { 8, 7, 8, 18 };
const uint8_t tClockDisplay::_AnnunciatorToCathodeNum[CLOCK_NUM_ANNUNCIATORS] = { 2, 1, 1,  1 };


/***************************************
* tClockDisplay constructor
*
* INPUTS:
*   
*/

tClockDisplay::tClockDisplay(tMax6954 &Max) :
  _Max(Max)
{
  int i;
  
  for (i=0; i<CLOCK_NUM_DIGITS; i++)
    Digit[i] = ' ';
  
    for (i=0; i<CLOCK_NUM_ANNUNCIATORS; i++)
    Annunciator[i] = 0;
}


/***************************************
* tClockDisplay::Update
*
* Computes the new segment patterns from the Digit and Annunciator states, then
* updates the Max LED controller
*/

void tClockDisplay::Update()
{
  const tSegmentPattern *pSegs;
  uint8_t i, iWhichDigit;

  // Zero out the output digits
  for (i=0; i<MAX6954_NUM_DIGITS; i++)  _MaxDigits[i] = 0;

  for (i=0; i<CLOCK_NUM_DIGITS; i++) {
   
    pSegs = Encode7Segments(Digit[i]);
    
    if (pSegs == NULL) {
      Serial.print(F("tClockDisplay::Update: Unknown character: "));
      Serial.print((int) Digit[i]);
      Serial.print(" for digit ");
      Serial.println(i);
      continue;
    }

    if (pSegs->a)  _LightUpSegment(i,0);
    if (pSegs->b)  _LightUpSegment(i,1);
    if (pSegs->c)  _LightUpSegment(i,2);
    if (pSegs->d)  _LightUpSegment(i,3);
    if (pSegs->e)  _LightUpSegment(i,4);
    if (pSegs->f)  _LightUpSegment(i,5);
    if (pSegs->g)  _LightUpSegment(i,6);
  }

  // Do the annunciators
  for (i=0; i<CLOCK_NUM_ANNUNCIATORS; i++) {
    if (Annunciator[i])  _LightUpAnnunciator(i);
  }

  // Output the digits
  for (i=0; i<MAX6954_NUM_DIGITS; i++)  {
     iWhichDigit = (i==0) ? 0 : (i==1) ? 1 : (i==2) ? 8 : 9;
    _Max.WriteDigit(iWhichDigit, MAX6954_REG_PLANE0 | MAX6954_REG_PLANE1, _MaxDigits[i]);
  }
}



/***************************************
* tClockDisplay::LightUpSegment
*
*/

void tClockDisplay::_LightUpSegment(int ClockDigit, int Segment)
{
  uint8_t OutputNum  = _ClockDigitToOutputNum[ClockDigit][Segment];
  uint8_t CathodeNum = _ClockDigitToCathodeNum[ClockDigit][Segment];

  // If it's a non-existent segment (e.g. for a '0' in the first digit), bail out
  if (OutputNum == 0  ||  CathodeNum == 0)  return;

  Serial.print("LightUpSeg: Digit ");
  Serial.print(ClockDigit);
  Serial.print(" Seg ");
  Serial.print(Segment);
  Serial.print("->  Output ");
  Serial.print(OutputNum);
  Serial.print("  Cathode ");
  Serial.println(CathodeNum);

  _TurnOnMaxSegment(OutputNum, CathodeNum);
}


/***************************************
* tClockDisplay::LightUpAnnunciator
* 
*/

void tClockDisplay::_LightUpAnnunciator(int iAnnunciator)
{
  uint8_t OutputNum  = _AnnunciatorToOutputNum[iAnnunciator];
  uint8_t CathodeNum = _AnnunciatorToCathodeNum[iAnnunciator];

  _TurnOnMaxSegment(OutputNum, CathodeNum);
}


/***************************************
* tClockDisplay::TurnOnMaxSegment
* 
* Givan a Max output number and cathode number, sets the appropriate bit
* in the MaxDigits register values that we are building up.
*/

void tClockDisplay::_TurnOnMaxSegment(uint8_t OutputNum, uint8_t CathodeNum)
{
  // Figure out the Max digit number.  Cathode 1 is Max digits 0 and 1.
  // Cathode 2 is Max digits 2 and 3.
  uint8_t MaxDigitNum = _MaxDigitNumFromOutputNum[OutputNum] - 1;
  if (CathodeNum == 2) MaxDigitNum += 1;

  // Turn on the bit
  _MaxDigits[MaxDigitNum] |= _MaxBitValueFromOutputNum[OutputNum];
}
