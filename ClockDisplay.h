/***************
* NTP Clock
* 
* The ClockDisplay class has the job of taking a request to display a digit and turning
* it into calls into the MAX6954 library to turn segments on and off.
*
* See the comments at the top of the .cpp file for details on the mapping involved.
*
* Brad Hines
* Feb 2020
*
*/


#ifndef CLOCK_DISPLAY_H
#define CLOCK_DISPLAY_H


#include "Max6954.h"


#define CLOCK_NUM_DIGITS (4)
#define DIGIT_NUM_SEGMENTS (7)

// How many digits this corresponds to on the MAX6954.  It's the same, but conceptually
// it's a different quantity, so we track it differently.
#define MAX6954_NUM_DIGITS (4)

// These are indices into the AnnunciatorToOutputNum array
typedef enum {
  CLOCK_ANNUNCIATOR_AM, 
  CLOCK_ANNUNCIATOR_PM, 
  CLOCK_ANNUNCIATOR_ALARM,
  CLOCK_ANNUNCIATOR_COLON,
  CLOCK_NUM_ANNUNCIATORS
} CLOCK_ANNUNCIATOR;


class tClockDisplay {
public:
  tClockDisplay(tMax6954 &Max);

  // These can be poked from outside.  When done poking, call UpdateDisplay()
  char Digit[4];   // This should be an ASCII value, not a number
  bool Annunciator[CLOCK_NUM_ANNUNCIATORS];

  void Update();

protected:
  void _LightUpSegment(int ClockDigit, int Segment);
  void _LightUpAnnunciator(int iAnnunciator);
  void _TurnOnMaxSegment(uint8_t OutputNum, uint8_t CathodeNum);;

  tMax6954 &_Max;

  // The segment maps we'll be outputting.  Making them a member just keeps from having to pass them around
  uint8_t _MaxDigits[MAX6954_NUM_DIGITS];  

  static const uint8_t _MaxDigitNumFromOutputNum[];
  static const uint8_t _MaxBitValueFromOutputNum[];

  static const uint8_t _ClockDigitToOutputNum[CLOCK_NUM_DIGITS][DIGIT_NUM_SEGMENTS];
  static const uint8_t _ClockDigitToCathodeNum[CLOCK_NUM_DIGITS][DIGIT_NUM_SEGMENTS];
  static const uint8_t _AnnunciatorToOutputNum[CLOCK_NUM_ANNUNCIATORS];
  static const uint8_t _AnnunciatorToCathodeNum[CLOCK_NUM_ANNUNCIATORS];
};

#endif   /* CLOCK_DISPLAY_H */
