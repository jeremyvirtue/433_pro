//
// Created by Administrator on 2020/6/28.
//

#include "KeyBoard.h"

KeyData   keys;

void KeyPress(void);
void KeyRelease(void);
void KeyContinued(void);

void KeyScan(void)
{
    uint8_t i;
    for (i=0;i<KEY_COUNT;i++)
    {
        keys.key[i].now=0x00;
    }
    if(KEY1_READ)
    {
        keys.key[KEY_1].now=KEY_DOWN;
    }
    for (i=0;i<KEY_COUNT;i++)
    {
        keys.key[i].press=keys.key[i].now & (keys.key[i].now^keys.key[i].continued);
        keys.key[i].release=keys.key[i].now ^ keys.key[i].press ^ keys.key[i].continued;
        keys.key[i].continued=keys.key[i].now;
        if (keys.key[i].continued==0x00)
            keys.key[i].duration =0;
    }

    KeyPress();
    KeyRelease();
    KeyContinued();

}

/**       按下
*
*
*/
void KeyPress(void) {
    if (keys.key[KEY_1].press)//电源
    {
        keys.key_num = K_SET_P;
    }
}

/**       释放
*
*
*/
void KeyRelease(void) {
    if (keys.key[KEY_1].release)//
    {
        keys.key_num = K_SET_R;
    }
}

/**       按下不放
*
*
*/
void KeyContinued(void) {
    if (keys.key[KEY_1].continued)//电源
    {
        if (keys.key[KEY_1].duration < KEY_DURATION_TIME)
            keys.key[KEY_1].duration++;
        if (keys.key[KEY_1].duration == KEY_DURATION_TIME - 1) {
            keys.key_num = K_SET_C;
        }
    }
}