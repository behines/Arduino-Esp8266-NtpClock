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


/*****************************************
* Constants
*/

#define NTP_RETRY_INTERVAL_SECONDS (10)

int ModuleLedPin =  2;
int ModeLedPin   = 16;
unsigned int localPort = 2390;           // local port to listen for UDP packets

// Note - the tNtp class demans that this be constant for the life of the app
char timeServer[] = "time.nist.gov";    // time.nist.gov NTP server


/*****************************************
* Global objects
* 
*/

tWiFiConnection WiFiConnection(NTP_SSID, NTP_PASSWD, ModuleLedPin);
tNtp            NtpServer(timeServer, localPort);


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
  int i;

  Serial.println(F("\nSending request to NTP server..."));  
  NtpServer.SendRequest(); // send an NTP packet to a time server
  
  for (i=0; i<NTP_RETRY_INTERVAL_SECONDS; i++) {

    // If we get an answer, break out.  Else try again
    if (NtpServer.GetResponse())
      break;

    // Pause then try again
    delay(1000);
  }

  // Wait ten seconds before asking for the time again
  delay(10000);
}
