//
// Created by Administrator on 2020/7/1.
//
/*
433���մ���*/

#include "Rx433Remote.h"
#include "stdio.h"
#include "keeloq_lock.h"

RX433_IR_DATA rx_dat;
#define Rx433_IR_DATA_LEN 4

u8 Rx433IrBuf[Rx433_IR_DATA_LEN];
u8 GetRx433IrBuf[Rx433_IR_DATA_LEN];

volatile u16 a1;
volatile u16 a2;

u8 rx_433_ir_send_ok;

void Rx433_Ir_Init()
{
    rx_dat.time = 0;
    rx_dat.step = 0;
    rx_dat.bit_count = 0;
    rx_dat.dat_count = 0;
}

void Rx433IrDispose(void ){
    u8 i,j;
    u32 passw = 0;
    if(rx_433_ir_send_ok){
        rx_433_ir_send_ok = 0;

        printf("\r\n");

        for(i = 0;i<Rx433_IR_DATA_LEN;i++){//��ȡ���ݰ��������ϵ�һ��32λ������
            j = Rx433_IR_DATA_LEN-i - 1;
            passw = passw | ((u32)GetRx433IrBuf[i] << j*8);
        }
//        printf("code = %x\r\n",passw);

        printf("code = %x\r\n",passw);
        passw = DECRYPT(passw);//���ݽ���

        for ( i = 0; i < Rx433_IR_DATA_LEN; i++){//���ܵ�4byte�����ϵ��������������
            j = Rx433_IR_DATA_LEN-i - 1;
            GetRx433IrBuf[i] =( passw>> j*8 )& 0x000000ff;
            printf("getbuf[%d] = %x\t",i,GetRx433IrBuf[i]);
        }
        printf("\r\n");

    }

}

void Rx433IrDatMove(void){
    u8 i;
    rx_dat.time = 0;
    rx_dat.time++;//�����Ѿ�������1����ʱ�����ڣ����Լ������Լ�һ
    rx_dat.bit_count++;
    rx_dat.step = 2;

    if(rx_dat.bit_count % 8 == 0){
        rx_dat.dat_count++;
    }
    if(rx_dat.bit_count>=Rx433_IR_DATA_LEN * 8 ){
            LED0_SET;
            for(i = 0;i<Rx433_IR_DATA_LEN;i++){
//                printf("%d\t",Rx433IrBuf[i]);//����ʹ��printf������Ӱ��ͨ������ ����©��
                GetRx433IrBuf[i] = Rx433IrBuf[i];//��ȡ����
                Rx433IrBuf[i] = 0;
            }
        rx_433_ir_send_ok = 1;//�յ����ݵı�־λ
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
                rx_dat.time++;//�����Ѿ�������1����ʱ�����ڣ����Լ������Լ�һ
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
                    rx_dat.time++;//�����Ѿ�������1����ʱ�����ڣ����Լ������Լ�һ
                    rx_dat.step = 3;
                }
                else if(rx_dat.time>7 && rx_dat.time < 11){//1
                    rx_dat.time = 0;
                    rx_dat.time++;//�����Ѿ�������1����ʱ�����ڣ����Լ������Լ�һ
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

















