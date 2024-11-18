//#include <REG52.H>
#include <intrins.h>
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "uart.h"
#include "stcflash.h"
#include "sample.h"


/* 打开 ISP,IAP 功能 */
void ISP_IAP_enable(void)
{
	//EA	=	0;	// 关中断
	ISP_CONTR	=	ISP_CONTR & 0x18;       // 0001,1000
	ISP_CONTR	=	ISP_CONTR | WAIT_TIME;
	ISP_CONTR	=	ISP_CONTR | 0x80;       // 1000,0000
}

/* 关闭 ISP,IAP 功能 */
void ISP_IAP_disable(void)
{
	ISP_CONTR	=	ISP_CONTR & 0x7f;	// 0111,1111
	ISP_TRIG	=	0x00;
	//EA			=   1;                	// 开中断
}
void StcResetCheck(void)
{
	unsigned char i;
	/**************************
	Test MCU reset stats;
	***************************/
	ISP_IAP_enable();    //打开IAP功能，设置Flash操作等待时间
  ISP_CMD = 0x01;                //选择读AP模式
  //--------------------------
  ISP_ADDRH = (uchar)(ADDRADJUST>>8);      //填页地址
  ISP_ADDRL = (uchar)(ADDRADJUST);      //填页地址
  //EA = 0;
  ISP_TRIG = 0x46;      //触发ISP处理器
  ISP_TRIG = 0xB9;
  i = ISP_DATA;
  if(ISP_CONTR&0x10){
  	/*CMD_FAIL to reset system*/
    ISP_CONTR=0x28;
  }
  else{
    ISP_IAP_disable();
  }
}
/* 字节读 */
uchar byte_read(uint byte_addr)
{
	ISP_ADDRH	=	(INT8U)(byte_addr >> 8);
	ISP_ADDRL	=	(INT8U)(byte_addr & 0x00ff);

	ISP_CMD		=	ISP_CMD	&	0xf8;        // 1111,1000
	ISP_CMD		=	ISP_CMD	|	READ_AP_and_Data_Memory_Command;        // 0000,0001

	ISP_IAP_enable();

	ISP_TRIG	=	0x46;
	ISP_TRIG	=	0xb9;
	_nop_();

	ISP_IAP_disable();
	return (ISP_DATA);
}
//------------------------------------------------------
//功能：擦除扇区
//入口：uint address＝页地址0～512，为了提高处理速度，最好用0~256的范围
//出口：
//设计：郭准，伟业，2006/2/7
//------------------------------------------------------
/*
void Sector_Erase(uint address)
{
  ISP_IAP_enable();    //打开IAP功能，设置Flash操作等待时间
  ISP_CMD = 0x03;                //选择页擦除模式
  //--------------------------

  ISP_ADDRH = (uchar)(address>>8);      //填页地址
  ISP_ADDRL = (uchar)(address);      //填页地址
  //EA = 0;
  ISP_TRIG = 0x46;      //出发ISP处理器
  ISP_TRIG = 0xB9;
      _nop_();
  //EA = 1;
  //关闭IAP功能,清与ISP有关的特殊功能寄存器
  ISP_CONTR = 0;
  ISP_CMD = 0;
  ISP_TRIG = 0;
}
*/
//------------------------------------------------------
//功能：字节编程，写
//入口：uint address＝页地址0～512，为了提高处理速度，
//      最好用0~256的范围;uchar ch="要写的数据"
//出口：
//设计：郭准，伟业，2006/2/7
//------------------------------------------------------
/*
void Byte_Program(uint address,uchar ch)
{
  //   Sector_Erase(address);
  ISP_IAP_enable();    //打开IAP功能，设置Flash操作等待时间
  ISP_CMD = 0x02;                //选择字节编程模式
  //--------------------------
  ISP_ADDRH = (uchar)(address>>8);      //填页地址
  ISP_ADDRL = (uchar)(address);      //填页地址
  ISP_DATA = ch;
  //EA = 0;
  ISP_TRIG = 0x46;      //触发ISP处理器
  ISP_TRIG = 0xB9;
      _nop_();

   // EA = 1;
  //-关闭IAP功能,清与ISP有关的特殊功能寄存器
  ISP_CONTR = 0;
  ISP_CMD = 0;
  ISP_TRIG = 0;
}
*/
/* 写数据进 数据Flash存储器, 只在同一个扇区内写，不保留原有数据	*/
/* begin_addr,被写数据Flash开始地址；counter,连续写多少个字节； array[]，数据来源	*/
void eep_write_sector(uint begin_addr, uint counter, uchar array[])
{
	uint	i	=	0;
	uint	in_sector_begin_addr	=	0;
	uint	sector_addr	=	0;
  /*
	// 判是否是有效范围,此函数不允许跨扇区操作
	if(counter > USED_BYTE_QTY_IN_ONE_SECTOR)
		return	ERROR;
	in_sector_begin_addr =        begin_addr & 0x01ff;         // 0000,0001,1111,1111
	if( (in_sector_begin_addr + counter) > USED_BYTE_QTY_IN_ONE_SECTOR )
		return ERROR;
	*/

    // 擦除 要修改/写入 的扇区
  ISP_IAP_enable();  //2012-12-14 18:06
	sector_addr	=	(begin_addr & 0xfe00);	// 1111,1110,0000,0000; 取扇区地址
	ISP_ADDRH	=	(INT8U)(sector_addr >> 8);
	ISP_ADDRL	=	0x00;
	ISP_CMD		=	ISP_CMD	&	0xf8;		// 1111,1000
	ISP_CMD		=	ISP_CMD	|	SECTOR_ERASE_AP_and_Data_Memory_Command;	// 0000,0011

	ISP_IAP_enable();
	ISP_TRIG	=	0x46;        // 触发ISP_IAP命令
	ISP_TRIG	=	0xb9;        // 触发ISP_IAP命令
	_nop_();

	for(i = 0; i< counter; i++)
	{
		// 写一个字节
		ISP_ADDRH	=	(INT8U)(begin_addr >> 8);
		ISP_ADDRL	=	(INT8U)(begin_addr & 0x00ff);
		ISP_DATA	=	array[i];
		ISP_CMD		=	ISP_CMD	&	0xf8;	// 1111,1000
		ISP_CMD		=	ISP_CMD	|	PROGRAM_AP_and_Data_Memory_Command;		// 0000,0010

		ISP_TRIG	=	0x46;	// 触发ISP_IAP命令
		ISP_TRIG	=	0xb9;	// 触发ISP_IAP命令
		_nop_();

		/*
		// 读回来
		ISP_DATA	=	0x00;

		ISP_CMD		=	ISP_CMD	&	0xf8;	// 1111,1000
		ISP_CMD     =	ISP_CMD	|	READ_AP_and_Data_Memory_Command;	// 0000,0001

		ISP_TRIG	=	0x46;	// 触发ISP_IAP命令
		ISP_TRIG	=	0xb9;	// 触发ISP_IAP命令
		_nop_();

		//  比较对错
		if(ISP_DATA != array[i])
		{
			ISP_IAP_disable();
			return ERROR;
		}
		*/
    begin_addr++;
	}
	ISP_IAP_disable();
	//return	OK;
}
void RdFromROM(uint begin_addr, uint counter, uchar array[])//reentrant
{
	unsigned char i;
	for(i=0;i<counter;i++)
	{
		array[i]=byte_read(begin_addr+i);
		//delay_ms(2);
	}
}
/*
void DataInitFromStcflash(void)
{

	unsigned int stcflashaddr,i;
	//AUXR |=0X02;
  RegDO.CHxx=0;



  stcflashaddr = ADDRADJUST+StcFlashSec1Add;
  Moreyte_Read(stcflashaddr,DListTotalLength,WorkBuf);
  for(i=0; i<4; i++){
  	RegData.buf[i] = WorkBuf[DListVersionIDAddr+i];
  }
  if(RegData.Ldat == EEPROMCHECK){
  	//get data
  	DeviceAddr=WorkBuf[DListDeviceAddrAddr];
  	CommBaud=WorkBuf[DListCommBaudAddr];
  	DOStatusKeep=WorkBuf[DListRelayStatusKeep];
    if(DOStatusKeep==Logic_True){
    	i=WorkBuf[DListDOStatusAddr];
      RegDO.CHxx =0;
    	RegDO.CHxx |= i<<8;
    	i=WorkBuf[DListDOStatusAddr+1];
    	RegDO.CHxx |= i;
    }
    //
    DIStaAvailableTime =0;
    DIStaAvailableTime |= ((uint)WorkBuf[DListDIStaAvailableTime])<<8;
    DIStaAvailableTime |= ((uint)WorkBuf[DListDIStaAvailableTime+1]);
    if((DIStaAvailableTime<1)||(DIStaAvailableTime>500)){
    	DIStaAvailableTime = 1;
    }



    //
  }
  else{
  	//RunLed=0;
  	//erase flash
  	Sector_Erase(stcflashaddr);
  	//write data to flash
  	RegData.Ldat= EEPROMCHECK;
  	for(i=0; i<4; i++){
  	   WorkBuf[DListVersionIDAddr+i] = RegData.buf[i];
    }
    DeviceAddr=1;
    CommBaud = Baud9600;
    DOStatusKeep=Logic_True;//keep
    RegDO.CHxx= 0;
    WorkBuf[DListDeviceAddrAddr]=DeviceAddr;
  	WorkBuf[DListCommBaudAddr]=CommBaud;
  	WorkBuf[DListRelayStatusKeep]=DOStatusKeep;
  	if(DOStatusKeep==Logic_True){
  	  WorkBuf[DListDOStatusAddr]=(RegDO.CHxx>>8)&0xff;
  	  WorkBuf[DListDOStatusAddr+1]=RegDO.CHxx&0xff;
  	}
  	//
  	DIStaAvailableTime =1;
  	WorkBuf[DListDIStaAvailableTime]=(DIStaAvailableTime>>8)&0xff;
  	WorkBuf[DListDIStaAvailableTime+1]=DIStaAvailableTime&0xff;
  	//
  	delay(600);
  	MoreByteProg(stcflashaddr,DListTotalLength,WorkBuf);
  }
}
*/
/*
void SaveDatToStcflash(unsigned char SaveDatFlag)
{
	unsigned int stcflashaddr;
  stcflashaddr = ADDRADJUST+StcFlashSec1Add;
  if(SaveDatFlag == Logic_True){
  	SaveDatFlag = Logic_False;
  	//erase flash
  	Sector_Erase(stcflashaddr);
  	//write data to flash
    WorkBuf[DListDeviceAddrAddr]=DeviceAddr;
  	WorkBuf[DListCommBaudAddr]=CommBaud;
  	WorkBuf[DListRelayStatusKeep]=DOStatusKeep;
    if(DOStatusKeep==Logic_True){
  	  WorkBuf[DListDOStatusAddr]=(RegDO.CHxx>>8)&0xff;
  	  WorkBuf[DListDOStatusAddr+1]=RegDO.CHxx&0xff;
    }
    //
    WorkBuf[DListDIStaAvailableTime]=(DIStaAvailableTime>>8)&0xff;
  	WorkBuf[DListDIStaAvailableTime+1]=DIStaAvailableTime&0xff;
  	//
  	delay(600);
  	MoreByteProg(stcflashaddr,DListTotalLength,WorkBuf);
  }
}
*/
//=======================================================
void RdPara(void)
{
  RdFromROM(ADDRPARA,sizeof(para),(unsigned char *)&para);
  if(para.gAddress>255)
  	para.gAddress=1;
  if(para.baud>4)
  	para.baud=4;
}
void WrPara(void)
{
  eep_write_sector(ADDRPARA,sizeof(para),(unsigned char *)&para);
}

void WrAdjust(void)
{
  eep_write_sector(ADDRADJUST,sizeof(ChannelAdjustValue),(unsigned char *)&ChannelAdjustValue);
}
void RdAdjust(void)
{
	unsigned char i;
	RdFromROM(ADDRADJUST,sizeof(ChannelAdjustValue),(unsigned char *)&ChannelAdjustValue);
	  for(i=0;i<(CHANNEL_MAX+1);i++)
		{
		  if((ChannelAdjustValue[i]>1.200)||(ChannelAdjustValue[i]<0.800)){
		    ChannelAdjustValue[i]=1.0;
			}
		}
}

void RomInit(void)
{
  unsigned char i;
  RdPara();
  if(para.parainitflg != EEPROMCHECK){
  	para.baud=3;
  	para.gAddress=1;
  	para.parainitflg= EEPROMCHECK;
  	WrPara();
  	for(i=0;i<(CHANNEL_MAX+1);i++){
  		ChannelAdjustValue[i]=1.0;
  	}
  	WrAdjust();
  	/*
  	TempID.IDCount=0;
  	WrRomID();
  	*/
	}
  else{
  	RdAdjust();
	  //RdRomID();
	}
}