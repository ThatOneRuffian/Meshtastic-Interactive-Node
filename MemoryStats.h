#ifndef MEMORY_STATS_H
#define MEMORY_STATS_H

void printMemoryStats();
void printDataMetric(const __FlashStringHelper* metricTag, const unsigned int dataUsed, const unsigned int *volumeAvailable, const unsigned int *percentUsed);
void printDivider(const char* glyph, const int padCount);

#endif
