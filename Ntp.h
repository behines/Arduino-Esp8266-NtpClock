/***************
* NTP Clock
* 
* Brad Hines
* Feb 2020
*/


#ifndef INC_NTP_H
#define INC_NTP_H

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <TimeLib.h>


#define NTP_MIN_QUERY_INTERVAL_SECONDS (10)

// NTP time stamp is in the first 48 bytes of the message
#define NTP_PACKET_SIZE (48)

class tNtp {
public:
  //tNtp(IPAddress &IpAddress, unsigned int uiLocalPort);
  tNtp(const char *sTimeServerHostNameOrIp, unsigned int uiLocalPort,
       time_t tQueryIntervalInSeconds = 300);

  time_t GetUtcTime();

protected:
  void _SendRequest();
  bool _GetResponse();

  time_t       _tQueryIntervalInSeconds;
  time_t       _tNextQueryTime;
  time_t       _tCurTimeUtc;

  const char  *_sTimeServerHostNameOrIp;
  WiFiUDP      _Udp;  // A UDP instance to let us send and receive packets over UDP
  byte         _PacketBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
};



#endif /* INC_NTP_H */
