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
bool            bTickOccurred = false;
  
// start of timerCallback
void timerCallback(void *pArg) {
  bTickOccurred = ~bTickOccurred;
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

  Serial.println(F("Hello from NtpClock"));

  pinMode(NodeLedPin, OUTPUT);

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
 
  tNow      = NtpServer.GetUtcTime();
  tNowLocal = TimeZoneSet.TimeZone(iTimeZone)->UtcToLocal(tNow);

  iThisSecond = second(tNowLocal);
  if (iThisSecond != iLastSecondPrinted) {
    iLastSecondPrinted = iThisSecond;
    sprintf(sTimeStr, "%02d:%02d:%02d, %d", hour(tNowLocal), minute(tNowLocal), iThisSecond, bTickOccurred);
    Serial.println(sTimeStr);
  }
  delay(100);
}
