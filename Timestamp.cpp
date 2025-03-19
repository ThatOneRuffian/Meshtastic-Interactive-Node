#include <DS3231.h>
#include "PrintDivider.h"
#include "Timestamp.h"

#ifdef RTC_ENABLED
void printTimestamp() {
  DS3231 rtc;

  // print datetime
  const char* daysOfWeek[] = {"", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  const char* months[] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                          "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  
  const unsigned int timestampBuffer = 48;
  char timestamp[timestampBuffer];
  const __FlashStringHelper* smallDiv = F("-");
  bool centuryBit = true;
  if(snprintf(timestamp, timestampBuffer, "Timestamp (UTC-0): %s. %02u, %s. %04u %02u:%02u:%02u", 
          daysOfWeek[rtc.getDoW()],         // Day of week
          rtc.getDate(),                    // Day of month
          months[rtc.getMonth(centuryBit)], // Month
          rtc.getYear(),                    // Year
          0,                                // Hour placeholder
          rtc.getMinute(),                  // Minutes
          rtc.getSecond()                   // Seconds
  )){
      Serial.println(timestamp);
    }
  else{
    Serial.println(F("Could Not Format Datetime"));
  }
  
}
#else
void printTimestamp() {
  const unsigned int timestampBufferSize = 48;
  char timestampBuffer[timestampBufferSize];
  if(snprintf(timestampBuffer, timestampBufferSize, "System Timestamp: %lu", millis()) > 0){
    // If successful, then print system timestamp
    Serial.println(timestampBuffer);
  }
}
#endif 
