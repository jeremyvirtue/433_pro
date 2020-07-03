//
// Created by Administrator on 2020/6/29.
//

#ifndef F1_DEMO_RX433LIN_H
#define F1_DEMO_RX433LIN_H

#include "main.h"

#define ASK_SW		0



/*typedef enum{
    ASK_NULL,
    ASK_START,
    ASK_FREQ,
    ASK_ID,
    ASK_OFF,
    ASK_FACTORY,
}ASK_ORDER;*/


//extern volatile u8 Askbuf[10];
extern u8 AskReadbuf[10];

void RX_AskRxCode(void);

void Rx_data_Print(void);


#endif //F1_DEMO_RX433LIN_H
