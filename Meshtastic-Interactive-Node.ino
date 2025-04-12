#include <Meshtastic.h>
#include <Wire.h>  // For RTC
#include <SD.h>
#include "SystemLog.h"

// to exclude SRAM memory profiling set debug to false
// consumes 26 bytes of SRAM
#define DEBUG_MODE false

#if DEBUG_MODE
  #include "MemoryStats.h"
#endif

const uint8_t chipSelect = 10;

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

  if (!SD.begin(chipSelect)) {

    Serial.println(F("initialization failed. Things to check:"));

    Serial.println(F("1. is a card inserted?"));

    Serial.println(F("2. is your wiring correct?"));

    Serial.println(F("3. did you change the chipSelect pin to match your shield or module?"));

    Serial.println(F("Note: press reset or reopen this serial monitor after fixing your issue!"));

    while (true);
  }

  syslog bootLog = {"System Booted"};
  writeInternalLogEntry(bootLog);
}

void loop() {
  if (!Serial.available() > 0) {
    return;
  }
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

  // open index file
  File bibleIndexFile = SD.open(F("KJV"), FILE_READ);
  if (!bibleIndexFile || !bibleIndexFile.available()) {
      Serial.println(F("Unable to Open Bible Index"));
      return;
  }
  Serial.println(F("Reading Index Header..."));

  //read in index header
  uint16_t magicNumber = 0x0333;
  uint16_t numberRead[1];
  // read magic number
  bibleIndexFile.read(numberRead, sizeof(magicNumber));
  if (numberRead[0] == 0x0333){
    Serial.println(F("Magic Number Found"));
  }else{
    Serial.println(F("Unable to Open Index. Magic Number Not Found"));
  }

  // read index version
  uint8_t indexVersion[1];
  bibleIndexFile.read(indexVersion, sizeof(indexVersion));
  Serial.print(F("Index Version: "));
  Serial.println(indexVersion[0]);
  // read bible info string length

  uint8_t bibleInfoLength[1];
  bibleIndexFile.read(bibleInfoLength, sizeof(bibleInfoLength));

  Serial.print(F("Bible Info: "));
  for (uint8_t i = 0; i < bibleInfoLength[0]; i++){
    Serial.write(bibleIndexFile.read());
  }
  Serial.println();

  // read in number of books found in index
  uint8_t bookCount[1];
  bibleIndexFile.read(bookCount, sizeof(bookCount));
  Serial.print(F("Book Count: "));
  Serial.println(bookCount[0], DEC);
  Serial.println();

  // skip book index
  uint16_t newByteOffset = 0;
  uint16_t bookIndexOffset = bibleIndexFile.position();
  newByteOffset = bibleIndexFile.position() + bookCount[0] * sizeof(uint32_t);
  uint16_t sectionEndSeq[1];

  bibleIndexFile.seek(newByteOffset);

  // read section end sequence to confirm end of header
  bibleIndexFile.read(sectionEndSeq, sizeof(sectionEndSeq));

  if(!sectionEndSeq[0] == 0xFACE){
    Serial.println(F("Could Not Reach Header End"));
    bibleIndexFile.close();
    return;
  }

  Serial.println(F("Index File Open Successfully"));

  // jump to book index
  Serial.print(F("Book Index Start: "));
  Serial.println(bookIndexOffset, DEC);
  bibleIndexFile.seek(bookIndexOffset);

  // read first book offset
  uint32_t bookLoc[1];
  bibleIndexFile.read(bookLoc, sizeof(bookLoc));
  Serial.print(F("Jumping to first book: "));
  Serial.println(bookLoc[0], DEC);
  bibleIndexFile.seek(bookLoc[0]);

  // read book title length
  uint8_t bookTitleLength[1];
  bibleIndexFile.read(bookTitleLength, sizeof(bookTitleLength));
  Serial.print(F("Title Length: "));
  Serial.println(bookTitleLength[0], DEC);
  Serial.print(F("Book Title: "));
  for (uint8_t i = 0; i < bookTitleLength[0]; i++){
    Serial.write(bibleIndexFile.read());
  }
  Serial.println();

  // get chapter count
  uint8_t chapterCount[1];
  bibleIndexFile.read(chapterCount, sizeof(chapterCount));
  Serial.print(F("Chapter Count: "));
  Serial.println(chapterCount[0], DEC);

  // read first chapter location
  if(chapterCount[1] == 0){
    Serial.print(F("Number of Chapters is Zero. Stopping Lookup"));
    bibleIndexFile.close();
    return false;
  }
  uint16_t chapterLoc[1];
  bibleIndexFile.read(chapterLoc, sizeof(chapterLoc));

  Serial.print(F("Jumping to first chapter: "));
  Serial.println(chapterLoc[0], DEC);
  bibleIndexFile.seek(chapterLoc[0]);

  // get number of verses in chapter
  uint8_t verseCount[1];
  Serial.print(F("Verse Count: "));
  bibleIndexFile.read(verseCount, sizeof(verseCount));
  Serial.println(verseCount[0], DEC);
  if(verseCount[0] == 0){
    Serial.print(F("Number of Verses is Zero. Stopping Lookup"));
    bibleIndexFile.close();
    return false;
  }

  // read verse number byte
  uint8_t verseNumber[1];
  bibleIndexFile.read(verseNumber, sizeof(verseNumber));
  Serial.print(F("Verse Number: "));
  Serial.println(verseNumber[0], DEC);

  uint32_t verseOffset[1];
  bibleIndexFile.read(verseOffset, sizeof(verseOffset));
  bibleIndexFile.close();

  Serial.print(F("Verse 1 offset: "));
  Serial.println(verseOffset[0], HEX);

  // read verse from bible source
  File kjv = SD.open(F("KJV.TXT"), FILE_READ);
  if(!kjv || !kjv.available()){
    Serial.println(F("Unable to Open Bible Source"));
    kjv.close();
    return;
  }

  // seek to verse offset byte
  kjv.seek(verseOffset[0]);
  char tmpbuf;
  while(true){
    tmpbuf = kjv.read();
    Serial.print(tmpbuf);
    if (tmpbuf == 0x0A){
      break;
    }
  }

  kjv.close();
  Serial.println();
  delay(10000);
}
