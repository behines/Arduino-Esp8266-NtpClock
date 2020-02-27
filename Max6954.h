/***************
* NTP Clock
* 
* Brad Hines
* Feb 2020
*/


#ifndef MAX6954_H
#define MAX6954_H

#define MAX_CS_GPIO (5)

#include <Arduino.h>


/***********************************
* MAX6954 register definitions
*/

#define MAX6954_REG_NoOp               (0x00)
#define MAX6954_REG_DecodeMode         (0x01)
#define MAX6954_REG_GlobalIntensity    (0x02)
#define MAX6954_REG_ScanLimit          (0x03)
#define MAX6954_REG_Configuration      (0x04)
#define MAX6954_REG_GPIOData           (0x05)
#define MAX6954_REG_PortConfiguration  (0x06)
#define MAX6954_REG_DisplayTest        (0x07)
#define MAX6954_REG_KEY_A_MSK_Deb      (0x08)
#define MAX6954_REG_KEY_B_MSK_Deb      (0x09)
#define MAX6954_REG_KEY_C_MSK_Deb      (0x0A)
#define MAX6954_REG_KEY_D_MSK_Deb      (0x0B)
#define MAX6954_REG_DigitType          (0x0C)
#define MAX6954_REG_KEY_A_Pressed      (0x0C)
#define MAX6954_REG_KEY_B_Pressed      (0x0D)
#define MAX6954_REG_KEY_C_Pressed      (0x0E)
#define MAX6954_REG_KEY_D_Pressed      (0x0F)
#define MAX6954_REG_Intensity10        (0x10)
#define MAX6954_REG_Intensity32        (0x11)
#define MAX6954_REG_Intensity54        (0x12)
#define MAX6954_REG_Intensity76        (0x13)
#define MAX6954_REG_Intensity10a       (0x14)
#define MAX6954_REG_Intensity32a       (0x15)
#define MAX6954_REG_Intensity54a       (0x16)
#define MAX6954_REG_Intensity76a       (0x17)
#define MAX6954_REG_Digit0_PlaneP0     (0x20)
#define MAX6954_REG_Digit1_PlaneP0     (0x21)
#define MAX6954_REG_Digit2_PlaneP0     (0x22)
#define MAX6954_REG_Digit3_PlaneP0     (0x23)
#define MAX6954_REG_Digit4_PlaneP0     (0x24)
#define MAX6954_REG_Digit5_PlaneP0     (0x25)
#define MAX6954_REG_Digit6_PlaneP0     (0x26)
#define MAX6954_REG_Digit7_PlaneP0     (0x27)
#define MAX6954_REG_Digit0a_PlaneP0    (0x28)
#define MAX6954_REG_Digit1a_PlaneP0    (0x29)
#define MAX6954_REG_Digit2a_PlaneP0    (0x2A)
#define MAX6954_REG_Digit3a_PlaneP0    (0x2B)
#define MAX6954_REG_Digit4a_PlaneP0    (0x2C)
#define MAX6954_REG_Digit5a_PlaneP0    (0x2D)
#define MAX6954_REG_Digit6a_PlaneP0    (0x2E)
#define MAX6954_REG_Digit7a_PlaneP0    (0x2F)
#define MAX6954_REG_Digit0_PlaneP1     (0x40)
#define MAX6954_REG_Digit1_PlaneP1     (0x41)
#define MAX6954_REG_Digit2_PlaneP1     (0x42)
#define MAX6954_REG_Digit3_PlaneP1     (0x43)
#define MAX6954_REG_Digit4_PlaneP1     (0x44)
#define MAX6954_REG_Digit5_PlaneP1     (0x45)
#define MAX6954_REG_Digit6_PlaneP1     (0x46)
#define MAX6954_REG_Digit7_PlaneP1     (0x47)
#define MAX6954_REG_Digit0a_PlaneP1    (0x48)
#define MAX6954_REG_Digit1a_PlaneP1    (0x49)
#define MAX6954_REG_Digit2a_PlaneP1    (0x4A)
#define MAX6954_REG_Digit3a_PlaneP1    (0x4B)
#define MAX6954_REG_Digit4a_PlaneP1    (0x4C)
#define MAX6954_REG_Digit5a_PlaneP1    (0x4D)
#define MAX6954_REG_Digit6a_PlaneP1    (0x4E)
#define MAX6954_REG_Digit7a_PlaneP1    (0x4F)
#define MAX6954_REG_Digit0_PlanesP0P1  (0x60)
#define MAX6954_REG_Digit1_PlanesP0P1  (0x61)
#define MAX6954_REG_Digit2_PlanesP0P1  (0x62)
#define MAX6954_REG_Digit3_PlanesP0P1  (0x63)
#define MAX6954_REG_Digit4_PlanesP0P1  (0x64)
#define MAX6954_REG_Digit5_PlanesP0P1  (0x65)
#define MAX6954_REG_Digit6_PlanesP0P1  (0x66)
#define MAX6954_REG_Digit7_PlanesP0P1  (0x67)
#define MAX6954_REG_Digit0a_PlanesP0P1 (0x68)
#define MAX6954_REG_Digit1a_PlanesP0P1 (0x69)
#define MAX6954_REG_Digit2a_PlanesP0P1 (0x6A)
#define MAX6954_REG_Digit3a_PlanesP0P1 (0x6B)
#define MAX6954_REG_Digit4a_PlanesP0P1 (0x6C)
#define MAX6954_REG_Digit5a_PlanesP0P1 (0x6D)
#define MAX6954_REG_Digit6a_PlanesP0P1 (0x6E)
#define MAX6954_REG_Digit7a_PlanesP0P1 (0x6F)

