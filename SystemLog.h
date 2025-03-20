#ifndef SYSTEM_LOG_H
#define SYSTEM_LOG_H

#include <Arduino.h>
#include <EEPROM.h>
#include "Timestamp.h"

const uint16_t MAX_INTERNAL_EEPROM = 1024;
const uint8_t MAX_ENTRY_SIZE = 16;

typedef char syslog[MAX_ENTRY_SIZE];

struct systemLogHeader {
  uint16_t nextWriteAddr;
  uint16_t totalLogWrites;
};

struct syslogEntry {
  smallTimestamp logTimestamp;
  syslog logEntry;
};


const uint8_t MAX_LOG_ENTRIES = (MAX_INTERNAL_EEPROM - sizeof(systemLogHeader)) / sizeof(syslogEntry);
const uint8_t INTERNAL_LOG_START = sizeof(systemLogHeader);

uint16_t getLogWriteCount();

void writeInternalLogEntry(syslog);
void viewLogWriteCount();
void clearInternalLogs();
void dumpInternalLogs();
void initInternalLog();
#endif
