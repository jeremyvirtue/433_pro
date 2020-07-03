//
// Created by Administrator on 2020/7/1.
//

#include "Rx433Remote.h"
#include "stdio.h"

RX433_IR_DATA rx_dat;
#define Rx433_IR_DATA_LEN 8

u8 Rx433IrBuf[Rx433_IR_DATA_LEN];

volatile u16 a1;
volatile u16 a2;

void Rx433_Ir_Init()
{
    rx_dat.time = 0;
    rx_dat.step = 0;
    rx_dat.bit_count = 0;
    rx_dat.dat_count = 0;

}

void Rx433IrDatMove(void){
    u8 i;
    rx_dat.time = 0;
    rx_dat.time++;
    rx_dat.bit_count++;
    rx_dat.step = 2;

    if(rx_dat.bit_count % 8 == 0){
        rx_dat.dat_count++;
    }
    if(rx_dat.bit_count>=Rx433_IR_DATA_LEN * 8 ){
            LED0_SET;
            for(i = 0;i<Rx433_IR_DATA_LEN;i++){
//                printf("%d\t",Rx433IrBuf[i]);//这里使用printf会严重影响通信质量 导致漏码
                Rx433IrBuf[i] = 0;
            }
            printf("1\r\n");
            Rx433_Ir_Init();
    }

}


void Rx433IrTask(void){
    u8 i;
    switch (rx_dat.step) {
        case 0:
            if(!PING_ASK){
                Rx433_Ir_Init();
                rx_dat.step = 1;
            }
            break;
        case 1:
            if(!PING_ASK){
                rx_dat.time++;
            }
            else{
                if(rx_dat.time<88 || rx_dat.time>100){
                    Rx433_Ir_Init();
                    return;
                }
                rx_dat.time = 0;
                rx_dat.time++;
                rx_dat.step = 2;
            }
            break;
        case 2:
            if(PING_ASK){
                rx_dat.time++;
            }
            else{
                if(rx_dat.time>=2&& rx_dat.time<5){//0
                    rx_dat.time = 0;
                    rx_dat.time++;
                    rx_dat.step = 3;
                }
                else if(rx_dat.time>7 && rx_dat.time < 11){//1
                    rx_dat.time = 0;
                    rx_dat.time++;
                    rx_dat.step = 3;
                }
                else{
                    Rx433_Ir_Init();
                    return;
                }
            }
            break;
        case 3:
            if(!PING_ASK){
                rx_dat.time++;
            }
            else{
                if(rx_dat.time>7 && rx_dat.time < 11){//0
                    Rx433IrBuf[rx_dat.dat_count] <<=1;
                    Rx433IrDatMove();

                }
                else if(rx_dat.time>=2&& rx_dat.time<5){//1
                    Rx433IrBuf[rx_dat.dat_count] <<=1;
                    Rx433IrBuf[rx_dat.dat_count] |= 0x01;
                    Rx433IrDatMove();


                }
                else{
                    Rx433_Ir_Init();
                    return;
                }
            }
            break;
    }
}

















