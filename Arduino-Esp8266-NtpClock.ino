

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

#include <Time.h>

/*****************************************
* Constants
*
* Most are declared static so that their scope is limited to this file.
*/

#define NTP_REFRESH_INTERVAL_SECONDS (300)

static int ModuleLedPin =  2;
static int ModeLedPin   = 16;
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

  // Connect to the router.  0 means to try forever
  WiFiConnection.ConnectToRouter(0);
}


/*****************************************
* loop() - Runtime code for the Arduino app
* 
* The loop function runs over and over again forever
*/

void loop()
{
  static time_t tNow;
  static int    iLastSecondPrinted = -1;
  static int    iThisSecond;
  static char   sTimeStr[30];
 
  tNow = NtpServer.GetUtcTime();

  iThisSecond = second(tNow);
  if (iThisSecond != iLastSecondPrinted) {
    iLastSecondPrinted = iThisSecond;
    sprintf(sTimeStr, "%02d:%02d:%02d", hour(tNow), minute(tNow), iThisSecond);
    Serial.println(sTimeStr);
  }

  delay(100);
}
