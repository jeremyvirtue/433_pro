//
// Created by Administrator on 2020/6/30.
//

/*发射频率为433.92M，最大功率。
协议：
1.编码一共24位，BYTE0+BYTE1+BYTE2+结束位
        BYTE0,BYTE1为地址码，滚动码，每个遥控器的地址不同。
BYTE2为键值码，固定不变。
每帧码之间没有间隔。
0：0.3MS高电平+0.9MS低电平
1：0.9MS高电平+0.3MS低电平
结束位为0.3MS高电平+9.3MS低电平*/

#include "Tx433Remote.h"

TX_433_Ir_DATA irDat;


#define TX433DATA_LEN   8

//u8 TX_433_Ir_BUF[TX433DATA_LEN] = {0xe0,0x07};
u8 TX_433_Ir_BUF[TX433DATA_LEN] = {0X00,0XFF,0X00,0XFF,0X00,0XFF,0X00,0XFF};

/*433初始化*/
void Tx433IrInit(void ){
    tx_433_Pin_RESET;
    irDat.tx_sw = 0;
    irDat.step = TX433_IR_STEP_GET_BIT;
    irDat.count = 0;
    irDat.dat = 0;
    irDat.dat_bit = 0;
}

/*发射并设定发射次数*/
void Tx433IrOpen(u8 num){
    Tx433IrInit();
    irDat.tx_sw = 1;
    TX_433_Ir_BUF[0]++;
    irDat.send_num = num;
}

/*位获取*/
void Tx433GetBit(void){
    u8 getbit;
    getbit = (TX_433_Ir_BUF[irDat.dat] << irDat.dat_bit) & 0x80;
    if(getbit)
        irDat.step = TX433_IR_STEP_1bit;
    else
        irDat.step = TX433_IR_STEP_0bit;
}

/*位移动*/
void Tx433IrDataMove(void){
    irDat.count = 0;
    irDat.dat_bit++;//位寄存器递增
    if(irDat.dat_bit == 8){//位寄存器>8，个寄存器递增
        irDat.dat_bit = 0;
        irDat.dat++;
    }
    if(irDat.dat == TX433DATA_LEN){
        irDat.dat = 0;
        irDat.step = TX433_IR_STEP_END;
    }
    else{
        Tx433GetBit();
    }
}

/*1：0.9MS高电平+0.3MS低电平*/
void Tx433IrSend1Bit(void){
    irDat.count++;
    if(irDat.count <= 9){//900us高电平
        tx_433_Pin_SET;
    }
    else{
        tx_433_Pin_RESET;
        if(irDat.count == (3+9)){
            Tx433IrDataMove();
        }
    }
}

/* 0：0.3MS高电平+0.9MS低电平*/
void Tx433IrSend0Bit(){
    irDat.count++;
    if(irDat.count <= 3){//300us高电平
        tx_433_Pin_SET;
    }
    else{
        tx_433_Pin_RESET;
        if(irDat.count == (3+9)){
            Tx433IrDataMove();
        }
    }
}

/*结束位为0.3MS高电平+9.3MS低电平*/
void Tx433IrSendEnd(void){
    irDat.count++;
    if(irDat.count<=3){
        tx_433_Pin_SET;
    }
    else if(irDat.count <= (93+3)){
        tx_433_Pin_RESET;
    }
    else{
        irDat.step = TX433_IR_STEP_GET_BIT;
        irDat.count = 0;
        irDat.send_num--;//发射次数递减
        if(!irDat.send_num) {
            irDat.tx_sw = 0;//关闭发射
        }
    }
}



void Tx433IrTask(){
    if(irDat.tx_sw){
        switch (irDat.step) {
            case TX433_IR_STEP_GET_BIT:
                Tx433GetBit();
                break;
            case TX433_IR_STEP_1bit:
                Tx433IrSend1Bit();
                break;
            case TX433_IR_STEP_0bit:
                Tx433IrSend0Bit();
                break;
            case TX433_IR_STEP_END:
                Tx433IrSendEnd();
                break;
        }

    }

}




