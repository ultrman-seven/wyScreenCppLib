#include "frameShow.hpp"
// #include "chain.hpp"
#include "string.h"

#include "list"

struct _FrameColCnt
{
    uint8_t startAdd;
    uint8_t len;
};

#define _FrameColCnt2MemBlock(__FRAME_COL_CNT_NAME) (*(uint16_t *)&__FRAME_COL_CNT_NAME)

void Frame::display(void)
{
    // wyADT::Chain<_FrameColCnt> diff;
    std::list<_FrameColCnt> diff;
    _FrameColCnt colCnt;
    _FrameColCnt lastCol;
    uint8_t flg;
    uint8_t c;

    uint8_t *p1;
    uint8_t *p2;
    p1 = this->picUpdate;
    p2 = this->picDisplay;
    this->picUpdate = this->picDisplay;
    this->picDisplay = p1;

    // compare and find the difference
    for (uint8_t r = 0; r < this->picRow; ++r)
    {
        flg = 0; // colCNT is recording data now
        for (c = 0; c < this->picCol; ++c)
        {
            if (*p1++ != *p2++)
            {
                if (flg)
                    colCnt.len++;
                else
                {
                    colCnt.startAdd = c;
                    colCnt.len = 1;
                    flg = 1;
                }
            }
            else
            {
                if (flg)
                    diff.push_back(colCnt);
                flg = 0;
            }
        }
        // a col loop is finished
        if (flg)
        // diff.append(colCnt);
            diff.push_back(colCnt);

        // send pic data
        _FrameColCnt2MemBlock(lastCol) = 0;
        for (auto &i : diff)
        {
            if (i.startAdd - (lastCol.startAdd + lastCol.len) >= 8)
            {
                this->picShowLine(this->picDisplay, r, lastCol.startAdd, lastCol.len);
                _FrameColCnt2MemBlock(lastCol) = _FrameColCnt2MemBlock(i);
            }
            else
                lastCol.len = i.startAdd - lastCol.startAdd + i.len;
        }
        if (lastCol.len)
            this->picShowLine(this->picDisplay, r, lastCol.startAdd, lastCol.len);
        // diff.clean();
        diff.clear();
    }
}

void Frame::clear(void)
{
    memset(this->picUpdate, 0x00, this->picRow * this->picCol);
}

void Frame::char_display(const uint8_t *ptr_pic, bool contrast, uint8_t l, uint8_t word, uint8_t h, uint8_t w)
{
    for (uint8_t r = l; r < l + (h / 8); ++r)
        memcpy(this->picUpdate + r * this->picCol + word * this->asciiWide, ptr_pic, w);
}

void Frame::char_display(uint8_t *ptr_pic, bool contrast, uint8_t l, uint8_t word, uint8_t h, uint8_t w)
{
    for (uint8_t r = l; r < l + (h / 8); ++r)
        memcpy(this->picUpdate + r * this->picCol + word * this->asciiWide, ptr_pic, w);
}

Frame::Frame(uint8_t picRow, uint8_t picCol, void (*picShowLine)(uint8_t *, uint8_t, uint8_t, uint8_t))
{
    this->picDisplay = new uint8_t[picRow * picCol];
    this->picUpdate = new uint8_t[picRow * picCol];
    this->dataFreeFlag = 1;

    this->picShowLine = picShowLine;
    this->picRow = picRow;
    this->picCol = picCol;
}

Frame::Frame(uint8_t picRow, uint8_t picCol, uint8_t *picShow, uint8_t *picUpdate, void (*picShowLine)(uint8_t *, uint8_t, uint8_t, uint8_t))
{
    this->picUpdate = picUpdate;
    this->picDisplay = picShow;

    this->picShowLine = picShowLine;
    this->picRow = picRow;
    this->picCol = picCol;
}

Frame::~Frame()
{
    if (this->dataFreeFlag)
    {
        delete picDisplay;
        delete picUpdate;
    }
}
