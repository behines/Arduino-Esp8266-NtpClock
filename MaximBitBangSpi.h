/***************
* NTP Clock
* 
* Brad Hines
* Feb 2020
*/


#ifndef MaximBitBangSPI_h
#define MaximBitBangSPI_h

#define MAX_CS_GPIO (5)

#include <Arduino.h>

#define MAXIM_LOGIC_DELAY delayMicroseconds(1)


class tMaximBitBangSpi {
public:
  tMaximBitBangSpi() {}
  tMaximBitBangSpi(int ClkPin, int DinPin, int DoPin, int CsPin);
  uint16_t Write16(uint16_t x);
  uint8_t ReadReg(uint8_t reg);

protected:
  uint8_t _ClockOutBit(uint8_t bit);

  int _ClkPin;
  int _DinPin;
  int _DoPin;
  int _CsPin;
};




#endif   /* MaximBitBangSPI_h */
