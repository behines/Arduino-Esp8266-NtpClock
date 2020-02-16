# Arduino-Esp8266-NtpClock
NTP client on ESP8266 NodeMCU or ESP-12 module under Arduino

This is a distro I wrote to provide a backend for a bedstand LED clock.  The goal is to
make a clock that never has to be set.  I gutted the clock on my nightstand and replaced
its guts with a ESP8266 module running a NTP client.  The distro is in the form on an
Arduino sketch (the .ino file is the master at top).

Below a certain level, this distro is tailored for the LED display of the clock I gutted,
which was a LiteOn 637C1P-12.  I compute the 7-segment display patterns in the ESP8266, 
and then I am using a custom-developed GAL 22V10 logic array to record the current segment
patterns and handle the multiplexing of the display.

However, up to that point, the library is generic and will hopefully be of use to anyone
who wants to know the time on a ESP8266 device.

NOTE: To use this distro, you have to provide your own ssidPrivate.h.  See ssid.h for details.
I want to publish a ready-to-go distro, but obviously I don't want to publish my own WiFi
access credentials.  To deal with this, I put my credentials in the ssidPrivate.h file 
and provide instructions on how to create your own.

