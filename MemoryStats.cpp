#include <Arduino.h>
#include "PrintDivider.h"
#include "Timestamp.h"

extern int __heap_start, *__brkval;

void printDataMetric(const __FlashStringHelper* metricTag, const unsigned int dataUsed, const unsigned int *volumeAvailable, const unsigned int *percentUsed){
  Serial.print(metricTag);
  Serial.print(dataUsed);
  Serial.print(F(" bytes of "));
  Serial.print(*volumeAvailable);
  Serial.print(F(" bytes used ("));
  Serial.print(*percentUsed);
  Serial.println(F("%)"));
}

void printMemoryStats() {
  // Print SRAM usage
  // SRAM stores Text, Data, BSS, Stack, and Heap
  // as per https://docs.arduino.cc/learn/programming/memory-guide/
  // Stack increases down and heap increases up.
  bool v; // mark current stack position

  // AVR memory boundaries
  // todo non-UNO?
  const unsigned int RAM_START = 0x0100;  // 256
  const unsigned int RAM_END = RAMEND;    
  const unsigned int TOTAL_RAM = RAM_END - RAM_START + 1; // 2048 bytes
  const unsigned int HEAP_START = (long)&__heap_start;
  const unsigned int heapTop = (__brkval == 0 ? HEAP_START : (long)__brkval);

  // Calculate sizes
  const unsigned int staticData = HEAP_START - RAM_START;           // Static data size
  const unsigned int heapSize = heapTop - HEAP_START;               // Heap used
  const unsigned int usedStackSize = RAM_END - (long)&v;            // Stack used
  const unsigned int heapStackUsed = heapSize + usedStackSize;      // Combined heap + stack 
  const unsigned int ramAvailable = TOTAL_RAM - staticData;         // Free SRAM

  // Percentages
  const unsigned int staticDataPercent = (staticData * 100) / TOTAL_RAM;
  const unsigned int heapStackPercent = (heapStackUsed * 100) / ramAvailable;
  const unsigned int totalUsedPercent = ((staticData + heapStackUsed) * 100) / TOTAL_RAM;

  // Print Metrics
  const __FlashStringHelper* largeDiv = F("=");
  const __FlashStringHelper* smallDiv = F("-");

  printDivider(largeDiv);
  printTimestamp();
  printDivider(smallDiv);

  // Print total RAM usage
  printDataMetric(F("Total RAM Used: "), staticData + heapStackUsed, &TOTAL_RAM, &totalUsedPercent);
  printDivider(smallDiv);

  // Print RAM Static / Heap / Stack composition
  printDataMetric(F("Static Data Usage: "), staticData, &TOTAL_RAM, &staticDataPercent);
  printDataMetric(F("Heap/Stack Usage: "), heapStackUsed, &ramAvailable, &heapStackPercent);
  printDivider(largeDiv);
  Serial.println();
}
