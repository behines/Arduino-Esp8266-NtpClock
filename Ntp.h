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

// NTP time stamp is in the first 48 bytes of the message
#define NTP_PACKET_SIZE (48)

class tNtp {
public:
  tNtp(IPAddress &IpAddress, unsigned int uiLocalPort);
  void SendRequest();
  bool GetResponse();
  
protected:
  IPAddress &_IpAddress;
  WiFiUDP    _Udp;  // A UDP instance to let us send and receive packets over UDP
  
  byte       _PacketBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
};



#endif /* INC_NTP_H */
