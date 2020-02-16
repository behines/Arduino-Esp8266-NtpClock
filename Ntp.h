/***************
* NTP Clock
* 
* Brad Hines
* Feb 2020
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#ifndef INC_NTP_H
#define INC_NTP_H

class tNtp {
public:
  tNtp::tNtp();
  
protected:
  IPAddress &IpAddress;
};



#endif /* INC_NTP_H */
