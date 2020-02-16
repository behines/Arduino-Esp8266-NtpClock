/***************
* tWifiConnection - Manages connection to the DHCP server
* 
* Brad Hines
* Feb 2020
*/

#include <ESP8266WiFi.h>

#ifndef INC_WIFICONNECTION_H
#define INC_WIFICONNECTION_H

#define WIFI_SECONDS_BETWEEN_CONNECTION_TRIES (10)

class tWiFiConnection {
public:
  tWiFiConnection::tWiFiConnection(const char *sSsid, const char *sPassword, int iLedPin);
  bool tWiFiConnection::ConnectToRouter(int iNumTimesToTry = 0);
  int  GetStatus();
  void PrintStatus();
  
protected:
  const char *_sSsid;
  const char *_sPassword;
  int         _iLedPin;
  int         _status;

};


#endif /* INC_WIFICONNECTION_H */
