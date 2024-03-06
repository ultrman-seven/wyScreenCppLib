#include "generalO.hpp"
#include "stdlib.h"
// #include "mathTable.hpp"
// #include "stack.hpp"
#include "stack"

namespace genO
{
    const char *endl = "\r\n";
    const char tab = '\t';
    const char *testStr = "hello world";
} // namespace genO

using namespace genO;
uint16_t utf8ToUnicode(uint8_t *c)
{
    uint16_t result = 0;
    uint8_t tmp;
    result += c[2] & 0x3f;
    tmp = c[1] & 0x3f;
    result += tmp << 6;
    tmp = c[0] & 0x0f;
    result += tmp << 12;
    return result;
}
#define ZhStart 0x3400
#define MidLen 0x19b5
#define Zh2Start 0x4e00
void generalOutputBase::putChar(char ch, bool contrast)
{
    if (wordCount >= maxWord)
    {
        wordCount = 0;
        line += asciiHigh / 8;
    }
    if (line >= maxLine)
        return;
    if (this->zhCnt)
    {
        this->zhBuf[this->zhCnt++] = (uint8_t)ch;
        if (this->zhCnt == 3)
        {
            this->zhCnt = 0;
            uint16_t zh = utf8ToUnicode(this->zhBuf);
            uint8_t zhSize = zh_High * zh_Wide / 8;
            uint8_t *pic = (uint8_t *)calloc(zhSize, sizeof(uint8_t));
            if (zh - ZhStart > MidLen)
                this->loadZH((MidLen + zh - Zh2Start + 1) * zhSize, pic, zhSize);
            else
                this->loadZH((zh - ZhStart) * zhSize, pic, zhSize);
            char_display(pic, contrast, this->line, this->wordCount, zh_High, zh_Wide);
            this->wordCount += 2;
            free(pic);
        }
        return;
    }
    if (((uint8_t)ch & 0xf0) == 0xe0)
    {
        if (this->loadZH != nullptr)
            this->zhBuf[this->zhCnt++] = (uint8_t)ch;
        return;
    }
    switch (ch)
    {
    case '\n':
        line += asciiHigh / 8;
        wordCount = 0;
        return;
    case '\b':
        wordCount--;
        char_display(font, contrast);
        return;
    case '\a':
        char_display(font, contrast);
#ifndef _LOW_SPACE__
        (this->placeHolder)[line / (asciiHigh / 8)]->push_back(wordCount);
#endif
        break;
    case 127:
        break;
    default:
        if (ch >= 32)
            char_display(font + fontUnitSize * (ch - 32), contrast);
        break;
    }
    wordCount++;
}

void generalOutputBase::scrollPrint(char *s)
{
    lineStr ls;
    uint8_t sLen = 0;
    if (this->wordCount)
        this->putChar('\n');
    ls.line = this->line;
    ls.s = s;
    this->scrollString.push_back(ls);

    while (*s++)
        sLen++;
}

void generalOutputBase::scrollUpdate(void)
{
}
void generalOutputBase::cursorReset(void)
{
    wordCount = 0;
    line = 0;
}

const unsigned int DecPower[] = {1, 10, 100, 1000, 10000, 100000, 1000000};

void generalOutputBase::putNum(float num, uint8_t n)
{
    int numIntPart;
    numIntPart = num;
    this->putNum(numIntPart);
    this->putChar('.');
    num -= numIntPart;
    num = num >= 0 ? num : -num;
    numIntPart = num * DecPower[n];
    this->putNum(numIntPart);
}
void generalOutputBase::putNum(__Screen__IO int num)
{
    std::stack<char> numStack;
    if (num < 0)
    {
        putChar('-');
        num = -num;
    }
    // if (num >= 10)
    //     putNum(num / 10);
    while (num)
    {
        numStack.push(num % 10 + '0');
        num /= 10;
    }
    while (!numStack.empty())
    {
        putChar(numStack.top());
        numStack.pop();
    }
    // putChar(num % 10 + '0');
}

void generalOutputBase::putNum(__Screen__IO int num, uint8_t decimalPlaces)
{
    uint32_t count;
    if (num < 0)
    {
        putChar('-');
        num = -num;
    }
    while (decimalPlaces--)
        count *= 10;
    if (num > count)
    {
        putNum((int)(num / count));
        putChar('.');
        putNum((int)(num % count));
    }
    else
        putNum(num);
}

void generalOutputBase::print(char *s, bool contrast)
{
    while (*s)
        putChar(*s++, contrast);
}
void generalOutputBase::print(const char *s, bool contrast)
{
    while (*s)
        putChar(*s++, contrast);
}

generalOutputBase &generalOutputBase::operator<<(char *s)
{
    this->print(s);
    return *this;
}
generalOutputBase &generalOutputBase::operator<<(const char *s)
{
    this->print(s);
    return *this;
}
generalOutputBase &generalOutputBase::operator<<(char c)
{
    this->putChar(c);
    return *this;
}
generalOutputBase &generalOutputBase::operator<<(int32_t dat)
{
    this->putNum(dat);
    return *this;
}
generalOutputBase &generalOutputBase::operator<<(float dat)
{
    this->putNum(dat);
    return *this;
}

void generalOutputBase::loadFont(const uint8_t *font, uint8_t asciiHigh, uint8_t asciiWide)
{
    this->font = font;
    this->asciiHigh = asciiHigh;
    this->asciiWide = asciiWide;
    this->fontUnitSize = asciiWide * asciiHigh / 8;
}

void generalOutputBase::loadZH_Font(void (*f)(uint32_t add, uint8_t *buf, uint16_t len), uint8_t h, uint8_t w)
{
    this->loadZH = f;
    this->zh_High = h;
    this->zh_Wide = w;
}

void generalOutputBase::setScreenSize(uint8_t width, uint8_t height)
{
    maxWord = width / asciiWide;
    // maxLine = height / asciiHigh;
    maxLine = height / 8;
#ifndef _LOW_SPACE__
    uint8_t tmp = maxLine / (asciiHigh / 8);
    this->placeHolder = (std::list<uint8_t> **)calloc(tmp, sizeof(void *));
    // this->placeHolder = new wyADT::Chain<uint8_t>[tmp];
    while (tmp--)
        this->placeHolder[tmp] = new std::list<uint8_t>();
#endif
};
#ifndef _LOW_SPACE__
void generalOutputBase::clearPlaceHolder(void)
{
    uint8_t tmp = maxLine / (asciiHigh / 8);
    while (tmp--)
        this->placeHolder[tmp]->clear();
}

void generalOutputBase::placeFill(char *s)
{
    uint8_t tmp = maxLine / (asciiHigh / 8);
    uint8_t __tmpCnt = 0;
    while (__tmpCnt < tmp)
    {
        // this->placeHolder[__tmpCnt]->searchReset();
        // while (this->placeHolder[__tmpCnt]->searchNext(w))
        //     char_display(font + fontUnitSize * (*s++ - 32), false, __tmpCnt * (asciiHigh / 8), w);
        for (auto i : *(this->placeHolder[__tmpCnt]))
            char_display(font + fontUnitSize * (*s++ - 32), false, __tmpCnt * (asciiHigh / 8), i);
        __tmpCnt++;
    }
}
#endif
