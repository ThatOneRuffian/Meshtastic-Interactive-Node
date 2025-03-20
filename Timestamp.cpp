#include <DS3231.h>
#include "PrintDivider.h"
#include "Timestamp.h"
#include "SystemLog.h"

const char* months[] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

#if RTC_ENABLED

void getTimestamp(smallTimestamp timestampBuffer){
  DS3231 rtc;
  bool centuryBit = false;
  if(!snprintf(timestampBuffer, sizeof(smallTimestamp), "%s.%02u.%02u %02uh%02um",
      months[rtc.getMonth(centuryBit)], // Month
      rtc.getDate(),                    // Day of month
      rtc.getYear(),               // Year
      0,                                // Hour placeholder
      rtc.getMinute()                   // Minutes
  )){
    Serial.println(F("Unable to copy buffer"));
  }
}

void printTimestamp() {
  DS3231 rtc;
  //getTemperature()
  // print datetime
  const char* daysOfWeek[] = {"", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  largeTimestamp timestampBuffer;
  const __FlashStringHelper* smallDiv = F("-");
  bool centuryBit = true;
  if(snprintf(timestampBuffer, sizeof(largeTimestamp), "Timestamp (UTC-0): %s %02u, %s. %04u %02u:%02u:%02u",
          daysOfWeek[rtc.getDoW()],         // Day of week
          rtc.getDate(),                    // Day of month
          months[rtc.getMonth(centuryBit)], // Month
          rtc.getYear()+2000,               // Year
          0,                                // Hour placeholder
          rtc.getMinute(),                  // Minutes
          rtc.getSecond()                   // Seconds
  )){
      Serial.println(timestampBuffer);
    }
  else{
    syslog fmtErr = {"Fmt Time Fail"};
    writeInternalLogEntry(fmtErr);
  }
}

#else
void getTimestamp(smallTimestamp timeBuffer){
  if(!snprintf(timeBuffer, sizeof(smallTimestamp), "%lu", millis())){
    syslog fmtErr = {"Fmt Time Fail"};
    writeInternalLogEntry(fmtErr);
  }
}

void printTimestamp() {
  smallTimestamp timestampBuffer;
  if(snprintf(timestampBuffer, sizeof(smallTimestamp), "System Timestamp: %lu", millis()) > 0){
    // If successful, then print system timestamp
    Serial.println(timestampBuffer);
  }
}
#endif 
