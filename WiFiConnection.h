/***************
* tWifiConnection - Manages connection to the DHCP server
* 
* Brad Hines
* Feb 2020
*/

#ifndef INC_WIFICONNECTION_H
#define INC_WIFICONNECTION_H

#include <ESP8266WiFi.h>

#define WIFI_SECONDS_BETWEEN_CONNECTION_TRIES (10)

class tWiFiConnection {
public:
  tWiFiConnection(const char *sSsid, const char *sPassword, int iLedPin);
  bool ConnectToRouter(int iNumTimesToTry = 0);
  int  GetStatus();
  void PrintStatus();
  void PrintInfo();
  
protected:
  const char *_sSsid;
  const char *_sPassword;
  int         _iLedPin;
  int         _status;

};

#endif /* INC_WIFICONNECTION_H */
