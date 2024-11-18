/*
  1.Project Name:Enegy meter;
  2.MAIN MCU    :stc12c5410AD;
  3.function    : display and LED direct;
                  read and seting CS5460;
                  Key input;
*/

//#include <REG52.H>
#include <intrins.h>
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "uart.h"
#include "stcflash.h"
#include "sample.h"


unsigned char Timer2InterruptFlag;
union Ldata RegData;
struct DIDO RegDI;
struct DIDO RegDO;

unsigned char DOStatusKeep=0;
unsigned int DICheckCountCH[10]={0};
unsigned int DIStaAvailableTime=0;

unsigned char xdata WorkBuf[100];

unsigned char xdata runledcount=0;
PARACfg xdata para;




void main (void)
{
	uint i;
	/*close whole interrupt*/
	EA=0;
	//RunLed=0;
	/*Close Watchdog*/
	//WDT_CONTR =0X3C;
	IOinit();
	//DataToDisplay(0x8888);
	//displaydri(0);
	i=100;
	/*wait 1.74s*/
	do {
		delay_ms(2);
	}while(--i);

	/*interrupt init*/
	//IT0=0;
	//EX0=1;
	//Timer Config;
	SysInit();
	//Timer2Init();

	//stop();
	//RST_Para();
	StcResetCheck();
	//GetDListStartAddressPoweron();
	//DataInitFromStcflash();

	RomInit();
	init_serialcomm(); //初始化串口
	//CommBaud=Baud19200;
	BPSset();
	//PCA_Init();
	//end of;

	/*open whole interrupt*/
	TF0=0;
	IE0=0;

	/*
	CCF0=0;
	CCF1=0;
	*/
	//Int0InterruptFlag=0;
	Timer2InterruptFlag=0;
	EA=1;

	while(1){
		_nop_();
		InterrProcess();
	}
}

void IOinit (void)
{

}

void delay_ms (unsigned char microsecond)
{
	unsigned char ll;
	for (; microsecond > 0; microsecond--) {
		for (ll = 0; ll<55; ll++) {
		}
	}
}

/*************************
port a interrupt entry
*************************/
void int_0() interrupt 0
{

}

//timer 0 interrupt entry
void timer_0() interrupt 1
{
	//IntWorkStaByte=2;
}

unsigned int xdata sl[SP_TIMES], sr[SP_TIMES];
void quicksort(unsigned int a[], int l, int r)
{
    static int sp=0;
    #define PUSH(A,B) {sl[sp]=A;sr[sp]=B;sp++;}
    #define POP(A,B) {sp--;A=sl[sp];B=sr[sp];}

    int i=0, j=0, p=0, t=0;
    sp=0;
    PUSH(l,r);
    while(sp) {
        POP(l,r);
        i=l;j=r;p=a[(i+j)/2];
        while(i<=j) {
            while(a[i]<p)i++;
            while(a[j]>p)j--;
            if(i<=j) {
                t=a[i];a[i]=a[j];a[j]=t;
                i++;j--;
            }
        }
        if(l<j)PUSH(l,j);
        if(i<r)PUSH(i,r);
    }   
}

void Deal_Sample(void)
{
    static unsigned int cnt=0;
    unsigned char i=0;

    Samples[cnt++] = SampleTotalCout;

    if (cnt>=SP_TIMES) {
        quicksort(Samples, 0, SP_TIMES-1);
        gInputValue[gChannel] = (Samples[4]+Samples[5])*0.5f;

        gChannel++;
        if( gChannel > CHANNEL_MAX ) {
            gChannel = 0;
        }
        SelectChannel();
        TL0 =0;
        TH0 =0;
        SampleCalcFlag = LOGIC_TRUE;
        cnt = 0;
    }
}

void Timer2INT( void ) interrupt 5
{
	TF2 = 0;
	TL2 = ( T2_20MS_INIT_VALUE & 0x00FF );
	TH2 = ( T2_20MS_INIT_VALUE >> 8 );
	Timer2InterruptFlag=1;
	SampleTime++;

	if (SampleTime >=5) { // 100ms
		SampleTime =0;
		//TR0 = 0;
		SampleTotalCout = (unsigned int)(TH0* 256 + TL0);
		TL0 =0;
		TH0 =0;
		Deal_Sample();
	}
}

void InterrProcess (void)
{
	if (Timer2InterruptFlag==1) {
		Timer2InterruptFlag=0;
		if (SampleCalcFlag == LOGIC_TRUE){
			SampleCalcFlag = LOGIC_FALSE;
			SampleProcess();
		}

		UartDataProcess();
		WatchDog();
		runledcount++;
		if (runledcount >= 25) {
			runledcount=0;
			FlashRunLed();
		}
	}
}

void SysInit (void)
{
	unsigned char i;
	//DisableRunLed();
	//time1 init
	TMOD = 0x25; 		// T0 as 16bit counter T1 for bandrate
	TR0 = 0;
	TL0 = 0;
	TH0 = 0;
	TR0 = 1;
	//time2 for time
	T2CON = 0;			// T2 autoload timer.
	TR2 = 0;
	TL2 = ( T2_20MS_INIT_VALUE & 0x00FF );
	TH2 = ( T2_20MS_INIT_VALUE >> 8 );
	RCAP2L = ( T2_20MS_INIT_VALUE & 0x00FF );
	RCAP2H = ( T2_20MS_INIT_VALUE >> 8 );
	TR2 = 1;
	ET2 = 1;

	for(i=0;i<CHANNEL_MAX+1;i++){
		ChannelAdjustValue[i] =1;
	}


}





