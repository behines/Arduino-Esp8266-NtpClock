/***************
* Wifi connection info
*
* Your wifi connection details go here.
* 
* Brad Hines
* Feb 2020
*/

#indef SSID_H
#define SSID_H

// You need to create your own ssidPrivate.h file that defines these values
// NTP_SSID   -  a text string that is the SSID of your network, like "MyWifiNet"
// NTP_PASSWD - the WPA2 password for your Wifi network.

// The reason for ssidPrivate is so that I can create an app that works for me, and
// publish it to GitHub, without publishing my Wifi access info.

// Note that this must be a2.4 Ghz network to work with the ESP8266.  Newer "5G" networks
// are not supported by the device.

// You don't need to create an ssidPrivate.h.  Instead you can comment out this #include
// and fill in the defines below

// You can use the F() variants if you like - these will save a few bytes of RAM by 
// storing your SSID and Password in flash instead of in RAM.

#include "ssidPrivate.h"

//#define NTP_SSID   "TheNameOfMyHomeWifi"
//#define NTP_PASSWD "TheSuperSecretWifiPassword"
//#define NTP_SSID   F("TheNameOfMyHomeWifi")
//#define NTP_PASSWD F("TheSuperSecretWifiPassword")

#endif  /* SSID_H */
