/***************
* NTP Clock
* 
* Brad Hines
* Feb 2020
*/

#include "Ntp.h"


/*****************************************
* tNtp Constructor
* 
*/

tNtp::tNtp(const char *sTimeServerHostNameOrIp, unsigned int uiLocalPort) :
  _sTimeServerHostNameOrIp(sTimeServerHostNameOrIp)
{
  _Udp.begin(uiLocalPort);
}


/*****************************************
* tNtp::SendRequest
* 
* Send a NTP request to the time server at the given address
*/

void tNtp::SendRequest() 
{
  /*** Construct a NTP request ***/
  // set all bytes in the buffer to 0
  memset(_PacketBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  _PacketBuffer[0] = 0b11100011;   // LI, Version, Mode
  _PacketBuffer[1] = 0;            // Stratum, or type of clock
  _PacketBuffer[2] = 6;            // Polling Interval
  _PacketBuffer[3] = 0xEC;         // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  _PacketBuffer[12]  = 49;
  _PacketBuffer[13]  = 0x4E;
  _PacketBuffer[14]  = 49;
  _PacketBuffer[15]  = 52;

  /*** Send the NTP request ***/
  //_Udp.beginPacket(_IpAddress, 123); //NTP requests are to port 123
  _Udp.beginPacket(_sTimeServerHostNameOrIp, 123); //NTP requests are to port 123
  
  _Udp.write(_PacketBuffer, NTP_PACKET_SIZE);
  _Udp.endPacket();
}


/*****************************************
* tNtp::GetResponse
* 
* Check for and process any incoming NTP packets
*
* RETURNS:
*   true if a NTP packet has been received
*   false if not
* SIDE EFFECTS:
* 
*/

bool tNtp::GetResponse() 
{
  // wait to see if a reply is available
  delay(1000);
  
  if (_Udp.parsePacket()) {
    Serial.println(F("packet received"));
    // We've received a packet, read the data from it
    _Udp.read(_PacketBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(_PacketBuffer[40], _PacketBuffer[41]);
    unsigned long lowWord  = word(_PacketBuffer[42], _PacketBuffer[43]);
    
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print(F("Seconds since Jan 1 1900 = "));
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print(F("Unix time = "));
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);

    // print the hour, minute and second:
    Serial.print(F("The UTC time is "));       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second

    return true;
  }
  
  return false;
}
