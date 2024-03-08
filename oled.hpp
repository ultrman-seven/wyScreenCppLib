#ifndef __WY_LIB_CPP_OLED_H__
#define __WY_LIB_CPP_OLED_H__
// #include "common.h"
// #include "wy_gpio.hpp"
#include "generalO.hpp"
// #include <stdio.h>

namespace screen
{
    class OLED_Object : public genO::generalOutputBase // dc,cs,res
    {
    private:
        inline void sendByte(uint8_t dat, bool isCMD);
        inline void setCol(uint8_t col);
        inline void setPage(uint8_t page);
        virtual void char_display(const uint8_t *ptr_pic, bool contrast, uint8_t l, uint8_t word, uint8_t h, uint8_t w);
        virtual void char_display(uint8_t *ptr_pic, bool contrast, uint8_t l, uint8_t word, uint8_t h, uint8_t w);
        void (*transFunc)(uint8_t dat) = nullptr;
        // GPIO::Gpio_Object *dc = nullptr, *cs = nullptr, *res = nullptr;
        void (*dcOut)(bool);
        void (*csOut)(bool);
        void (*resOut)(bool);

    public:
        // char str[64] = {0};
        // OLED_Object() = default;
        OLED_Object(void (*dc)(bool) = nullptr, void (*cs)(bool) = nullptr, void (*res)(bool) = nullptr, void (*f)(uint8_t) = nullptr)
            : dcOut(dc), csOut(cs), resOut(res), transFunc(f) { this->reset(); }
        // void printStr(void) { print(str); }
        void reset(void);
        virtual void clear(void);
        void Picture_display(uint8_t *ptr_pic, uint8_t colStart, uint8_t pageStart, uint8_t line, uint8_t col);
        void Picture_display(const uint8_t *ptr_pic, uint8_t colStart, uint8_t pageStart, uint8_t line, uint8_t col);
        void Screen_FillClear(uint8_t filler);
        void reverse(bool on);
        void setBrightness(uint8_t val);
    };
}
#endif /* __WY_LIB_CPP_OLED_H__ */
