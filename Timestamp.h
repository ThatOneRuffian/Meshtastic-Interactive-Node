#ifndef TIMESTAMP_H
#define TIMESTAMP_H
#include <Arduino.h>
#define RTC_ENABLED true
// changing RTC_ENABLED will change the structure of the EEPROM logs
// reinitialize EEPROM logs after changing


#if RTC_ENABLED
  #define TIMESTAMP_SMALL_BUFFER_SIZE 17
  typedef char smallTimestamp[TIMESTAMP_SMALL_BUFFER_SIZE];
  #define TIMESTAMP_LARGE_BUFFER_SIZE 47
  typedef char largeTimestamp[TIMESTAMP_LARGE_BUFFER_SIZE];
#else
  #define TIMESTAMP_SMALL_BUFFER_SIZE 10
  typedef char smallTimestamp[TIMESTAMP_SMALL_BUFFER_SIZE];
  #define TIMESTAMP_LARGE_BUFFER_SIZE 28
#endif

void printTimestamp();
void getTimestamp(smallTimestamp);
#endif
