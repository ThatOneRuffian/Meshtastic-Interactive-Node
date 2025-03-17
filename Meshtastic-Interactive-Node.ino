#define DEBUG_MODE true
#if DEBUG_MODE
  #include "MemoryStats.h"
#endif

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Native USB port only
  }
}

void loop() {
  if (Serial.available() > 0) {
    
    #if DEBUG_MODE
      printMemoryStats();
      void* testBuffer = malloc(1024);
      if (testBuffer == nullptr) {
        Serial.println("Could not allocate memory for  testBuffer");
        return;
      }
      printMemoryStats();
      free(testBuffer);
    #endif
    
    delay(10000);
  }
}
