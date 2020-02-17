/***************
* NTP Clock
* 
* Brad Hines
* Feb 2020
*/


#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include <Arduino.h>

// Define a bitfield structure to hold the segment patterns for each digit
struct tSegmentPattern {
  bool a : 1;
  bool b : 1;
  bool c : 1;
  bool d : 1;
  bool e : 1;
  bool f : 1;
  bool g : 1;
  bool dot : 1;
};

const tSegmentPattern *Encode7Segments(char cAscii);

void PrintSevenSegmentDigit(char cAscii);
void PrintAllSevenSegmentDigits();


#endif   /* SEVEN_SEGMENT_H */
