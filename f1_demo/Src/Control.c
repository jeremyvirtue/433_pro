//
// Created by Administrator on 2020/6/28.
//

#include "Control.h"
#include "KeyBoard.h"
#include "Tx433Lin.h"
#include "Tx433Remote.h"
#include "keeloq_lock.h"
#include "stdio.h"
#include "Rx433Remote.h"
extern  SYSTEM_DATA sdat;


void KeyMenu(void)
{
    u32 pas;
    u8 i;
   Rx433IrDispose();
    switch(keys.key_num) {
        case K_SET_P:
            keys.key_num = 0;
//            pas = 0x1010101;
//            printf("HOPE_CODE = 0x%x\r\n",pas);
//            pas = CRYPT(pas);
//            printf("HOPE_CODE = 0x%x\r\n",pas);
//            pas  = DECRYPT(pas);
//            printf("HOPE_DECODE = 0x%x\r\n",pas);
//            printf("\r\n");

//            AskOpen(2);
//            LED0_SET;
//            Tx433IrOpen(5);
//             AskSendData(5, ASK_START);
//            for(i =0;i<4;i++){
//                ask_buf[i]++;
//            }
            AskOpen(5);
        case K_SET_R:
            keys.key_num = 0;
            LED0_RESET;
            break;
        case K_SET_C:
            keys.key_num = 0;
            break;

    }
}



void Rx433LinCtrl(){

}



