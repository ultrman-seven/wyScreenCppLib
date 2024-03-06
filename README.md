# wyScreenCppLib

example

```C
#include "oled.hpp"

void csFun(bool s)
{
}
void dcFun(bool s)
{
}
void resFun(bool s)
{
}
void spiSend(uint8_t d)
{

}

int main()
{
    screen::OLED_Object oled(dcFun, csFun, resFun, spiSend);

    oled.setScreenSize(128, 64);

    oled << "hello world\n";

    return 0;
}
```
