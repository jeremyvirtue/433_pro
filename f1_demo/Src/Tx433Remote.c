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
#include "keeloq_lock.h"
#include "stdio.h"

TX_433_Ir_DATA irDat;


#define TX433DATA_LEN   4

//u8 TX_433_Ir_BUF[TX433DATA_LEN] = {0x00,0x07};
//u8 TX_433_Ir_BUF[TX433DATA_LEN] = {0x11,0x22,0x33,0x44};
u8 TX_433_Ir_BUF[TX433DATA_LEN] = {0x00,0x00,0x00,0x00};

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
    static u8 buf[TX433DATA_LEN]= {0};
    u8 i,j;
    u32 passw = 0;
    printf("\r\n");
    for ( i = 0; i < TX433DATA_LEN; i++) {
        j = TX433DATA_LEN-i - 1;
        buf[i]++;
        TX_433_Ir_BUF[i] = buf[i];
        passw = passw | ((u32)TX_433_Ir_BUF[i] << j*8);//将4byte的数组成员一个32位的数

    }
//    printf("passw =  0x%x\r\n",passw);
    passw = CRYPT(passw);//对数据进行加密
//     printf("pwssword_decode =  %x\r\n",passw);

    for (int i = 0; i < TX433DATA_LEN; i++) {//将加密后的数据重新导入txbuf的数组中
        j = TX433DATA_LEN-i - 1;
        TX_433_Ir_BUF[i] =( passw>> j*8 )& 0x000000ff;//
    }
    Tx433IrInit();//开始发送数据
    irDat.tx_sw = 1;
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




