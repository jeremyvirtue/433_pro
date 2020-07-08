//
// Created by Administrator on 2020/6/29.
//

#include "Rx433Lin.h"
#include <stdio.h>
/*�ó���433Э��
96us��ʱ��
		9ms�ߵ�						{����0���ߵ�				����1���ߵ�ƽ1.2ms
		ƽͬ����						ƽ560us��						���͵�ƽ1.2ms��
												��ƽ1.2ms)
	������������������������				 		��������						    ����������������
										|				      	|			|						  |							|
										|						|			|  0					  |							|    1
										|						|			|						  |							|
										����������������				����������������						����������������
							4.5ms�͵�
							ƽͬ����

ע�⣺��Ҫ����x���ֽڣ����������뷢��x+1���ֽڣ����һ���ֽ�Ϊ�����룩
*/


// 96us��ʱ��
//ͬ����ʱ�� �ߵ�ƽ8ms  �͵�ƽ4.5ms
#define ASK_START_H_MAX		67 	//9ms
#define ASK_START_H_MIN		53	//7ms

#define ASK_START_L_MAX		52//	5ms   //
#define ASK_START_L_MIN		31//	3ms

//#define ASK_END_MIN				260//40ms/96us

#define ASK_BIT_LEN				32//�ֽ�




//����ʱ��  ��1�� �� 1.1ms ��1.1ms
//					��0�� �� 560us ��1.1ms

#define ASK_DATA1_MAX		14  // 1ms/96us
#define ASK_DATA1_MIN		10		// 1.4ms/96us

#define ASK_DATA0_MAX		9  // 760us/96us
#define ASK_DATA0_MIN		3		// 360us/96us

u16 rx_ask_time_count;//��ƽ������
u8  rx_ask_step;//�����־λ
u8  rx_data_count;//���ݼ�����
u8  rx_pin_level = 0;//ǯ���Ʊ�־λ
u8  rx_Askbuf[10] ;//���յ�����
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
            case 0://�������ս�λ����ģʽ������ʼ�����
            {
                if(PING_ASK)
                {
                    rx_ask_time_count = 0;
                    rx_ask_step = 1;
                }
            }break;
            case 1:
            {
                if(PING_ASK)//���Ÿߵ�ƽͬ�����ʱ��
                {
                    rx_ask_time_count++;
                }
                else
                {
                    if((rx_ask_time_count < ASK_START_H_MIN )|| (rx_ask_time_count > ASK_START_H_MAX))//����ͬ�����ƽʱ�䣬����
                    {
                        rx_num = 0;//��־λ��������λ
                        rx_data_count = 0;
                        rx_ask_step = 0;
                        return;
                    }
                    rx_ask_time_count = 0;
                    rx_ask_step = 2;//�ߵ�ƽͬ���������ϣ�����ڶ���
                }
            }break;
            case 2:
            {
                if(!PING_ASK)//���͵�ƽͬ�����ʱ��
                {
                    rx_ask_time_count++;
                }
                else
                {
                        if((rx_ask_time_count > ASK_START_L_MAX) || (rx_ask_time_count < ASK_START_L_MIN))//������ͬ����͵���ͬ����ʱ�䣬����
                        {
                            rx_num = 0;//��־λ��������λ
                            rx_data_count = 0;
                            rx_ask_step = 0;
                            return;
                        }
                        rx_ask_time_count = 0;//��������0
                        rx_ask_step = 3;//���������
                }
            }break;
            case 3:
            {
                if(PING_ASK)//����0��1��ͨ���ߵ�ƽʱ���жϣ����Ե�ƽ����Ϊ�͵�ʱ��
                {
                     rx_pin_level = 1;//�ȴ��͵�ƽ �͵�ƽֻ����һ��
                    rx_ask_time_count++;
                }
                else
                {
                    if(rx_pin_level)
                    {
                        rx_pin_level = 0;// �͵�ƽֻ����һ�θô���
                        rx_data_count++;//����λ������
                        if((rx_ask_time_count <= ASK_DATA1_MAX) && (rx_ask_time_count >= ASK_DATA1_MIN))//����λ1 ���ո�λ
                        {
                            rx_Askbuf[rx_num] = rx_Askbuf[rx_num]<<1;
                            rx_Askbuf[rx_num] =rx_Askbuf[rx_num] |0x01;
                        }
                        else if((rx_ask_time_count <= ASK_DATA0_MAX) && (rx_ask_time_count >= ASK_DATA0_MIN))//����λ0
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
                        rx_ask_time_count = 0;//��ƽ��������0
                        if((rx_data_count % 8) == 0)//����8λ����ת��һ���ֽ�
                        {
                            rx_num++;
                        }
                        if(rx_data_count > ASK_BIT_LEN - 1)//�����޶�����ֽڣ��������ݽ�������������ʱ��
                        {
                                LED0_SET;
                                rx_succeed_num++;
                                for(i = 0;i<ASK_BIT_LEN/8;i++)
                                {
                                    AskReadbuf[i] = rx_Askbuf[i];
                                    printf("0x%x\t",rx_Askbuf[i]);
                                }
                                printf("\r\n");


                            rx_num = 0;//��־λ��������λ
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


