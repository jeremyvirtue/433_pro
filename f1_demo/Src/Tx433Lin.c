//
// Created by Administrator on 2020/6/28.
//

#include "Tx433Lin.h"
/*需要extern 的全局变量*/
u8 ask_sw;//433开关
u8 ask_runnum;//433总共发送数据的次数
u8 ask_buf[10] = {0x24,0x00,0x00,0xff,0xFF,0x00,0xFF,0x00,0xFF,0x00};//发送的数据

/*全局变量*/
u8 ask_step;//433步骤
u16 ask_count;//引脚高低电平计数器
u8 data_num;//第几个数据
u8 data_count;//第几位数据
u8 send_count;//433发射次数

#define DATA_LEN		4//发送个数 8个就为8个

#define START_HIGH	60//同步码高电平的时间 2ms
#define START_LOW		41//同步码低电平的时间 2ms

#define DATA_HIGH0	6//数据0 高电平的时间 560us
#define DATA_LOW0		13//数据0 低电平的时间 1.2ms

#define	DATA_HIGH1	13//数据1 高电平的时间 1.2ms
#define DATA_LOW1		13//数据1 低电平的时间1.2ms

#define ASK_END			30//结束位的时间

void AskInit(void)
{
    tx_433_Pin_RESET;
    ask_step = 0;
    ask_count = 0;
    data_num = 0;
    data_count = 0;
    ask_runnum = 0;
}

void AskClose(void)
{
//	ASK_IN;
    ask_sw = 0;
    tx_433_Pin_RESET;
}


void AskOpen(u8 count)
{
    ask_sw = 1;
    send_count = count;
    ask_runnum = 0;//重置发射计数器
}



/*发码流程*/
void AskTask(void)
{
    if(ask_runnum >= send_count)//超过指定发射次数，关闭433
    {
        AskClose();
        ask_runnum = 0;
        send_count = 0;
    }

    if(ask_sw)
    {
        switch(ask_step)
        {
            case 0://同步码高电平
            {
                tx_433_Pin_SET;
                ask_count++;
                if(ask_count >= START_HIGH)
                {
                    ask_step = 1;
                    ask_count = 0;
                }
            }break;
            case 1://同步码低电平
            {
                tx_433_Pin_RESET;
                ask_count++;
                if(ask_count >= START_LOW)
                {
                    ask_count = 0;
                    ask_step = 2;
                }
            }break;
            case 2:
            {
                if((ask_buf[data_num] << data_count) & 0x80)//先发高位
                {//1
                    tx_433_Pin_SET;
                    ask_count++;
                    if(ask_count >= DATA_HIGH1)
                    {
                        ask_count = 0;
                        ask_step = 3;
                    }
                }
                else
                {//0
                    tx_433_Pin_SET;
                    ask_count++;
                    if(ask_count >= DATA_HIGH0)
                    {
                        ask_count = 0;
                        ask_step = 3;
                    }
                }
            }break;
            case 3:
            {
                tx_433_Pin_RESET;
                ask_count++;
                if(ask_count > DATA_LOW1)
                {
                    ask_count = 0;
                    data_count++;
                    if(data_count>7)
                    {
                        data_count = 0;
                        data_num++;
                        if(data_num >= DATA_LEN)
                        {
                            data_num = 0;
                            ask_step = 4;//在完成一组数据时
                        }
                        else//在完成一位数据时
                        {
                            ask_step = 2;
                        }
                    }
                    else//在处理一位数据时
                        ask_step = 2;
                }
            }break;
            case 4:
            {
                tx_433_Pin_RESET;
                ask_count++;
                if(ask_count >= ASK_END)
                {
                    ask_runnum++;
                    ask_count = 0;
                    ask_step = 0;
                }
            }break;
        }
    }
}



/*数据设置*/
void AskDataSet(u8 dat0,u8 dat1,u8 dat2,u8 dat3/*,u8 dat4,u8 dat5,u8 dat6,u8 dat7*/){
    ask_buf[0] = dat0;
    ask_buf[1] = dat1;
    ask_buf[2] = dat2;
    ask_buf[3] = dat3;
//    ask_buf[4] = dat4;
//    ask_buf[5] = dat5;
//    ask_buf[6] = dat6;
//    ask_buf[7] = dat7;
}


void AskSendData(u8 num, ASK_MODE order)
{
//    u8 dat_1;
//    dat_1 = 0<<4;
//    dat_1 |= order;
    ask_buf[0]++;
    AskDataSet(1,2,3,4);
    AskOpen(num);
}

