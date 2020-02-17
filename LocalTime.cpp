/***************
* NTP Clock
* 
* Brad Hines
* Feb 2020
*/

#include "LocalTime.h"

/*****************************************
* tLocalTime Static members
* 
*/

const TimeChangeRule tLocalTime::usEDT = { "EDT", Second, Sun, Mar, 2, -240 };   //UTC - 4 hours
const TimeChangeRule tLocalTime::usEST = { "EST", First,  Sun, Nov, 2, -300 };   //UTC - 5 hours
const TimeChangeRule tLocalTime::usCDT = { "CDT", Second, Sun, Mar, 2, -300 };   //UTC - 5 hours
const TimeChangeRule tLocalTime::usCST = { "CST", First,  Sun, Nov, 2, -360 };   //UTC - 6 hours
const TimeChangeRule tLocalTime::usMDT = { "MDT", Second, Sun, Mar, 2, -360 };   //UTC - 6 hours
const TimeChangeRule tLocalTime::usMST = { "MST", First,  Sun, Nov, 2, -420 };   //UTC - 7 hours
const TimeChangeRule tLocalTime::usPDT = { "PDT", Second, Sun, Mar, 2, -420 };   //UTC - 7 hours
const TimeChangeRule tLocalTime::usPST = { "PST", First,  Sun, Nov, 2, -480 };   //UTC - 8 hours


/*****************************************
* tLocalTime Constructor
* 
*/

tLocalTime::tLocalTime(TimeChangeRule DaylightTimeRule, TimeChangeRule StandardTimeRule) :
  _tz(DaylightTimeRule, StandardTimeRule)
{
}


/*****************************************
* tLocalTime::UtcToLocal
* 
* Send a NTP request to the time server at the given address
*/

time_t tLocalTime::UtcToLocal(time_t tUtcTime) 
{
  time_t tCurLocalTime;

  TimeChangeRule *pTcr;

  tCurLocalTime = _tz.toLocal(tUtcTime, &pTcr);

  //Serial.print("The time zone is: ");
  //Serial.println(pTcr->abbrev);

  return tCurLocalTime;
}



/*****************************************
* tTimeZoneSet::tTimeZoneSet
* 
* Set up available time zone objects
*/

tTimeZoneSet::tTimeZoneSet()
{
  _iNumItems = 4;

  _TimeZoneItems = new tTimeZoneItem[_iNumItems];

  _TimeZoneItems[0].sName      = "Eastern";
  _TimeZoneItems[0].sShortName = "Edt";
  _TimeZoneItems[0].pLocalTime = new tLocalTime(tLocalTime::usEDT, tLocalTime::usEST);
  
  _TimeZoneItems[1].sName      = "Central";
  _TimeZoneItems[1].sShortName = "Cdt";
  _TimeZoneItems[1].pLocalTime = new tLocalTime(tLocalTime::usCDT, tLocalTime::usCST);

  _TimeZoneItems[2].sName      = "Mountain";
  _TimeZoneItems[2].sShortName = "Mdt";
  _TimeZoneItems[2].pLocalTime = new tLocalTime(tLocalTime::usMDT, tLocalTime::usMST);

  _TimeZoneItems[3].sName      = "Pacific";
  _TimeZoneItems[3].sShortName = "Pdt";
  _TimeZoneItems[3].pLocalTime = new tLocalTime(tLocalTime::usPDT, tLocalTime::usPST);
}
