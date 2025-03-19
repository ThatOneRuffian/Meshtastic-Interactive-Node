#include "PrintDivider.h"
const uint8_t _paddingWidth = 54;

void printDivider(const __FlashStringHelper* glyph) {
    for (uint8_t i = 0; i <= _paddingWidth; i++) {
        Serial.print(glyph);
    }
    Serial.println();
}
