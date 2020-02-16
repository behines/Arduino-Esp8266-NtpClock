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

  _status = WL_IDLE_STATUS:
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
    Serial.println("WiFi-capable hardware not present");
    return false;
  }

  while (bKeepTrying) {

    while (_status != WL_CONNECTED) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(sSsid);
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      status = WiFi.begin(sSsid, sPassword);

      // wait 10 seconds for connection:
      for (iWait = 0; iWait < WIFI_SECONDS_BETWEEN_CONNECTION_TRIES; iWait++) {
        delay(1000);
        _status = GetStatus();
        if (_status == WL_CONNECTED)  break;
      }

      PrintStatus();
    }

    // If iNumTimesToTry is non-zero, decrement it.  If it reaches 0, stop trying
    if (iNumTimesToTry > 0 && --iNumTimesToTry < 1)  bKeepTrying = false;
  }

  Serial.println("\n*** Connected to wifi ***\n");
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
    case WL_CONNECTED:       Serial.println("Connected");         break;
    case WL_IDLE_STATUS:     Serial.println("Idle");              break;
    case WL_NO_SHIELD:       Serial.println("No Shield");         break;
    case WL_NO_SSID_AVAIL:   Serial.println("No SSID Available"); break;
    case WL_SCAN_COMPLETED:  Serial.println("Scan completed");    break;
    case WL_CONNECT_FAILED:  Serial.println("Scan completed");    break;
    case WL_CONNECTION_LOST: Serial.println("Connection lost");   break;
    case WL_DISCONNECTED:    Serial.println("Disconnected");      break;
    default:                 Serial.println("Unknown");           break;
  }
}


#endif /* INC_WIFICONNECTION_H */
