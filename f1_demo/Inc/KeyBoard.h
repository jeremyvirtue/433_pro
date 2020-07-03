//
// Created by Administrator on 2020/6/28.
//

#ifndef F1_DEMO_KEYBOARD_H
#define F1_DEMO_KEYBOARD_H

#include "main.h"

#define KEY1_READ   !HAL_GPIO_ReadPin(key1_GPIO_Port,key1_Pin)

#define KEY_COUNT 		1//按键数量
#define KEY_DOWN        1
#define KEY_DURATION_TIME 60 //


typedef enum
{ //KeyId
    KEY_1,
}KEY_ID;

typedef enum {
    K_NONE,
    K_SET_P,
    K_SET_R,
    K_SET_C,
}KEY_RE_NAME;


typedef struct
{
    uint8_t now:1;         //当前键值
    uint8_t press:1;       //按下
    uint8_t release:1;     //释放
    uint8_t continued:1;   //持续按下
    uint16_t duration;    //持续按下时间
}Key;

typedef struct
{
    Key key[KEY_COUNT];
    uint16_t key_num;

}KeyData;

extern KeyData   keys;


void KeyScan(void);


#endif //F1_DEMO_KEYBOARD_H
