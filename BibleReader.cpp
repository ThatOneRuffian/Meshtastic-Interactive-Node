#ifndef BIBLE_READER_H
#define BIBLE_READER_H

#include <Arduino.h>
#include <SD.h>

class BibleReader {
public:
    BibleReader(String& filePath);
    ~BibleReader();
    bool checkHeader();
    String verse(int book, int chapter, int verse);

private:
    File file_;
    String filePath_;

    struct BookInfo {
        String name;
        int chapterCount;
        uint32_t chapterOffset;
    };

    struct ChapterInfo {
        int verseCount;
        uint32_t verseOffsetStart;
        ChapterInfo(int verses, uint32_t offset) : verseCount(verses), verseOffsetStart(offset) {}
    };

    void getBookInfo();
    BookInfo getBook(int bookNumber);
    ChapterInfo getChapter(int bookNumber, int chapterNumber);
    uint32_t getVerseOffset(int bookNumber, int chapterNumber, int verseNumber);
    String getVerseScripture(uint32_t verseOffset);
};

#endif
