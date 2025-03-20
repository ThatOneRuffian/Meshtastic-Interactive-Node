#include "PrintDivider.h"

void printDivider(const __FlashStringHelper* glyph) {
  for (uint8_t i = 0; i <= PADDING_WIDTH; i++) {
      Serial.print(glyph);
  }
  Serial.println();
}
