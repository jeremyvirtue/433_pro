//
// Created by Administrator on 2020/6/29.
//

#include "Rx433Lin.h"
#include <stdio.h>
/*该程序433协议
96us定时器
		9ms高电						{数据0（高电				数据1（高电平1.2ms
		平同步码						平560us低						，低电平1.2ms）
												电平1.2ms)
	————————————				 		————						    ————————
										|				      	|			|						  |							|
										|						|			|  0					  |							|    1
										|						|			|						  |							|
										————————				————————						————————
							4.5ms低电
							平同步码

注意：若要接收x个字节，发射代码必须发射x+1个字节（最后一个字节为结束码）
*/


// 96us定时器
//同步码时间 高电平8ms  低电平4.5ms
#define ASK_START_H_MAX		67 	//9ms
#define ASK_START_H_MIN		53	//7ms

#define ASK_START_L_MAX		52//	5ms   //
#define ASK_START_L_MIN		31//	3ms

//#define ASK_END_MIN				260//40ms/96us

#define ASK_BIT_LEN				32//字节




//数据时间  “1” 高 1.1ms 低1.1ms
//					“0” 高 560us 低1.1ms

#define ASK_DATA1_MAX		14  // 1ms/96us
#define ASK_DATA1_MIN		10		// 1.4ms/96us

#define ASK_DATA0_MAX		9  // 760us/96us
#define ASK_DATA0_MIN		3		// 360us/96us

u16 rx_ask_time_count;//电平计数器
u8  rx_ask_step;//步骤标志位
u8  rx_data_count;//数据计数器
u8  rx_pin_level = 0;//钳限制标志位
u8  rx_Askbuf[10] ;//接收的数据
u8  rx_num;

volatile u8 rx_succeed_num;


u8 AskReadbuf[10] ;

void RX_AskRxCode()
{
    u8 i;
    if(!ASK_SW)
    {
        switch(rx_ask_step)
        {
            case 0://开启接收脚位输入模式，作初始化检测
            {
                if(PING_ASK)
                {
                    rx_ask_time_count = 0;
                    rx_ask_step = 1;
                }
            }break;
            case 1:
            {
                if(PING_ASK)//引脚高电平同步码计时器
                {
                    rx_ask_time_count++;
                }
                else
                {
                    if((rx_ask_time_count < ASK_START_H_MIN )|| (rx_ask_time_count > ASK_START_H_MAX))//超出同步码电平时间，返回
                    {
                        rx_num = 0;//标志位计数器复位
                        rx_data_count = 0;
                        rx_ask_step = 0;
                        return;
                    }
                    rx_ask_time_count = 0;
                    rx_ask_step = 2;//高电平同步码接收完毕，进入第二步
                }
            }break;
            case 2:
            {
                if(!PING_ASK)//检测低电平同步码计时器
                {
                    rx_ask_time_count++;
                }
                else
                {
                        if((rx_ask_time_count > ASK_START_L_MAX) || (rx_ask_time_count < ASK_START_L_MIN))//若高于同步码和低于同步码时间，返回
                        {
                            rx_num = 0;//标志位计数器复位
                            rx_data_count = 0;
                            rx_ask_step = 0;
                            return;
                        }
                        rx_ask_time_count = 0;//计数器清0
                        rx_ask_step = 3;//进入第三步
                }
            }break;
            case 3:
            {
                if(PING_ASK)//数据0、1是通过高电平时间判断，所以电平跳变为低的时候，
                {
                     rx_pin_level = 1;//等待低电平 低电平只运行一次
                    rx_ask_time_count++;
                }
                else
                {
                    if(rx_pin_level)
                    {
                        rx_pin_level = 0;// 低电平只运行一次该代码
                        rx_data_count++;//数据位接收器
                        if((rx_ask_time_count <= ASK_DATA1_MAX) && (rx_ask_time_count >= ASK_DATA1_MIN))//接收位1 先收高位
                        {
                            rx_Askbuf[rx_num] = rx_Askbuf[rx_num]<<1;
                            rx_Askbuf[rx_num] =rx_Askbuf[rx_num] |0x01;
                        }
                        else if((rx_ask_time_count <= ASK_DATA0_MAX) && (rx_ask_time_count >= ASK_DATA0_MIN))//接收位0
                        {

                            rx_Askbuf[rx_num] =(rx_Askbuf[rx_num]<<1);
                        }
                        else{
                            rx_num = 0;
                            rx_ask_time_count = 0;
                            rx_ask_step = 0;
                            for(i = 0;i<ASK_BIT_LEN/8;i++)
                            {
                                rx_Askbuf[i] = 0;
                            }
                            return;
                        }
                        rx_ask_time_count = 0;//电平计数器清0
                        if((rx_data_count % 8) == 0)//接收8位，跳转下一个字节
                        {
                            rx_num++;
                        }
                        if(rx_data_count > ASK_BIT_LEN - 1)//超过限定最大字节，进行数据解析（不能有延时）
                        {
                                LED0_SET;
                                rx_succeed_num++;
                                for(i = 0;i<ASK_BIT_LEN/8;i++)
                                {
                                    AskReadbuf[i] = rx_Askbuf[i];
                                    printf("0x%x\t",rx_Askbuf[i]);
                                }
                                printf("\r\n");


                            rx_num = 0;//标志位计数器复位
                            rx_data_count = 0;
                            rx_ask_step = 0;
                        }
                   }
                }
            }break;
        }
    }
}

void Rx_data_Print(void){
    u8 i;
    for(i = 0;i<ASK_BIT_LEN/8;i++){
        printf("AskReadbuf[%d] = 0x%x",i,AskReadbuf[i]);

    }
}


