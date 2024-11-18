
#ifndef __REG_H__
#define __REG_H__
/*
typedef	unsigned long		ulong;
typedef	unsigned int		uint;
typedef	unsigned short		ushort;
typedef	unsigned char		uchar;
typedef	unsigned char		bool;
*/
//=============================================
//	Data type define
//=============================================
typedef bit bool;
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char UCHAR;
typedef unsigned int UINT;
typedef unsigned long ULONG;

#define INBUF_LEN 30 //数据长度

#define Logic_True              0x01
#define Logic_False             0x00

#define BIT0			(0x0001u)
#define BIT1			(0x0002u)
#define BIT2			(0x0004u)
#define BIT3			(0x0008u)
#define BIT4			(0x0010u)
#define BIT5			(0x0020u)
#define BIT6			(0x0040u)
#define BIT7			(0x0080u)
#define BIT8			(0x0100u)
#define BIT9			(0x0200u)
#define BIT10           	(0x0400u)
#define BIT11           	(0x0800u)
#define BIT12           	(0x1000u)
#define BIT13           	(0x2000u)
#define BIT14           	(0x4000u)
#define BIT15           	(0x8000u)


//Data list
#define DListVersionID 0
#define DListDOCH1     1
#define DListDOCH2     2
#define DListDOCH3     3
#define DListDOCH4     4
#define DListDOCH5     5
#define DListDOCH6     6
#define DListDOCH7     7
#define DListDOCH8     8
#define DListDOCH9     9
#define DListDOCH10    10
#define DListDeviceAddr   11
#define DListCommBaud     12

//time configuration
#define Time0CountMode()   TMOD|=0x04
#define Time0TimeMode()    TMOD&=~0x04




union Ldata
{
	long   Ldat;
	uchar  buf[4];
};
//*

struct DIDO
{
	unsigned int CHxx;
};


/*
sfr AUXR      = 0XE7;
//sfr stc flash
sfr ISP_DATA  = 0xE2;
sfr ISP_ADDRH = 0xE3;
sfr ISP_ADDRL = 0xE4;
sfr ISP_CMD   = 0xE5;
sfr ISP_TRIG  = 0xE6;
sfr ISP_CONTR = 0xE7;


//sbit SWRST   =ISP_CONTR^5;
//
sfr TM0D     =0x89;


***************************************
PCA sbit
***************************************
sfr CCON     =0XD8;
sfr CMOD     =0XD9;
sfr CL       =0XE9;
sfr CH       =0XF9;
sfr CCAP0L   =0XEA;
sfr CCAPM0   =0XDA;
sfr CCAPM1   =0XDB;

sfr CCAP1L   =0XEB;
sfr CCAP1H   =0XFB;




sbit EPCA_LVD=  IE^6;
sbit  CCF0   =CCON^0;
sbit  CCF1   =CCON^1;
sbit  CR     =CCON^6;
sbit  CF     =CCON^7;

****************************************
****************************************
sfr P1M0     =0X91;
sfr P1M1     =0X92;

sfr P2M0     =0X95;
sfr P2M1     =0X96;

sfr P3M0     =0xB1;
sfr P3M1     =0XB2;

sfr WDT_CONTR =0XE1;

//sbit RunLed=P2^7;
//sbit WDI   =P2^5;

sbit  P2_1=P2^1;

sbit DICH1=P2^2;
sbit DICH2=P2^3;
sbit DICH3=P3^2;
sbit DICH4=P3^3;
sbit DICH5=P3^4;
sbit DICH6=P3^5;
sbit DICH7=P2^4;
sbit DICH8=P2^5;
sbit DICH9=P2^6;
sbit DICH10=P2^7;


sbit DOCH1=P3^7;
sbit DOCH2=P1^0;
sbit DOCH3=P1^1;
sbit DOCH4=P1^2;
sbit DOCH5=P1^3;
sbit DOCH6=P1^4;
sbit DOCH7=P1^5;
sbit DOCH8=P1^6;
sbit DOCH9=P1^7;
sbit DOCH10=P2^0;
*/

/* After is STC additional SFR */

/* sfr  AUXR  = 0x8e; */
/* sfr  AUXR1 = 0xa2; */
/* sfr  IPH   = 0xb7; */

sfr  P4  = 0xc0;
sbit P47 = P4^7;
sbit P46 = P4^6;
sbit P45 = P4^5;
sbit P44 = P4^4;
sbit P43 = P4^3;
sbit P42 = P4^2;
sbit P41 = P4^1;
sbit P40 = P4^0;

sfr  XICON = 0xe8;

sfr  WDT_CONTR = 0xe1;

sfr ISP_DATA  = 0xe2;
sfr ISP_ADDRH = 0xe3;
sfr ISP_ADDRL = 0xe4;
sfr ISP_CMD   = 0xe5;
sfr ISP_TRIG  = 0xe6;
sfr ISP_CONTR = 0xe7;

