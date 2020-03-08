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
#include "Max6954.h"
#include "ClockDisplay.h"

extern "C" {
  // This define makes the microsecond timer call os_timer_arm_us visible
  // I'm not using it now, but it's worth keeping note of here
  #define USE_US_TIMER 1

  // This appears to define lots of ESP library functions, like for Wifi
  #include "user_interface.h"
  
  // The callback has to be extern-C'ed in order to compile
  // ISRs need to have ICACHE_RAM_ATTR before the function definition to run 
  // the interrupt code in RAM. See
  // https://stackoverflow.com/questions/58113937/esp8266-arduino-why-is-it-necessary-to-add-the-icache-ram-attr-macro-to-isrs-an
  // I assume a timer callback is similar.
  void ICACHE_RAM_ATTR timerCallback(void *pArg);
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

tMax6954        LedDriver;
tClockDisplay   Display(LedDriver);
tWiFiConnection WiFiConnection(NTP_SSID, NTP_PASSWD, ModuleLedPin);
tNtp            NtpServer(ntpServerName, localPort, NTP_REFRESH_INTERVAL_SECONDS);
tTimeZoneSet    TimeZoneSet;
os_timer_t      MyTimer;
int             iLastVal      = LOW;
  
// start of timerCallback
void ICACHE_RAM_ATTR timerCallback(void *pArg) {
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
 
  pinMode(NodeLedPin, OUTPUT);

  // Disable watchdog during the Wifi Init
  // See https://techtutorialsx.com/2017/01/21/esp8266-watchdog-functions/
  ESP.wdtDisable();
    // Connect to the router.  0 means to try forever
    WiFiConnection.ConnectToRouter(0);
    // WiFi.mode(WIFI_OFF);
  ESP.wdtEnable(1000);

  //PrintAllSevenSegmentDigits();
  os_timer_disarm(&MyTimer);
  os_timer_setfn(&MyTimer, &timerCallback, NULL);

  // Arm the timer
  // 250 is the fire time in ms
  // 0 for once and 1 for repeating
  os_timer_arm(&MyTimer, 250, true);

  // This line was in one of the demos, but it seems to make the timer go 10x faster
  // than I request.
  // system_timer_reinit();

  // Init with 2 digit pairs
  LedDriver.Init(2);
}


/*****************************************
* loop() - Runtime code for the Arduino app
* 
* The loop function runs over and over again forever
*/

void loop()
{
  //static char               c = '0';
  //static CLOCK_ANNUNCIATOR ca = CLOCK_ANNUNCIATOR_AM;
  static char cAmPm;
  
  static time_t tNow, tNowLocal;
  static int    iLastSecondPrinted = -1;
  static int    iThisSecond;
  static char   sTimeStr[30];
  static bool   bColon    = false;
  static int    iTimeZone = 3;
  static int    iHour24, iHour12;
  static int    iBrightness = 1;
 
  tNow        = NtpServer.GetUtcTime();
  tNowLocal   = TimeZoneSet.TimeZone(iTimeZone)->UtcToLocal(tNow);

  iThisSecond = second(tNowLocal);
  if (iThisSecond == 0)   bColon = true;

  
  if (iThisSecond != iLastSecondPrinted) {
    iLastSecondPrinted = iThisSecond;
    iHour24 = hour(tNowLocal);

    if (iHour24 < 12) {
      Display.Annunciator[CLOCK_ANNUNCIATOR_AM] = true;
      Display.Annunciator[CLOCK_ANNUNCIATOR_PM] = false;
      cAmPm = 'A';
    }
    else {
      Display.Annunciator[CLOCK_ANNUNCIATOR_AM] = false;
      Display.Annunciator[CLOCK_ANNUNCIATOR_PM] = true;
      cAmPm = 'P';
    }
  
    iHour12 = iHour24 >  12 ? iHour24-12 :
              iHour24 ==  0 ? 12 :
              iHour24;              
    
    sprintf(sTimeStr, "%02d:%02d:%02d %cM", iHour12, minute(tNowLocal), iThisSecond, cAmPm);
    Serial.println(sTimeStr);

    Display.Digit[0] = sTimeStr[0];
    Display.Digit[1] = sTimeStr[1];
    Display.Digit[2] = sTimeStr[3];
    Display.Digit[3] = sTimeStr[4];

    //Serial.println(c);
    
    //Display.Digit[0] = c;
    //Display.Digit[1] = c;
    //Display.Digit[2] = c;
    //Display.Digit[3] = c;

    // if (++c > 'Z')  c = '0';

    //Display.Annunciator[ca] = false;
    //ca = (CLOCK_ANNUNCIATOR) ( ((int)ca) + 1);
    //if (ca >= CLOCK_NUM_ANNUNCIATORS)   ca = CLOCK_ANNUNCIATOR_AM;
    //Display.Annunciator[ca]   = true;
    
    Display.Annunciator[CLOCK_ANNUNCIATOR_COLON] = bColon;
    
    Display.Update();

    if (++iBrightness > 11) iBrightness = 1;
    //LedDriver.SetBrightness(iBrightness);



    //Serial.print  ("Config Reg = 0x");
    //Serial.println(LedDriver.ReadRegister(4), HEX);
    //Serial.print  ("Brightness Reg = ");
    //Serial.println(LedDriver.ReadRegister(2));

    // Keep rewriting config
    //LedDriver.WriteConfig(MAX6954_CFG_SHUTDOWN_MODE);

    //Serial.print("Writing Digit 9 = 0x");
    //Serial.println(bit,HEX);
    //LedDriver.WriteDigit(9, MAX6954_REG_PLANE0 | MAX6954_REG_PLANE1, bit);
    //if (bit & 0x80) bit = 1;
    //else            bit = bit << 1;

    bColon = !bColon;
  }
  delay(100);
}
