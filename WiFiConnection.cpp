/***************
* NTP Clock
* 
* Brad Hines
* Feb 2020
*/

#include "WiFiConnection.h"

/***************************************
* tWiFiConnection constructor
*
* INPUTS:
*   sSsid     - a string containing the SSID of your router, e.g. "MyNetGearWiFi"
*   sPassword - The password for your wifi as a string, e.g. "LotsOfGerbils"
*   iLedPin   - which ESP8266 GPIO pin to use for connection status.  Recommend using
*               2, which is the LED on the ESP8266 module itself.  If you specify 0, 
*               no LED will be used, and you can use the LED pins for other things.
*/

tWiFiConnection::tWiFiConnection(const char *sSsid, const char *sPassword, int iLedPin) :
  _sSsid(sSsid), 
  _sPassword(sPassword), 
  _iLedPin(iLedPin)
{
  // Init the display pin as an output
  if (iLedPin > 0)  pinMode(iLedPin, OUTPUT);

  _status = WL_IDLE_STATUS;
}


/***************************************
* tWiFiConnection::ConnectToRouter
*
* Forge a connection to the WiFi router
*
* Will try connecting at 10-second intervals.
*
* INPUTS:
*   iNumTimesToTry - will try this many times before returning.  Set to 0 to
*                    try forever.
* RETURNS:
*   true if connected to the router
*   false if failed after iNumTimesToTry
*  
*/

bool tWiFiConnection::ConnectToRouter(int iNumTimesToTry)
{ 
  bool bKeepTrying = true;

  _status = GetStatus();
  if (_status == WL_NO_SHIELD) {
    Serial.println(F("WiFi-capable hardware not present"));
    return false;
  }

  while (bKeepTrying) {

    while (_status != WL_CONNECTED) {
      Serial.print(F("Attempting to connect to SSID: "));
      Serial.print(_sSsid);
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      WiFi.begin(_sSsid, _sPassword);

      // wait 10 seconds for connection:
      for (int iWait = 0; iWait < WIFI_SECONDS_BETWEEN_CONNECTION_TRIES; iWait++) {
        Serial.print(F("."));
        delay(1000);
        if (GetStatus() == WL_CONNECTED) {
          bKeepTrying = false;
          break;
        }
      }

      PrintStatus();
    }

    // If iNumTimesToTry is non-zero, decrement it.  If it reaches 0, stop trying
    if (iNumTimesToTry > 0 && --iNumTimesToTry < 1)  bKeepTrying = false;
  }

  Serial.println(F("\n*** Connected to wifi ***\n"));
}


/***************************************
* tWiFiConnection::GetStatus
*
* Retrieves the status of the connection and sets the LED to match
* 
* Note that the LED on the ESP module is active low, so this follows that
* paradigm.
*
* INPUTS:
*   None
* OUTPUTS:
*   The current connection status
* SIDE_EFFECTS:
*   Updates _status to match the current status.
*   Turns on the LED if status is WL_CONNECTED, else turns it off.  Outputs a 0 to turn the 
*   LED on, 1 to turn it off.
*/

int tWiFiConnection::GetStatus()
{
  _status = WiFi.status();

  if (_iLedPin > 0) {
    digitalWrite(_iLedPin, !(_status == WL_CONNECTED));
  }

  return _status;
}


/***************************************
* tWiFiConnection::PrintStatus
*
* Prints the current status of the connection to Serial.
*
*/

void tWiFiConnection::PrintStatus()
{
  switch (_status) {
    case WL_CONNECTED:       Serial.println(F("Connected"));         break;
    case WL_IDLE_STATUS:     Serial.println(F("Idle"));              break;
    case WL_NO_SHIELD:       Serial.println(F("No Shield"));         break;
    case WL_NO_SSID_AVAIL:   Serial.println(F("No SSID Available")); break;
    case WL_SCAN_COMPLETED:  Serial.println(F("Scan completed"));    break;
    case WL_CONNECT_FAILED:  Serial.println(F("Scan completed"));    break;
    case WL_CONNECTION_LOST: Serial.println(F("Connection lost"));   break;
    case WL_DISCONNECTED:    Serial.println(F("Disconnected"));      break;
    default:                 Serial.println(F("Unknown"));           break;
  }
}

/***************************************
* tWiFiConnection::PrintInfo
*
* Prints the details of the connection to Serial.
*
*/

void tWiFiConnection::PrintInfo() 
{
  // print the SSID of the network you're attached to:
  Serial.print(F("SSID: "));
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print(F("IP Address: "));
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print(F("Signal strength (RSSI):"));
  Serial.print(rssi);
  Serial.println(F(" dBm"));
}