/* Above is STC additional SFR */

/*--------------------------------------------------------------------------
REG51F.H

Header file for 8xC31/51, 80C51Fx, 80C51Rx+
Copyright (c) 1988-1999 Keil Elektronik GmbH and Keil Software, Inc.
All rights reserved.

Modification according to DataSheet from April 1999
 - SFR's AUXR and AUXR1 added for 80C51Rx+ derivatives
--------------------------------------------------------------------------*/

/*  BYTE Registers  */
sfr P0   = 0x80;
sfr P1   = 0x90;
sfr P2   = 0xA0;
sfr P3   = 0xB0;
sfr PSW  = 0xD0;
sfr ACC  = 0xE0;
sfr B    = 0xF0;
sfr SP   = 0x81;
sfr DPL  = 0x82;
sfr DPH  = 0x83;
sfr PCON = 0x87;
sfr TCON = 0x88;
sfr TMOD = 0x89;
sfr TL0  = 0x8A;
sfr TL1  = 0x8B;
sfr TH0  = 0x8C;
sfr TH1  = 0x8D;
sfr IE   = 0xA8;
sfr IP   = 0xB8;
sfr SCON = 0x98;
sfr SBUF = 0x99;

/*  80C51Fx/Rx Extensions  */
sfr AUXR   = 0x8E;
sfr AUXR1  = 0xA2;
sfr SADDR  = 0xA9;
sfr IPH    = 0xB7;
sfr SADEN  = 0xB9;
sfr T2CON  = 0xC8;
sfr T2MOD  = 0xC9;
sfr RCAP2L = 0xCA;
sfr RCAP2H = 0xCB;
sfr TL2    = 0xCC;
sfr TH2    = 0xCD;

/* PCA SFR
sfr CCON   = 0xD8;
sfr CMOD   = 0xD9;
sfr CCAPM0 = 0xDA;
sfr CCAPM1 = 0xDB;
sfr CCAPM2 = 0xDC;
sfr CCAPM3 = 0xDD;
sfr CCAPM4 = 0xDE;
sfr CL     = 0xE9;
sfr CCAP0L = 0xEA;
sfr CCAP1L = 0xEB;
sfr CCAP2L = 0xEC;
sfr CCAP3L = 0xED;
sfr CCAP4L = 0xEE;
sfr CH     = 0xF9;
sfr CCAP0H = 0xFA;
sfr CCAP1H = 0xFB;
sfr CCAP2H = 0xFC;
sfr CCAP3H = 0xFD;
sfr CCAP4H = 0xFE;
*/

/*  BIT Registers  */
/*  PSW   */
sbit CY   = PSW^7;
sbit AC   = PSW^6;
sbit F0   = PSW^5;
sbit RS1  = PSW^4;
sbit RS0  = PSW^3;
sbit OV   = PSW^2;
sbit P    = PSW^0;

/*  TCON  */
sbit TF1  = TCON^7;
sbit TR1  = TCON^6;
sbit TF0  = TCON^5;
sbit TR0  = TCON^4;
sbit IE1  = TCON^3;
sbit IT1  = TCON^2;
sbit IE0  = TCON^1;
sbit IT0  = TCON^0;

/*  IE   */
sbit EA   = IE^7;
sbit EC   = IE^6;
sbit ET2  = IE^5;
sbit ES   = IE^4;
sbit ET1  = IE^3;
sbit EX1  = IE^2;
sbit ET0  = IE^1;
sbit EX0  = IE^0;

/*  IP   */
/*	sbit PPC  = IP^6;*/
sbit PT2  = IP^5;
sbit PS   = IP^4;
sbit PT1  = IP^3;
sbit PX1  = IP^2;
sbit PT0  = IP^1;
sbit PX0  = IP^0;

/*  P3  */
sbit RD   = P3^7;
sbit WR   = P3^6;
sbit T1   = P3^5;
sbit T0   = P3^4;
sbit INT1 = P3^3;
sbit INT0 = P3^2;
sbit TXD  = P3^1;
sbit RXD  = P3^0;

/*  SCON  */
sbit SM0  = SCON^7; // alternatively "FE"
sbit FE   = SCON^7;
sbit SM1  = SCON^6;
sbit SM2  = SCON^5;
sbit REN  = SCON^4;
sbit TB8  = SCON^3;
sbit RB8  = SCON^2;
sbit TI   = SCON^1;
sbit RI   = SCON^0;

/*  P1  */
/* PCA
sbit CEX4 = P1^7;
sbit CEX3 = P1^6;
sbit CEX2 = P1^5;
sbit CEX1 = P1^4;
sbit CEX0 = P1^3;
sbit ECI  = P1^2;
*/

sbit T2EX = P1^1;
sbit T2   = P1^0;

