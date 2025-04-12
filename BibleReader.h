#include "BibleReader.h"

BibleReader::BibleReader(String& filePath) : filePath_(filePath) {
    file_ = SD.open(filePath_.c_str(), FILE_READ);
    checkHeader();
    getBookInfo();
}

BibleReader::~BibleReader() {
}

bool BibleReader::checkHeader() {
    return true;
}

String BibleReader::verse(int book, int chapter, int verse) {
    return getVerseScripture(0);
}

void BibleReader::getBookInfo() {
}

BibleReader::BookInfo BibleReader::getBook(int bookNumber) {
    BookInfo bookInfo;
    return bookInfo;
}

BibleReader::ChapterInfo BibleReader::getChapter(int bookNumber, int chapterNumber) {
    return ChapterInfo(0, 0);
}

uint32_t BibleReader::getVerseOffset(int bookNumber, int chapterNumber, int verseNumber) {
    return 0x00;
}

String BibleReader::getVerseScripture(uint32_t verseOffset) {
    return "";
}