/*********************************************
* Bitmasks for computing digit register numbers
*/

#define MAX6954_REG_PLANE0  (0x20)
#define MAX6954_REG_PLANE1  (0x40)


/*********************************************
* Register bit definitions
*/

#define MAX6954_CFG_SHUTDOWN_MODE              (0x00)
//#define MAX6954_CFG_X                        (0x01)   // Not used
#define MAX6954_CFG_BLINKRATE_FAST             (0x02)
#define MAX6954_CFG_GLOBALBLINK_ENABLE         (0x04)
#define MAX6954_CFG_GLOBALBLINK_TIMING_RESET   (0x10)
#define MAX6954_CFG_GLOBAL_CLEAR_DIGIT_DATA    (0x20)
#define MAX6954_CFG_USE_INDIVIDUAL_INTENSITIES (0x40)
#define MAX6954_CFG_BLNK_PHASE_READBACK        (0x80)

// Digit types for Digit Type register.  These are two-bit codes
// Reading between the lines, what you see is that a 0 means "interpret
// this as a 16-segment digit or device" while 1 means "interpret this 
// digit as a 14-segment digit."
#define MAX6954_DIGIT_TYPE_16_AND_16  (0x00)
#define MAX6954_DIGIT_TYPE_14_AND_16  (0x01)
#define MAX6954_DIGIT_TYPE_16_AND_14  (0x10)
#define MAX6954_DIGIT_TYPE_14_AND_14  (0x11)


// Define a bitfield structure to hold the segment patterns for each digit
class tMax6954 {
public:
  tMax6954();
  void _SetupSPI();
  void Init(uint8_t NumDigits);

  void WriteCmd(uint8_t Register, uint8_t Data);
  void DisplayTest(bool bEnable);
  void SetScanLimit(uint8_t NumDigits);
  void WriteConfig(uint8_t u8Flags);

  void NoOp();
  void SetBrightness(uint8_t uiBrightness);
  void SetDecodeMode(uint8_t uiDecodeFlags);
  void SetDigitTypes(uint8_t DigitTypes76, uint8_t DigitTypes54,
                     uint8_t DigitTypes32, uint8_t DigitTypes10);
  void WriteDigit(uint8_t u8Digit, uint8_t u8Planes, uint8_t u8Value);

protected:
  uint8_t _u8ConfigRegisterValue;
};




#endif   /* MAX6954_H */
