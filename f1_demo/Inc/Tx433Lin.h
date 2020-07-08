//
// Created by Administrator on 2020/6/28.
//

#ifndef F1_DEMO_TX433LIN_H
#define F1_DEMO_TX433LIN_H

#include "main.h"

//#define ASK				tx_433_Pin


typedef enum {
    ASK_NULL,
    ASK_START,
    ASK_FREQ,
    ASK_ID,
    ASK_OFF,
}ASK_MODE;

extern u8 ask_sw;//433开关
extern u8 ask_runnum;//433总共发送数据的次数
extern u8 ask_buf[10];//发送的数


void AskTask();
void AskOpen(u8 count);
void AskClose(void);
void AskInit(void);
void AskDataSet(u8 dat0,u8 dat1,u8 dat2,u8 dat3/*,u8 dat4,u8 dat5,u8 dat6,u8 dat7*/);
void AskSendData(u8 num, ASK_MODE order);


#endif //F1_DEMO_TX433LIN_H
