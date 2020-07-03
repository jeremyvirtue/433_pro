//
// Created by Administrator on 2020/6/30.
//

#include "keeloq_lock.h"

unsigned long long int  key_lock=0xefcdab2143658709;
int NLF[2][2][2][2][2];
unsigned int HOPE;
unsigned int key_dat=0;

int  getBit(unsigned long long int source,int n) {
    unsigned long long int temp0=((long long int) 1<<n);
    unsigned long long int temp1=source&temp0;
    if ( temp1 != 0) {
        return 1;
    }
    return 0;
}

void RrcHOPE(){
    if(key_dat!=0){
        HOPE=(HOPE>>1)|0x80000000;
    }else{
        HOPE=(HOPE>>1)&0x7fffffff;
    }
}

/*‘À––¥Û∏≈6ms stm32f103c8t6*/
void CRYPT(u16 dat) {
    //key=SERX;
    HOPE = dat;
    for (int i = 0; i < 528; i++) {
        int nlf=NLF[getBit(HOPE, 31)][getBit(HOPE, 26)][getBit(HOPE, 20)][getBit(HOPE, 9)][getBit(HOPE, 1)];
        int y16=getBit(HOPE, 16);
        int y0=getBit(HOPE, 0);
        int k=getBit(key_lock, i%64);
        int result=nlf^y16^y0^k;
        if (result!=0) {
            key_dat=1;
        }else {
            key_dat=0;
        }

        RrcHOPE();
    }
}

void NLF_DEFINE(){
    NLF[0][0][0][0][0]=0;
    NLF[0][0][0][0][1]=1;
    NLF[0][0][0][1][0]=1;
    NLF[0][0][0][1][1]=1;
    NLF[0][0][1][0][0]=0;
    NLF[0][0][1][0][1]=1;
    NLF[0][0][1][1][0]=0;
    NLF[0][0][1][1][1]=0;

    NLF[0][1][0][0][0]=0;
    NLF[0][1][0][0][1]=0;
    NLF[0][1][0][1][0]=1;
    NLF[0][1][0][1][1]=0;
    NLF[0][1][1][0][0]=1;
    NLF[0][1][1][0][1]=1;
    NLF[0][1][1][1][0]=1;
    NLF[0][1][1][1][1]=0;

    NLF[1][0][0][0][0]=0;
    NLF[1][0][0][0][1]=0;
    NLF[1][0][0][1][0]=1;
    NLF[1][0][0][1][1]=1;
    NLF[1][0][1][0][0]=1;
    NLF[1][0][1][0][1]=0;
    NLF[1][0][1][1][0]=1;
    NLF[1][0][1][1][1]=0;

    NLF[1][1][0][0][0]=0;
    NLF[1][1][0][0][1]=1;
    NLF[1][1][0][1][0]=0;
    NLF[1][1][0][1][1]=1;
    NLF[1][1][1][0][0]=1;
    NLF[1][1][1][0][1]=1;
    NLF[1][1][1][1][0]=0;
    NLF[1][1][1][1][1]=0;
}

