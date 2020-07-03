//
// Created by Administrator on 2020/6/28.
//

#ifndef F1_DEMO_CONTROL_H
#define F1_DEMO_CONTROL_H

#include "main.h"


typedef struct {
    u8 rx_433_order;
}SYSTEM_DATA;

extern  SYSTEM_DATA sdat;

void KeyMenu(void);

#endif //F1_DEMO_CONTROL_H
