/***************
* Wifi connection info
*
* Your wifi connection details go here.
* 
* Brad Hines
* Feb 2020
*/

#ifndef SSID_H
#define SSID_H

// You need to create your own ssidPrivate.h file that defines these values
// NTP_SSID   -  a text string that is the SSID of your network, like "MyWifiNet"
// NTP_PASSWD - the WPA2 password for your Wifi network.

// Note that the SSID must be a 2.4 Ghz network to work with the ESP8266.  Newer "5G" networks
// are not supported by the device.

// The reason for ssidPrivate here is so that I can create an app that works for me, and
// publish it to GitHub, without publishing my Wifi access info.
//
// If you are trying out this library, you don't need to create an ssidPrivate.h.  Instead,
// just can comment out the #include below and fill in and uncomment the defines below instead

#include "ssidPrivate.h"

//#define NTP_SSID   "TheNameOfMyHomeWifi"
//#define NTP_PASSWD "TheSuperSecretWifiPassword"

#endif  /* SSID_H */
