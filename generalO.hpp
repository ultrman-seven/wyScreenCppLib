#ifndef __WY_LIB_CPP_GEN_OUT_H__
#define __WY_LIB_CPP_GEN_OUT_H__
// #include "common.h"
// #include "stdint.h"
// #include "chain.hpp"
#include "list"

#ifdef __IO
#define __Screen__IO __IO
#else
#define __Screen__IO volatile
#endif

namespace genO
{
    class generalOutputBase
    {
    protected:
        const uint8_t *font;
        uint8_t asciiHigh, asciiWide, fontUnitSize;
        uint8_t zh_High, zh_Wide;
        uint8_t maxWord = 16, maxLine = 4;
        uint8_t line = 0, wordCount = 0;
        virtual void char_display(const uint8_t *ptr_pic, bool contrast, uint8_t l, uint8_t word, uint8_t h, uint8_t w) = 0;
        virtual void char_display(uint8_t *ptr_pic, bool contrast, uint8_t l, uint8_t word, uint8_t h, uint8_t w) = 0;
        void char_display(const uint8_t *ptr_pic, bool contrast, uint8_t l, uint8_t word)
        {
            char_display(ptr_pic, contrast, l, word, this->asciiHigh, this->asciiWide);
        }
        void char_display(const uint8_t *ptr_pic, bool contrast)
        {
            char_display(ptr_pic, contrast, this->line, this->wordCount, this->asciiHigh, this->asciiWide);
        }
#ifndef _LOW_SPACE__
        void clearPlaceHolder(void);
#endif
    private:
        struct lineStr
        {
            uint8_t line;
            char *s;
        };
        // wyADT::Chain<uint8_t> **placeHolder = nullptr;
        // wyADT::Chain<lineStr> scrollString;
        std::list<uint8_t> **placeHolder = nullptr;
        std::list<lineStr> scrollString;
        void (*loadZH)(uint32_t add, uint8_t *buf, uint16_t len);
        uint8_t zhCnt = 0;
        uint8_t zhBuf[3];

    public:
        generalOutputBase() = default;
        void loadFont(const uint8_t *font, uint8_t asciiHigh, uint8_t asciiWide);
        void loadZH_Font(void (*f)(uint32_t add, uint8_t *buf, uint16_t len), uint8_t h, uint8_t w);
        void setScreenSize(uint8_t width, uint8_t height);
        void putChar(char c, bool contrast = false);
        void putNum(__Screen__IO int num);
        void putNum(float num, uint8_t effectiveNumber = 7);
        void putNum(__Screen__IO int num, uint8_t decimalPlaces);
        void print(char *s, bool contrast = false);
        void print(const char *s, bool contrast = false);
        void cursorReset(void);
        void scrollPrint(char *s);
        void scrollUpdate(void);
        generalOutputBase &operator<<(char c);
        generalOutputBase &operator<<(int32_t dat);
        generalOutputBase &operator<<(float dat);
        generalOutputBase &operator<<(char *s);
        generalOutputBase &operator<<(const char *s);
#ifndef _LOW_SPACE__
        void placeFill(char *);
#endif
        virtual void clear(void) = 0;
        // ~generalOutputBase();
    };

    extern const char *endl;
    extern const char tab;
    extern const char *testStr;
} // namespace genO
#endif /* __WY_LIB_CPP_GEN_OUT_H__ */
