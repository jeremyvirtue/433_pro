//
// Created by Administrator on 2020/6/30.
//

/*����Ƶ��Ϊ433.92M������ʡ�
Э�飺
1.����һ��24λ��BYTE0+BYTE1+BYTE2+����λ
        BYTE0,BYTE1Ϊ��ַ�룬�����룬ÿ��ң�����ĵ�ַ��ͬ��
BYTE2Ϊ��ֵ�룬�̶����䡣
ÿ֡��֮��û�м����
0��0.3MS�ߵ�ƽ+0.9MS�͵�ƽ
1��0.9MS�ߵ�ƽ+0.3MS�͵�ƽ
����λΪ0.3MS�ߵ�ƽ+9.3MS�͵�ƽ*/

#include "Tx433Remote.h"
#include "keeloq_lock.h"
#include "stdio.h"

TX_433_Ir_DATA irDat;


#define TX433DATA_LEN   4

//u8 TX_433_Ir_BUF[TX433DATA_LEN] = {0x00,0x07};
//u8 TX_433_Ir_BUF[TX433DATA_LEN] = {0x11,0x22,0x33,0x44};
u8 TX_433_Ir_BUF[TX433DATA_LEN] = {0x00,0x00,0x00,0x00};

/*433��ʼ��*/
void Tx433IrInit(void ){
    tx_433_Pin_RESET;
    irDat.tx_sw = 0;
    irDat.step = TX433_IR_STEP_GET_BIT;
    irDat.count = 0;
    irDat.dat = 0;
    irDat.dat_bit = 0;
}

/*���䲢�趨�������*/
void Tx433IrOpen(u8 num){
    static u8 buf[TX433DATA_LEN]= {0};
    u8 i,j;
    u32 passw = 0;
    printf("\r\n");
    for ( i = 0; i < TX433DATA_LEN; i++) {
        j = TX433DATA_LEN-i - 1;
        buf[i]++;
        TX_433_Ir_BUF[i] = buf[i];
        passw = passw | ((u32)TX_433_Ir_BUF[i] << j*8);//��4byte�������Աһ��32λ����

    }
//    printf("passw =  0x%x\r\n",passw);
    passw = CRYPT(passw);//�����ݽ��м���
//     printf("pwssword_decode =  %x\r\n",passw);

    for (int i = 0; i < TX433DATA_LEN; i++) {//�����ܺ���������µ���txbuf��������
        j = TX433DATA_LEN-i - 1;
        TX_433_Ir_BUF[i] =( passw>> j*8 )& 0x000000ff;//
    }
    Tx433IrInit();//��ʼ��������
    irDat.tx_sw = 1;
    irDat.send_num = num;
}

/*λ��ȡ*/
void Tx433GetBit(void){
    u8 getbit;
    getbit = (TX_433_Ir_BUF[irDat.dat] << irDat.dat_bit) & 0x80;
    if(getbit)
        irDat.step = TX433_IR_STEP_1bit;
    else
        irDat.step = TX433_IR_STEP_0bit;
}

/*λ�ƶ�*/
void Tx433IrDataMove(void){
    irDat.count = 0;
    irDat.dat_bit++;//λ�Ĵ�������
    if(irDat.dat_bit == 8){//λ�Ĵ���>8�����Ĵ�������
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

/*1��0.9MS�ߵ�ƽ+0.3MS�͵�ƽ*/
void Tx433IrSend1Bit(void){
    irDat.count++;
    if(irDat.count <= 9){//900us�ߵ�ƽ
        tx_433_Pin_SET;
    }
    else{
        tx_433_Pin_RESET;
        if(irDat.count == (3+9)){
            Tx433IrDataMove();
        }
    }
}

/* 0��0.3MS�ߵ�ƽ+0.9MS�͵�ƽ*/
void Tx433IrSend0Bit(){
    irDat.count++;
    if(irDat.count <= 3){//300us�ߵ�ƽ
        tx_433_Pin_SET;
    }
    else{
        tx_433_Pin_RESET;
        if(irDat.count == (3+9)){
            Tx433IrDataMove();
        }
    }
}

/*����λΪ0.3MS�ߵ�ƽ+9.3MS�͵�ƽ*/
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
        irDat.send_num--;//��������ݼ�
        if(!irDat.send_num) {
            irDat.tx_sw = 0;//�رշ���
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




