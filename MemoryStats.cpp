#include <Arduino.h>
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

void printDivider(const __FlashStringHelper* glyph, const int padCount){
  for (unsigned int i = 0; i <= padCount; i++){
    Serial.print(glyph);
  }
  Serial.println();
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
  const unsigned int paddingWidth = 54;
  //todo research real buffer size of millis()
  const unsigned int timestampBufferSize = 50;
  char timestampBuffer[timestampBufferSize];
  
  printDivider(largeDiv, paddingWidth);
  
  if(snprintf(timestampBuffer, timestampBufferSize, "Timestamp: %lu", millis()) > 0){
    // If successful, then print system timestamp
    Serial.println(timestampBuffer);
    printDivider(smallDiv, paddingWidth);
  }
  
  // Print total RAM usage
  printDataMetric(F("Total RAM Used: "), staticData + heapStackUsed, &TOTAL_RAM, &totalUsedPercent);
  printDivider(smallDiv, paddingWidth);

  // Print RAM Static / Heap / Stack composition
  printDataMetric(F("Static Data Usage: "), staticData, &TOTAL_RAM, &staticDataPercent);
  printDataMetric(F("Heap/Stack Usage: "), heapStackUsed, &ramAvailable, &heapStackPercent);
  printDivider(largeDiv, paddingWidth);
  Serial.println();
}
