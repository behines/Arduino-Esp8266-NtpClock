/***************
* NTP Clock
* 
* Brad Hines
* Feb 2020
*/


#ifndef INC_LOCALTIME_H
#define INC_LOCALTIME_H

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <TimeLib.h>
#include <Timezone.h>

class tLocalTime {
public:
friend class tTimeZoneSet;
  tLocalTime(TimeChangeRule DaylightTimeRule, TimeChangeRule StandardTimeRule);

  time_t UtcToLocal(time_t tUtcTime);
  const char *CurTimeZoneShortName() { return _sTzAbbrev; }

protected:
  Timezone _tz;
  
  const char *_sTzAbbrev;

  static const TimeChangeRule usEDT;
  static const TimeChangeRule usEST;
  static const TimeChangeRule usCDT;
  static const TimeChangeRule usCST;
  static const TimeChangeRule usMDT;
  static const TimeChangeRule usMST;
  static const TimeChangeRule usPDT;
  static const TimeChangeRule usPST;
};


struct tTimeZoneItem {
  tLocalTime *pLocalTime;
  char       *sName;
  char       *sShortName;
};


class tTimeZoneSet {
public:
  tTimeZoneSet();

  int GetNumItems() const { return _iNumItems; }
  char       *Name     (int iWhichItem) const { return _TimeZoneItems[iWhichItem].sName;      }
  char       *ShortName(int iWhichItem) const { return _TimeZoneItems[iWhichItem].sShortName; }
  tLocalTime *TimeZone (int iWhichItem) const { return _TimeZoneItems[iWhichItem].pLocalTime; }

protected:
  int            _iNumItems;
  tTimeZoneItem *_TimeZoneItems;
};

#endif /* INC_LOCALTIME_H */
