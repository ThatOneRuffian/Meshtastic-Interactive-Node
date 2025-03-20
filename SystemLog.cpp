#include "SystemLog.h"

systemLogHeader _logHeaderData;

void writeInternalLogEntry(syslog logMessage){
  // determine if write is an overflow
  if(_logHeaderData.nextWriteAddr+sizeof(syslogEntry) > MAX_INTERNAL_EEPROM){
    _logHeaderData.nextWriteAddr = INTERNAL_LOG_START;
  }
  
  // write log entry
  syslogEntry newLogEntry;
  smallTimestamp timeBuffer;
  getTimestamp(timeBuffer);
  strncpy(newLogEntry.logTimestamp, timeBuffer, sizeof(smallTimestamp));
  strncpy(newLogEntry.logEntry, logMessage, sizeof(syslog));

  EEPROM.put(_logHeaderData.nextWriteAddr, newLogEntry);
  
  // update write counts and next write address for next use
  _logHeaderData.nextWriteAddr += sizeof(syslogEntry);
  
  // catch overflows
  if(_logHeaderData.totalLogWrites < UINT16_MAX){
    _logHeaderData.totalLogWrites++;
  }
  
  EEPROM.put(0, _logHeaderData);
}

void viewLogWriteCount(){
  Serial.println(_logHeaderData.totalLogWrites);
}

uint16_t getLogWriteCount(){
  return _logHeaderData.totalLogWrites;
}

void initInternalLog() {
  // clear logs and initialize log structure
  /*
  +----------------------+
  | systemLogHeader      |
  +----------------------+
  | Log Entry n          |
  +----------------------+
  */
  clearInternalLogs();

  // write log header to EEPROM
  _logHeaderData.nextWriteAddr = INTERNAL_LOG_START;
  _logHeaderData.totalLogWrites = 1;
  EEPROM.put(0, _logHeaderData);
}

void clearInternalLogs() {
  const int emptyValue = 0;
  for (uint16_t addrOffset = INTERNAL_LOG_START; addrOffset < MAX_INTERNAL_EEPROM; addrOffset++) {
    EEPROM.update(addrOffset, emptyValue);
  }
  _logHeaderData.nextWriteAddr = INTERNAL_LOG_START;
  Serial.println(F("Internal Logs Cleared"));
}

void dumpInternalLogs() {
  for (uint16_t addrOffset = INTERNAL_LOG_START; addrOffset+sizeof(syslogEntry) < MAX_INTERNAL_EEPROM; addrOffset += sizeof(syslogEntry)) {
    syslogEntry fetchedLog;
    EEPROM.get(addrOffset, fetchedLog);
    Serial.print(addrOffset);
    Serial.print(":\t");
    if(fetchedLog.logTimestamp[0] == 0x00 && fetchedLog.logEntry[0] == 0x00){
      Serial.println("--");
    }else{
      Serial.print(fetchedLog.logTimestamp);
      Serial.print(": ");
      Serial.println(fetchedLog.logEntry);
    }
  }
}