/*  T2CON  */
sbit TF2   = T2CON^7;
sbit EXF2  = T2CON^6;
sbit RCLK  = T2CON^5;
sbit TCLK  = T2CON^4;
sbit EXEN2 = T2CON^3;
sbit TR2   = T2CON^2;
sbit C_T2  = T2CON^1;
sbit CP_RL2= T2CON^0;

/*  CCON  */
/*  PCA
sbit CF    = CCON^7;
sbit CR    = CCON^6;

sbit CCF4  = CCON^4;
sbit CCF3  = CCON^3;
sbit CCF2  = CCON^2;
sbit CCF1  = CCON^1;
sbit CCF0  = CCON^0;
*/

/**********************************************
//	Description: Head file of Battery monitor unit.
//	Author: Yanbin Zheng
//	Date: 2007-06-08
**********************************************/
/*
#ifndef __CONST_H__
#define __CONST_H__
*/

#define VERSION 103
//=============================================
//	I/O port define.
//=============================================
sbit CHSELCETEN = P0^6;
sbit ADDEVE1 = P0^0;
sbit ADDEVE2 = P0^1;
sbit ADDEVE3 = P0^2;
sbit ADDODD1 = P0^3;
sbit ADDODD2 = P0^4;
sbit ADDODD3 = P0^5;
sbit ADDSEGE1 = P2^7;
sbit ADDSEGE2 = P3^5;
sbit ADDSEGO1 = P2^6;
sbit ADDSEGO2 = P2^5;
sbit ADDSEGEN = P0^7;
sbit ADDPOLP = P2^1;
sbit ADDPOLN = P2^0;
sbit ADDPOLEN = P4^0;
sbit En485 		= P4^3;
sbit WDI 		= P3^2;
sbit LED_RUN 	= P4^4;
sbit RXD_RUN 	= P4^6;
sbit TXD_RUN 	= P4^1;
sbit TEMP_R 	= P3^6;
sbit TEMP_W 	= P3^7;

sbit ADDS1 = P2^4;
sbit ADDS2 = P2^3;
sbit ADDSEN = P2^2;
#define ADDUP	{ ADDS1 = 1;ADDS2=1;ADDSEN=0; }
#define ADDDOWN	{ ADDS1 = 0;ADDS2=0;ADDSEN=0; }
#define ADDDBETEEN	{ ADDS1 = 0;ADDS2=1;ADDSEN=0; }

//=============================================
//	Logic define
//=============================================
#define LOGIC_FALSE				( 0 )
#define LOGIC_TRUE				( 1 )
#define LOGIC_OK				( 0 )
#define LOGIC_FAIL				( 1 )
#define FALSE 	0
#define TRUE  	1

//=============================================
#define T2_20MS_INIT_VALUE		( 0xb7ff )		// 20ms
#define COUNT_MAX 				( 32767 )		// Max count
#define CHANNEL_MAX         	( 44 )			// Max channel
#define MAXTEMP                 ( 2 )
#define ASCII_ERR				( 255 )			// Err code for asc-->hex
//=============================================
//============================================
#define COM_MAX_LEN					( 96+12 )
#define UART_STATUS_RECEIVE			( 0 )
#define UART_STATUS_SEND			( 1 )
//============================================

#define EnableRunLed()			{ LED_RUN = 1; }
#define DisableRunLed() 		{ LED_RUN = 0; }
#define FlashRunLed()			{ LED_RUN = !LED_RUN; }
#define Set485Receive()			{ En485 = 0; }
#define Set485Send()			{ En485 = 1; }
#define WatchDog()				{ WDI = !WDI;}

typedef struct RomID_cfg
{
uint IDCount;
uchar RomID[MAXTEMP][8];
}ROMIDCfg,*LPROMIDCfg;

typedef struct Para_cfg
{
  unsigned long parainitflg;
  unsigned char gAddress;
  unsigned char baud;
}PARACfg,*LPPARACfg;

extern PARACfg  xdata para;

//#endif



//glode variable
extern unsigned char Timer2InterruptFlag;
extern union Ldata RegData;
extern struct DIDO RegDI;
extern struct DIDO RegDO;
//extern unsigned char DeviceAddr;
extern unsigned char CommBaud;
extern unsigned char DOStatusKeep;
extern unsigned int DICheckCountCH[10];
extern unsigned int DIStaAvailableTime;
extern unsigned char xdata WorkBuf[100];

//function assert
void main(void);
void IOinit(void);
//void delay(uint aa);
void delay_ms( unsigned char microsecond);
void int_0();
void timer_0();
void InterrProcess(void);
void SysInit(void);
//void Timer2Init(void);

//void WatchDogProcess(void);
//void DiCheck(void);
//void DoControl(void);
void quicksort(unsigned int a[], int l, int r);



#endif