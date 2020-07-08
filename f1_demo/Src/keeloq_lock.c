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

/*加密*/
void RrcHOPE(){
    if(key_dat!=0){
        HOPE=(HOPE>>1)|0x80000000;
    }else{
        HOPE=(HOPE>>1)&0x7fffffff;
    }
}

/*解码时*/
void RlcHOPE(){
    if(key_dat!=0){
        HOPE=(HOPE<<1)|1;
    }else{
        HOPE=(HOPE<<1)&0xFFFFFFFE;
    }
}

/*运行大概6ms stm32f103c8t6*/
u32 CRYPT(u32 dat) {
    //key=SERX;
//    HOPE = dat;
    u32 getdat;
    getdat = dat;
    for (int i = 0; i < 528; i++) {
        int nlf=NLF[getBit(getdat, 31)][getBit(getdat, 26)][getBit(getdat, 20)][getBit(getdat, 9)][getBit(getdat, 1)];
        int y16=getBit(getdat, 16);
        int y0=getBit(getdat, 0);
        int k=getBit(key_lock, i%64);
        int result=nlf^y16^y0^k;
        if (result!=0) {
            key_dat=1;
        }else {
            key_dat=0;
        }

        if(key_dat!=0){
            getdat=(getdat>>1)|0x80000000;
        }else{
            getdat=(getdat>>1)&0x7fffffff;
        }

    }
    return getdat;
}

/*解密*/
u32 DECRYPT(u32 dat) {
//    key=key_lock;
    for (int i = 528; i > 0; i--) {
        int nlf = NLF[getBit(dat, 30)][getBit(dat, 25)][getBit(dat, 19)][getBit(dat, 8)][getBit(dat, 0)];
        int y15 = getBit(dat, 15);
        int y31 = getBit(dat, 31);
        int k = getBit(key_lock, (i - 1) % 64);
        int result = nlf ^y15 ^y31 ^k;
        if (result != 0) {
            key_dat = 1;
        } else {
            key_dat = 0;
        }

        if(key_dat!=0){
            dat=(dat<<1)|1;
        }else{
            dat=(dat<<1)&0xFFFFFFFE;
        }
    }
    return dat;
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

