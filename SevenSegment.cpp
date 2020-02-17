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

