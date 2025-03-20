#include <Meshtastic.h>
#include <Wire.h>
#include "SystemLog.h"

// to exclude SRAM memory profiling set debug to false
// consumes 26 bytes of SRAM
#define DEBUG_MODE true

#if DEBUG_MODE
  #include "MemoryStats.h"
#endif

// todo
// usage disclaimer
// node description/menu handler?

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. USB port only
  }
  // log if unable to connect after certain duration?
  // need toolset to set limit for elasped time mills and datetime? can I get epoch time?
  // interface must implement? object that gets passed? or lib knows how to count elapsed times?
  Wire.begin();  // start i2c

  // todo set time if power loss, prompt for epoch
  //time_t currentTime = 1742279720;
  //rtc.setEpoch(currentTime);

  syslog bootLog = {"System Booted"};
  writeInternalLogEntry(bootLog);
}

void loop() {
  if (Serial.available() > 0) {

    // print self-test

    // todo this should only be ran if the last write is 0?
    extern systemLogHeader _logHeaderData;
    EEPROM.get(0, _logHeaderData);
    if (_logHeaderData.nextWriteAddr == 0){
      Serial.println(F("Initializing Internal Log"));
      initInternalLog();
    }

    #if DEBUG_MODE
      printMemoryStats();
      void* testBuffer = malloc(1024);
      if (testBuffer == nullptr) {
        syslog buffErr = {"Failed Malloc"};
        writeInternalLogEntry(buffErr);
        return;
      }
      printMemoryStats();
      free(testBuffer);
    #endif
    
    delay(10000);
  }
}
