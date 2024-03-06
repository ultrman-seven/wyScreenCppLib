#ifndef __WY_LIB_Frame_Partial_Refresh__
#define __WY_LIB_Frame_Partial_Refresh__

#include "stdint.h"
#include "generalO.hpp"

/**
 * @brief Frame partial refresh
 * @author @ultrman-seven
 * @example
 * void showCallback(uint8_t *dat, uint8_t lineStart, uint8_t colStart, uint8_t colLength)
 * {
 *  // show your pic in screen or somewhere whatever you like
 * }
 *
 * int main()
 * {
 *      Frame frame(8, 128, showCallback);
 *      uint8_t dat[128*8];
 *      while (1)
 *      {
 *          // get some shit down
 *          // do something to update the data in dat. eg: update(dat);
 *
 *          memcpy(frame.picUpdate, dat, 128 * 8);
 *          frame.display();
 *
 *          // get other shit done
 *      }
 * }
 */
class Frame : public genO::generalOutputBase
{
private:
    uint8_t dataFreeFlag = 0;
    uint8_t *picDisplay;
    uint8_t picRow;
    uint8_t picCol;
    void (*picShowLine)(uint8_t *dat, uint8_t lineStart, uint8_t colStart, uint8_t colLength);

    virtual void char_display(const uint8_t *ptr_pic, bool contrast, uint8_t l, uint8_t word, uint8_t h, uint8_t w);
    virtual void char_display(uint8_t *ptr_pic, bool contrast, uint8_t l, uint8_t word, uint8_t h, uint8_t w);

public:
    uint8_t *picUpdate;
    Frame(uint8_t picRow, uint8_t picCol, void (*picShowLine)(uint8_t *dat, uint8_t lineStart, uint8_t colStart, uint8_t colLength));
    Frame(uint8_t picRow, uint8_t picCol, uint8_t *picShow, uint8_t *picUpdate, void (*picShowLine)(uint8_t *dat, uint8_t lineStart, uint8_t colStart, uint8_t colLength));
    void display(void);
    virtual void clear(void);
    ~Frame();
};

#endif /* __WY_LIB_Frame_Partial_Refresh__ */
