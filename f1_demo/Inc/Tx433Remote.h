//
// Created by Administrator on 2020/6/30.
//

#ifndef F1_DEMO_TX433REMOTE_H
#define F1_DEMO_TX433REMOTE_H

#include "main.h"



typedef struct {
    u8 tx_sw;
    u8 send_num;
    u8 step;
    u8 count;
    u8 dat;
    u8 dat_bit;
}TX_433_Ir_DATA;

typedef enum {
    TX433_IR_GET_NONE,
    TX433_IR_STEP_GET_BIT,
    TX433_IR_STEP_0bit,
    TX433_IR_STEP_1bit,
    TX433_IR_STEP_END,
}IT_DAT_STEP;

void Tx433GetBit(void);
void Tx433IrDataMove(void);
void Tx433IrSend0Bit(void);
void Tx433IrSend1Bit(void);
void Tx433IrSendEnd(void);

void Tx433IrTask(void);
void Tx433IrOpen(u8 num);
void Tx433IrInit(void);


#endif //F1_DEMO_TX433REMOTE_H
