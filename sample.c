#include <intrins.h>
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "uart.h"
#include "stcflash.h"
#include "sample.h"


unsigned char xdata SampleTime=0;
unsigned char xdata SampleTimeCnt =0;

unsigned char SampleCalcFlag = LOGIC_FALSE;

unsigned char xdata gCountH,gCountL;
unsigned char xdata gChannel = 0;

unsigned int xdata SampleTotalCout=0;
unsigned int xdata Samples[SP_TIMES+1];
unsigned int xdata gInputValue[CHANNEL_MAX+1];
float xdata ChannelVolageValue[CHANNEL_MAX+1];
float xdata ChannelAdjustValue[CHANNEL_MAX+1]={1.0};


void SampleProcess(void)
{
	unsigned char ch;
	if(gChannel!=0){
		//40001 通道1的数据存在 ChannelVolageValue[1]；
		ch = gChannel-1;
		ChannelVolageValue[ch]  = ((float)(gInputValue[ch]))*10;
		ChannelVolageValue[ch] /= gInputValue[44];
	}
	else{
		ch = CHANNEL_MAX;
		ChannelVolageValue[ch] = ((float)(gInputValue[ch]))*10;
		ChannelVolageValue[ch] *= 1.5;//AD620 OCCURE
		ChannelVolageValue[ch] /= RefVolageFre;
	}
}
//=============================================
//	Description: Select channel.
//	Para: channel.
//	return: none
//	Note:
//	Channel1,Channel3,
//=============================================
void SelectChannel( void )
{
	CHSELCETEN=0;
	ADDSEGEN=1;
	ADDSEN=1;
	ADDPOLEN=1;
  switch(gChannel)
  {
	  case 1:
			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=0;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=1;
			ADDEVE3=1;
			//双数选择
			ADDODD1=1;
			ADDODD2=1;
			ADDODD3=1;
    	break;
	  case 2:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=0;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=1;
			ADDEVE3=1;
			//双数选择
			ADDODD1=1;
			ADDODD2=1;
			ADDODD3=1;
    	break;
	  case 3:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=0;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=1;
			ADDEVE3=1;
			//双数选择
			ADDODD1=0;
			ADDODD2=1;
			ADDODD3=1;
		  break;
	  case 4:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=0;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=0;
			ADDEVE3=1;
			//双数选择
			ADDODD1=0;
			ADDODD2=1;
			ADDODD3=1;
    	break;
	  case 5:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=0;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=0;
			ADDEVE3=1;
			//双数选择
			ADDODD1=1;
			ADDODD2=0;
			ADDODD3=1;
		  break;
	  case 6:
			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=0;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=0;
			ADDEVE3=1;
			//双数选择
			ADDODD1=1;
			ADDODD2=0;
			ADDODD3=1;
		  break;
	  case 7:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=0;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=0;
			ADDEVE3=1;
			//双数选择
			ADDODD1=0;
			ADDODD2=0;
			ADDODD3=1;
	    break;
	  case 8:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=0;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=1;
			ADDEVE3=0;
			//双数选择
			ADDODD1=0;
			ADDODD2=0;
			ADDODD3=1;
		  break;
	  case 9:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=0;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=1;
			ADDEVE3=0;
			//双数选择
			ADDODD1=1;
			ADDODD2=1;
			ADDODD3=0;
    	break;
	  case 10:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=0;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=1;
			ADDEVE3=0;
			//双数选择
			ADDODD1=1;
			ADDODD2=1;
			ADDODD3=0;
    	break;
	  case 11:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=0;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=1;
			ADDEVE3=0;
			//双数选择
			ADDODD1=0;
			ADDODD2=1;
			ADDODD3=0;
	    break;
	  case 12:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=0;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=0;
			ADDEVE3=0;
			//双数选择
			ADDODD1=0;
			ADDODD2=1;
			ADDODD3=0;
    	break;
	  case 13:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=0;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=0;
			ADDEVE3=0;
			//双数选择
			ADDODD1=1;
			ADDODD2=0;
			ADDODD3=0;
	    break;
	  case 14:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=0;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=0;
			ADDEVE3=0;
			//双数选择
			ADDODD1=1;
			ADDODD2=0;
			ADDODD3=0;
	    break;
	  case 15:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=0;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=0;
			ADDEVE3=0;
			//双数选择
			ADDODD1=0;
			ADDODD2=0;
			ADDODD3=0;
	    break;
	  case 16://P21

			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=1;
			ADDSEGO2=0;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=1;
			ADDEVE3=1;
			//双数选择
			ADDODD1=0;
			ADDODD2=0;
			ADDODD3=0;
	    break;
	  case 17://P22

			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=1;
			ADDEVE3=1;
			//双数选择
			ADDODD1=1;
			ADDODD2=1;
			ADDODD3=1;
	    break;
	  case 18:

			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=1;
			ADDEVE3=1;
			//双数选择
			ADDODD1=1;
			ADDODD2=1;
			ADDODD3=1;
	    break;
	  case 19:

			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=1;
			ADDEVE3=1;
			//双数选择
			ADDODD1=0;
			ADDODD2=1;
			ADDODD3=1;
	    break;
	  case 20:

			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=0;
			ADDEVE3=1;
			//双数选择
			ADDODD1=0;
			ADDODD2=1;
			ADDODD3=1;
	    break;
	  case 21:

			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=0;
			ADDEVE3=1;
			//双数选择
			ADDODD1=1;
			ADDODD2=0;
			ADDODD3=1;
	    break;
	  case 22:

			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=0;
			ADDEVE3=1;
			//双数选择
			ADDODD1=1;
			ADDODD2=0;
			ADDODD3=1;
	    break;
	  case 23:

			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDUP;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=0;
			ADDEVE3=1;
			//双数选择
			ADDODD1=0;
			ADDODD2=0;
			ADDODD3=1;
	    break;
	  case 24:
			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDBETEEN;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=1;
			ADDEVE3=0;
			//双数选择
			ADDODD1=0;
			ADDODD2=0;
			ADDODD3=1;
	    break;
		case 25:

			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=1;
			ADDEVE3=0;
			//双数选择
			ADDODD1=1;
			ADDODD2=1;
			ADDODD3=0;
	    break;
		case 26:

			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=1;
			ADDEVE3=0;
			//双数选择
			ADDODD1=1;
			ADDODD2=1;
			ADDODD3=0;
	    break;
	  case 27:

			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=1;
			ADDEVE3=0;
			//双数选择
			ADDODD1=0;
			ADDODD2=1;
			ADDODD3=0;
	    break;
	  case 28:
			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=0;
			ADDEVE3=0;
			//双数选择
			ADDODD1=0;
			ADDODD2=1;
			ADDODD3=0;
	    break;
	  case 29:

			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=0;
			ADDEVE3=0;
			//双数选择
			ADDODD1=1;
			ADDODD2=0;
			ADDODD3=0;
	    break;
	  case 30:

			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=0;
			ADDEVE3=0;
			//双数选择
			ADDODD1=1;
			ADDODD2=0;
			ADDODD3=0;
	    break;
	  case 31:

			//使能选择
			ADDSEGE1=0;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=0;
			ADDEVE3=0;
			//双数选择
			ADDODD1=0;
			ADDODD2=0;
			ADDODD3=0;
	    break;
	  case 32://P32

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=1;
			ADDSEGO1=0;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=1;
			ADDEVE3=1;
			//双数选择
			ADDODD1=0;
			ADDODD2=0;
			ADDODD3=0;
	    break;
	  case 33:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=1;
			ADDSEGO1=1;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=1;
			ADDEVE3=1;
			//双数选择
			ADDODD1=1;
			ADDODD2=1;
			ADDODD3=1;
	    break;
	  case 34:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=1;
			ADDSEGO1=1;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=1;
			ADDEVE3=1;
			//双数选择
			ADDODD1=1;
			ADDODD2=1;
			ADDODD3=1;
	    break;
	  case 35:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=1;
			ADDSEGO1=1;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=1;
			ADDEVE3=1;
			//双数选择
			ADDODD1=0;
			ADDODD2=1;
			ADDODD3=1;
	    break;
	  case 36:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=1;
			ADDSEGO1=1;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=0;
			ADDEVE3=1;
			//双数选择
			ADDODD1=0;
			ADDODD2=1;
			ADDODD3=1;
	    break;
	  case 37:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=1;
			ADDSEGO1=1;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=0;
			ADDEVE3=1;
			//双数选择
			ADDODD1=1;
			ADDODD2=0;
			ADDODD3=1;
	    break;
	  case 38:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=1;
			ADDSEGO1=1;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=0;
			ADDEVE3=1;
			//双数选择
			ADDODD1=1;
			ADDODD2=0;
			ADDODD3=1;
	    break;
	  case 39:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=1;
			ADDSEGO1=1;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=0;
			ADDEVE3=1;
			//双数选择
			ADDODD1=0;
			ADDODD2=0;
			ADDODD3=1;
	    break;
	  case 40:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=1;
			ADDSEGO1=1;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=0;
			ADDPOLN=1;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=1;
			ADDEVE3=0;
			//双数选择
			ADDODD1=0;
			ADDODD2=0;
			ADDODD3=1;
	    break;
	  case 41:

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=1;
			ADDSEGO1=1;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=1;
			ADDEVE3=0;
			//双数选择
			ADDODD1=1;
			ADDODD2=1;
			ADDODD3=0;
	    break;
	  case 42:	//总电压

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=1;
			ADDSEGO1=1;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=1;
			ADDEVE3=0;
			//双数选择
			ADDODD1=0;
			ADDODD2=1;
			ADDODD3=0;
	    break;
	  case 43:	//模拟输入

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=1;
			ADDSEGO1=1;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=1;
			ADDEVE2=0;
			ADDEVE3=0;
			//双数选择
			ADDODD1=1;
			ADDODD2=0;
			ADDODD3=0;
	    break;
	  case 44:		//VREF

			//使能选择
			ADDSEGE1=1;
			ADDSEGE2=1;
			ADDSEGO1=1;
			ADDSEGO2=1;
			ADDSEGEN=0;
			//段选择
			ADDDOWN;
			//正负选择
			ADDPOLP=1;
			ADDPOLN=0;
			ADDPOLEN=0;
			//单数选择
			ADDEVE1=0;
			ADDEVE2=0;
			ADDEVE3=0;
			//双数选择
			ADDODD1=0;
			ADDODD2=0;
			ADDODD3=0;
    	break;
    case 0:
		  break;
	  default:
    	break;
    }
	CHSELCETEN=1;
}
