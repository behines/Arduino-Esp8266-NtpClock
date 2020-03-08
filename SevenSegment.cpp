/***************
* NTP Clock
* 
* Brad Hines
* Feb 2020
*/

#include "SevenSegment.h"


// ASCII codes
// 48-57  are the digits 0-9
// 65-90  are the letters A-Z
// 97-122 are the letters a-z.  Subtract 32 to get the capitals

// LED Segments go clockwise from top:
//
//     A
//   F   B
//     G
//   E   C
//     D

// This array starts at ASCII 48 for 0

const tSegmentPattern SegmentPatterns[] = {
//  a  b  c  d  e  f  g  dot
  { 1, 1, 1, 1, 1, 1, 0, 0 },  // 0
  { 0, 1, 1, 0, 0, 0, 0, 0 },  // 1
  { 1, 1, 0, 1, 1, 0, 1, 0 },  // 2
  { 1, 1, 1, 1, 0, 0, 1, 0 },  // 3
  { 0, 1, 1, 0, 0, 1, 1, 0 },  // 4
  { 1, 0, 1, 1, 0, 1, 1, 0 },  // 5
  { 1, 0, 1, 1, 1, 1, 1, 0 },  // 6
  { 1, 1, 1, 0, 0, 0, 0, 0 },  // 7
  { 1, 1, 1, 1, 1, 1, 1, 0 },  // 8
  { 1, 1, 1, 1, 0, 1, 1, 0 },  // 9
  { 0, 0, 0, 1, 0, 0, 0, 0 },  // ascii 58 : (shows as underscore)
  { 0, 0, 0, 1, 0, 0, 0, 0 },  // ascii 59 ; (underscore)
  { 0, 0, 0, 1, 1, 0, 0, 0 },  // ascii 60 < 
  { 0, 0, 0, 1, 0, 0, 1, 0 },  // ascii 61 =
  { 0, 0, 1, 1, 0, 0, 0, 0 },  // ascii 62 >
  { 1, 1, 0, 0, 1, 0, 1, 0 },  // ascii 63 ?
  { 1, 0, 1, 1, 1, 1, 1, 0 },  // ascii 64 @
  { 1, 1, 1, 0, 1, 1, 1, 0 },  // A
  { 0, 0, 1, 1, 1, 1, 1, 0 },  // b
  { 0, 0, 0, 1, 1, 0, 1, 0 },  // c
  { 0, 1, 1, 1, 1, 0, 1, 0 },  // d
  { 1, 0, 0, 1, 1, 1, 1, 0 },  // E
  { 1, 0, 0, 0, 1, 1, 1, 0 },  // F
  { 1, 1, 1, 1, 0, 1, 1, 0 },  // g (same as 9)
  { 0, 0, 1, 0, 1, 1, 1, 0 },  // h
  { 0, 0, 1, 0, 0, 0, 0, 0 },  // i
  { 0, 1, 1, 1, 0, 0, 0, 0 },  // J
  { 0, 0, 0, 0, 1, 1, 1, 0 },  // k
  { 0, 0, 0, 1, 1, 1, 0, 0 },  // L
  { 1, 1, 1, 0, 1, 1, 0, 0 },  // M (A but no middle)
  { 0, 0, 1, 0, 1, 0, 1, 0 },  // n
  { 0, 0, 1, 1, 1, 0, 1, 0 },  // o
  { 1, 1, 0, 0, 1, 1, 1, 0 },  // P
  { 1, 1, 1, 0, 0, 1, 1, 0 },  // q (same as 9 without base)
  { 0, 0, 0, 0, 1, 0, 1, 0 },  // r
  { 1, 0, 1, 1, 0, 1, 1, 0 },  // S
  { 0, 0, 0, 1, 1, 1, 1, 0 },  // t
  { 0, 0, 1, 1, 1, 0, 0, 0 },  // u
  { 0, 0, 1, 1, 0, 0, 0, 0 },  // v (u minus left bar)
  { 0, 1, 1, 0, 1, 1, 0, 0 },  // w ('11 shape')
  { 0, 1, 1, 0, 1, 1, 1, 0 },  // x (H)
  { 0, 1, 1, 1, 0, 1, 1, 0 },  // y (9 missing top bar)
  { 1, 1, 0, 1, 1, 0, 1, 0 }   // Z (same as 2)
};

const tSegmentPattern Blank = { 0, 0, 0, 0, 0, 0, 0, 0 };

const tSegmentPattern *Encode7Segments(char cAscii)
{
  // Convert lower case to upper
  if (cAscii > 90) cAscii -= 32;

  // Space is a special case
  if (cAscii == 32) return &Blank;
  
  // Unknown characters
  if (cAscii < 48 || cAscii > 90)  return NULL;

  return &SegmentPatterns[cAscii-48];
}



// LED Segments go clockwise from top:
//
//     A
//   F   B
//     G
//   E   C
//     D

void PrintSevenSegmentDigit(char cAscii) {
  const tSegmentPattern *pSegs;

  pSegs = Encode7Segments(cAscii);
  if (pSegs == NULL) {
    Serial.print(F("PrintSevenSegmentDigit: Unknown character: "));
    Serial.print(cAscii);
    return;
  }
  
  if (pSegs->a)  Serial.println(F(" -- "));
  else           Serial.println(' ');

  if (pSegs->f)  Serial.print(F("|  "));
  else           Serial.print(F("   "));

  if (pSegs->b)  Serial.println('|');
  else           Serial.println(' ');

  if (pSegs->g)  Serial.println(F(" -- "));
  else           Serial.println(' ');

  if (pSegs->e)  Serial.print(F("|  "));
  else           Serial.print(F("   "));

  if (pSegs->c)  Serial.println('|');
  else           Serial.println(' ');

  if (pSegs->d)  Serial.println(F(" -- "));
  else           Serial.println(' ');
}


void PrintAllSevenSegmentDigits() {
  char c;

  for (c=48; c<=90; c++)
    PrintSevenSegmentDigit(c);
}
