/***************
* NTP Clock
* 
* Brad Hines
* Feb 2020
*/

#include <ESP8266WiFi.h>
#include "ssid.h"
#include "WiFiConnection.h"
#include "Ntp.h"

#include <SPI.h>

#include <TimeLib.h>
#include "LocalTime.h"
#include "SevenSegment.h"

extern "C" {
#define USE_US_TIMER 1
#include "user_interface.h"
void timerCallback(void *pArg);
}


/*****************************************
* Constants
*
* Most are declared static so that their scope is limited to this file.
*/

#define NTP_REFRESH_INTERVAL_SECONDS (300)

static int ModuleLedPin =  2;
static int NodeLedPin   = 16;
static unsigned int localPort = 2390;           // local port to listen for UDP packets

// Note - the tNtp class demands that this be constant for the life of the app
static const char ntpServerName[] = "us.pool.ntp.org";
//static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";


/*****************************************
* Global objects
* 
*/

tWiFiConnection WiFiConnection(NTP_SSID, NTP_PASSWD, ModuleLedPin);
tNtp            NtpServer(ntpServerName, localPort, NTP_REFRESH_INTERVAL_SECONDS);
tTimeZoneSet    TimeZoneSet;
os_timer_t      MyTimer;
int             iLastVal      = LOW;
  
// start of timerCallback
void timerCallback(void *pArg) {
  if (iLastVal == LOW) {
    digitalWrite(NodeLedPin, HIGH);
    iLastVal = HIGH;
  } 
  else {
    digitalWrite(NodeLedPin, LOW);
    iLastVal = LOW;
  } 
} // End of timerCallback

  
/*****************************************
* setup() - Initialization code for the Arduino app
*
* The job of this routine is to connect to the router.  It will not exit
* and proceed to the body of the application until a connection (and an IP
* address assignment) is complete.
*/

void setup() 
{
  Serial.begin(115200);
  while (!Serial) { }

  Serial.println(F("\n\nHello from NtpClock"));

  SPI.begin();
  Serial.println(F("SPI Online\n"));

  // Device wants MSB then LSB
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.setHwCs(true);  // Needed so that ESP8266 manages CS itself
 
  pinMode(NodeLedPin, OUTPUT);

  //Serial.println(F("Reading Port Configuration Register"))

  Serial.println(F("Leaving shutdown\n"));
  // Config register bits: PIRTEBXS
  SPI.write16(0x0401,true);

  // Set brightness to 15 mA
  Serial.println(F("Set brightness to 15 mA\n"));
  SPI.write16(0x0205,true);

  // Set number of digits to two.  Ths is the "Scan-Limit" register
  Serial.println(F("Set Scan Limit to 2 digits\n"));
  SPI.write16(0x0301,true);
  
  // The recommended value of RSET (56K) also sets the peak current to 40mA, which makes the
  // segment current adjustable from 2.5mA to 37.5mA in 2.5mA steps.  We want to peak at 30 mA.
  // Probably should choose RSET = 75K.

  Serial.println(F("Display Test\n"));
  SPI.write16(0x0701,true);
  
  
  // Connect to the router.  0 means to try forever
  WiFiConnection.ConnectToRouter(0);

  //PrintAllSevenSegmentDigits();
  os_timer_disarm(&MyTimer);
  os_timer_setfn(&MyTimer, &timerCallback, NULL);

  // Arm the timer
  // 250 is the fire time in ms
  // 0 for once and 1 for repeating
  os_timer_arm(&MyTimer, 250, true);

  // This line was in one of the demos, but it seems to make the timer go 10x faste
  // than I request.
  // system_timer_reinit();
}


/*****************************************
* loop() - Runtime code for the Arduino app
* 
* The loop function runs over and over again forever
*/

void loop()
{
  static time_t tNow, tNowLocal;
  static int    iLastSecondPrinted = -1;
  static int    iThisSecond;
  static char   sTimeStr[30];
  static int    iTimeZone = 3;

  static int    iBrightness = 1;
 
  tNow      = NtpServer.GetUtcTime();
  tNowLocal = TimeZoneSet.TimeZone(iTimeZone)->UtcToLocal(tNow);

  iThisSecond = second(tNowLocal);
  if (iThisSecond != iLastSecondPrinted) {
    iLastSecondPrinted = iThisSecond;
    sprintf(sTimeStr, "%02d:%02d:%02d", hour(tNowLocal), minute(tNowLocal), iThisSecond);
    Serial.println(sTimeStr);

    if (++iBrightness > 11) iBrightness = 1;
    SPI.write16(0x0200+iBrightness,true);
  }
  delay(100);
}
