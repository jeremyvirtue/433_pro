//
// Created by Administrator on 2020/7/1.
//

#ifndef F1_DEMO_RX433REMOTE_H
#define F1_DEMO_RX433REMOTE_H

#include "main.h"

typedef struct {
    u8 step;
    u16 time;
    u8 dat_count;
    u8 bit_count;
}RX433_IR_DATA;

extern u8 rx_433_ir_send_ok;

void Rx433IrTask(void);
void Rx433IrDispose(void );

#endif //F1_DEMO_RX433REMOTE_H
