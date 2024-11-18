
//#include <REG52.H>
#include <intrins.h>
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "uart.h"
#include "stcflash.h"
#include "sample.h"




/*******************************************
*串口0 对应变量
********************************************/
unsigned char Uart_Rx_Cnt=0;			//串口接收数据计数
unsigned int Uart_Tx_Cnt=0;			//发送数据长度
unsigned int Uart_Rx_Length=0;		//接收数据包长度
unsigned int Uart_Tx_Length=0;		//发送数据包长度
unsigned char xdata Uart_Rx_Buf[100];
unsigned char xdata Uart_Tx_Buf[100];
unsigned char Com0_Pack_Error=0x00;
unsigned char Uart_Length_Flag=0;
unsigned char Uart_Rx_Flag=0;
//unsigned int Constant_Angle=540;

unsigned char Uart_Read_Flag=0;
unsigned char Uart_Write_Flag=0;
//unsigned int icount = 0;//leo add
unsigned char CRC16flag=0;


//
void init_serialcomm( void )
{
	TR1 = 0 ; // timer 1 run
	SCON = 0x50 ; //SCON: serail mode 1, 8-bit UART, enable ucvr; 1111
  //TMOD |= 0x20 ; //TMOD: timer 1, mode 2, 8-bit reload
  PCON = 0 ; //SMOD=0;
  //TH1=0xFD; //9600  //TH1 = 0xF4 ; //Baud:4800 fosc=11.0592MHz
  ES  =1 ; //Enable Serial Interrupt
  TR1 = 1 ; // timer 1 run
  /*
  SCON = 0x70 ; //SCON: serail mode 1, 8-bit UART, enable ucvr; 1111
  TMOD |= 0x20 ; //TMOD: timer 1, mode 2, 8-bit reload
  PCON |= 0x80 ; //SMOD=1;
  //TH1=0xFD; //9600  //TH1 = 0xF4 ; //Baud:4800 fosc=11.0592MHz
  ES  =1 ; //Enable Serial Interrupt
  TR1 = 1 ; // timer 1 run
  */
  ResetSerialToRxd();
  // TI=1;
}

//串口接收中断函数
void serial () interrupt 4
{

  if(RI)
  {
    RI = 0 ;

    RXD_RUN ^= 1;
    Uart_Rx_Buf[Uart_Rx_Cnt]= SBUF;
    Uart_Rx_Cnt++;
    Uart_Rx_Flag = DataPacketReceived;
    if(Uart_Rx_Cnt==2){
    	if((Uart_Rx_Buf[0]==0xff)&&(Uart_Rx_Buf[1]==0x06)){
    	}
    	else if((Uart_Rx_Buf[0]==0)&&(Uart_Rx_Buf[1]==0x03)){
    	}
    	else if((Uart_Rx_Buf[0]==para.gAddress)&&(Uart_Rx_Buf[1]==0x01)){
    	}
    	else if((Uart_Rx_Buf[0]==para.gAddress)&&(Uart_Rx_Buf[1]==0x02)){
    	}
    	else if((Uart_Rx_Buf[0]==para.gAddress)&&(Uart_Rx_Buf[1]==0x03)){
    	}
    	else if((Uart_Rx_Buf[0]==para.gAddress)&&(Uart_Rx_Buf[1]==0x05)){
    	}
    	else if((Uart_Rx_Buf[0]==para.gAddress)&&(Uart_Rx_Buf[1]==0x06)){
    	}
    	else if((Uart_Rx_Buf[0]==para.gAddress)&&(Uart_Rx_Buf[1]==0x0f)){
    	}
    	else if((Uart_Rx_Buf[0]==para.gAddress)&&(Uart_Rx_Buf[1]==0x10)){
    	}
    	else if((Uart_Rx_Buf[1]==para.gAddress)||(Uart_Rx_Buf[1]==0xff)||(Uart_Rx_Buf[1]==0)){
    		Uart_Rx_Buf[0]=Uart_Rx_Buf[1];
    		Uart_Rx_Cnt-=1;
    	}
    	else{
    		Uart_Rx_Cnt=0;
    		Uart_Rx_Flag=UartSleep;
    	}
    }

    if(Uart_Rx_Cnt>99){
    	Uart_Rx_Cnt=0;
      Uart_Length_Flag=0;
    }
  }
  if(TI){

  	TI=0;
  	Uart_Tx_Cnt+=1;
  	if(Uart_Tx_Cnt>=Uart_Tx_Length){
  		TXD_RUN ^= 1;
  		Uart_Rx_Flag=UartReturnToReceive;
  	}
  	else{
  		SBUF=Uart_Tx_Buf[Uart_Tx_Cnt];
  	}
  }
}


/*********************************************
*功能: UART接收后的处理
*********************************************/
void UartReceiveProcess(void)
{
	unsigned int CRC;
  unsigned int recvCRC1, recvCRC2;
  unsigned int RegisterStartAddress=0;
  unsigned int RegisterNumbers=0;
  unsigned char DataSaveFlag;
  unsigned int BakDOStatus;

  Com0_Pack_Error=Com0PackOk;

  if(Uart_Rx_Cnt<6){
  	Com0_Pack_Error=Com0PackErr;
  }
    if(Com0_Pack_Error==Com0PackOk){
  		/*CRC校验判断*/
  		CRC=CRC16(Uart_Rx_Buf,Uart_Rx_Cnt-2);
      recvCRC1=Uart_Rx_Buf[Uart_Rx_Cnt-2]*256+Uart_Rx_Buf[Uart_Rx_Cnt-1];
      recvCRC2=Uart_Rx_Buf[Uart_Rx_Cnt-1]*256+Uart_Rx_Buf[Uart_Rx_Cnt-2];
      if(CRC==recvCRC1)
      {
          CRC16flag = CRC16HiLo;
      }
      else if(CRC==recvCRC2)
      {
          CRC16flag = CRC16LoHi;
      }
      else
      {
        Com0_Pack_Error=Com0PackErr;/*data pack CRC err*/
      }
  	}
  RegisterStartAddress=Uart_Rx_Buf[2]*256+Uart_Rx_Buf[3];
	//RegisterStartAddress-=40000;
/*
	if(Uart_Rx_Buf[0]==0){
		if((RegisterStartAddress!=101)||(RegisterNumbers!=1)){
			Com0_Pack_Error=Com0PackErr;
		}
	}

	if(Uart_Rx_Buf[0]==0xff){
		if((RegisterStartAddress!=102)||(RegisterNumbers!=1)){
			Com0_Pack_Error=Com0PackErr;
		}
	}
	*/
  if(Com0_Pack_Error==Com0PackOk){
		/*数据地址边界判断*/
		switch(Uart_Rx_Buf[1]){
			case 0x01:{//coil status bit
				if(Uart_Rx_Cnt!=8){
  	      Com0_Pack_Error=Com0PackErr;
        }
        else{
  				RegisterNumbers=Uart_Rx_Buf[4]*256+Uart_Rx_Buf[5];
  				LoadCoilStatus( RegisterStartAddress, RegisterNumbers,&Uart_Tx_Buf[3]);
  				LoadCoilStatusAnswer(RegisterNumbers);
  			}
				break;
			}
			case 0x02:{//DI status bit
				if(Uart_Rx_Cnt!=8){
  	      Com0_Pack_Error=Com0PackErr;
        }
        else{
  				RegisterNumbers=Uart_Rx_Buf[4]*256+Uart_Rx_Buf[5];
  				LoadDIStatus( RegisterStartAddress, RegisterNumbers,&Uart_Tx_Buf[3]);
  				LoadDIStatusAnswer(RegisterNumbers);
  			}
				break;
			}
			case 0x03:{//read keep register
				RegisterNumbers=Uart_Rx_Buf[4]*256+Uart_Rx_Buf[5];
				if(Uart_Rx_Cnt!=8){
  	      Com0_Pack_Error=Com0PackErr;
        }
        if(Uart_Rx_Buf[0]==0){
    		  if((RegisterStartAddress!=40301)||(RegisterNumbers!=1)){
      			Com0_Pack_Error=Com0PackErr;
      		}
      	}
        if(Com0_Pack_Error==Com0PackOk){
  				LoadKeepRegister( RegisterStartAddress, RegisterNumbers,&Uart_Tx_Buf[3]);
  				LoadKeepAnswer(RegisterNumbers);
  			}
				break;
			}

			case 0x05:{//preset signal coil
				if(Uart_Rx_Cnt!=8){
  	      Com0_Pack_Error=Com0PackErr;
        }
        else{
        	BakDOStatus = RegDO.CHxx;
          SignalCoilControl(RegisterStartAddress,&Uart_Rx_Buf[4]);
          SignalCoilControlAnswer();
          if((RegDO.CHxx != BakDOStatus)&&(DOStatusKeep==Logic_True)){
          	DataSaveFlag=Logic_True;
  					//SaveDatToStcflash(DataSaveFlag);
          }

        }
				break;
			}
			case 0x0f:{//control more coil
				if((Uart_Rx_Buf[6]<1)||(Uart_Rx_Buf[6]>2)){
					Com0_Pack_Error=Com0PackErr;
				}
				if(Uart_Rx_Cnt!=(Uart_Rx_Buf[6]+9)){
  	      Com0_Pack_Error=Com0PackErr;
        }
        if(Com0_Pack_Error==Com0PackOk){
        	BakDOStatus = RegDO.CHxx;
  				RegisterNumbers=Uart_Rx_Buf[4]*256+Uart_Rx_Buf[5];
  				ForceMultipleCoil(RegisterStartAddress,RegisterNumbers,&Uart_Rx_Buf[7]);
  				ForceMultipleCoilAnswer();
  				if((RegDO.CHxx != BakDOStatus)&&(DOStatusKeep==Logic_True)){
  					DataSaveFlag=Logic_True;
  					//SaveDatToStcflash(DataSaveFlag);
          }
  			}
				break;
			}
			case 0x06:{//preset signal register
				if(Uart_Rx_Cnt!=8){
  	      Com0_Pack_Error=Com0PackErr;
        }
        else{
          DataSaveFlag=Logic_False;
    		  DataSaveFlag=PresetSignalReg( RegisterStartAddress,&Uart_Rx_Buf[4]);
    		  PresetSignalRegAnswer();
    			//SaveDatToStcflash(DataSaveFlag);
    		}
				break;
			}
			case 0x10:{//Preset Multiple Register
				RegisterNumbers=Uart_Rx_Buf[4]*256+Uart_Rx_Buf[5];
				if(Uart_Rx_Cnt!=(Uart_Rx_Buf[6]+9)){
  	      Com0_Pack_Error=Com0PackErr;
        }
        if((Uart_Rx_Buf[6]!=RegisterNumbers*2)){
					Com0_Pack_Error=Com0PackErr;
				}
				if(Uart_Rx_Buf[0]==0xff){
      		if((RegisterStartAddress!=40102)||(RegisterNumbers!=1)){
      			Com0_Pack_Error=Com0PackErr;
      		}
      	}
        if(Com0_Pack_Error==Com0PackOk){
          DataSaveFlag=Logic_False;
  				DataSaveFlag=SaveRegister( RegisterStartAddress,RegisterNumbers,&Uart_Rx_Buf[7]);
  				ModelbusWriteAnswer();
  				//SaveDatToStcflash(DataSaveFlag);
  			}
				break;
			}
			default:{
				Com0_Pack_Error=Com0PackErr;
				break;
			}
		}
	}
	if(Com0_Pack_Error!=Com0PackOk){
		Uart_Rx_Flag=UartReturnToReceive;
	}
}
/*****************************************
*功能: 读COIL STATUS
*****************************************/
void LoadCoilStatusAnswer(unsigned int RegisterNumbers)
{
	unsigned int Crc=0;
	switch(Com0_Pack_Error){
		case Com0PackOk:{
        Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
        Uart_Tx_Buf[1]=Uart_Rx_Buf[1];
        if(RegisterNumbers>8) Uart_Tx_Buf[2]=2;
        	else Uart_Tx_Buf[2]=1;
        Crc = CRC16( Uart_Tx_Buf, 3 + Uart_Tx_Buf[2] );
        if(CRC16flag==CRC16HiLo)
        {
            Uart_Tx_Buf[3 + Uart_Tx_Buf[2] ]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[4 + Uart_Tx_Buf[2] ]=( unsigned char )  Crc ;
        }
        else if(CRC16flag==CRC16LoHi)
        {
            Uart_Tx_Buf[4 + Uart_Tx_Buf[2] ]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[3 + Uart_Tx_Buf[2] ]=( unsigned char )  Crc ;
        }
        Uart_Tx_Length=5 + Uart_Tx_Buf[2];
			break;
		}
		case Com0PackOkButDatAddrOutside:{
			Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
			Uart_Tx_Buf[1]=Uart_Rx_Buf[1]+0X80;
			Uart_Tx_Buf[2]=Com0_Pack_Error;
			Com0_Pack_Error=Com0PackOk;
			Crc = CRC16( Uart_Tx_Buf, 3);
        if(CRC16flag==CRC16HiLo)
        {
            Uart_Tx_Buf[3]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[4]=( unsigned char )  Crc ;
        }
        else if(CRC16flag==CRC16LoHi)
        {
            Uart_Tx_Buf[4]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[3]=( unsigned char )  Crc ;
        }
        Uart_Tx_Length=5;
			break;
		}
		default:{
			//Com0_Pack_Error=0;
			break;
		}
	}
}
void LoadCoilStatus(unsigned int StartAddress, unsigned int Numbers, unsigned char *buf)
{
  unsigned int i=0;
  unsigned int CoilStatus=0;
  unsigned int RegisterAddress=0;
  //unsigned int Index=0;
  unsigned int ValueInt=0;

  i=0;
  buf[0]=0;
  buf[1]=0;
  CoilStatus=0;

  for(RegisterAddress=StartAddress;RegisterAddress<StartAddress+Numbers;RegisterAddress++)
  {
		switch(RegisterAddress){
			case DO1_Enum:{
        if(RegDO.CHxx&BIT0){
        	CoilStatus |= BIT0<<i;
        }
				break;
			}
			case DO2_Enum:{
        if(RegDO.CHxx&BIT1){
        	CoilStatus |= BIT0<<i;
        }
				break;
			}
			case DO3_Enum:{
        if(RegDO.CHxx&BIT2){
        	CoilStatus |= BIT0<<i;
        }
				break;
			}
			case DO4_Enum:{
        if(RegDO.CHxx&BIT3){
        	CoilStatus |= BIT0<<i;
        }
				break;
			}
			case DO5_Enum:{
        if(RegDO.CHxx&BIT4){
        	CoilStatus |= BIT0<<i;
        }
				break;
			}
			case DO6_Enum:{
        if(RegDO.CHxx&BIT5){
        	CoilStatus |= BIT0<<i;
        }
				break;
			}
			case DO7_Enum:{
        if(RegDO.CHxx&BIT6){
        	CoilStatus |= BIT0<<i;
        }
				break;
			}
			case DO8_Enum:{
        if(RegDO.CHxx&BIT7){
        	CoilStatus |= BIT0<<i;
        }
				break;
			}
			case DO9_Enum:{
        if(RegDO.CHxx&BIT8){
        	CoilStatus |= BIT0<<i;
        }
				break;
			}
			case DO10_Enum:{
        if(RegDO.CHxx&BIT9){
        	CoilStatus |= BIT0<<i;
        }
				break;
			}
			//
			case DO11_Enum:{
				break;
			}
			case DO12_Enum:{
				break;
			}
			case DO13_Enum:{
				break;
			}
			case DO14_Enum:{
				break;
			}
			case DO15_Enum:{
				break;
			}
			case DO16_Enum:{
				break;
			}
			default:{
				Com0_Pack_Error=Com0PackOkButDatAddrOutside;
				break;
			}
	  }
    i++;
	  if(Com0_Pack_Error!=Com0PackOk){
	  	break;
	  }
	}
	buf[0] = CoilStatus & 0xff ;
	buf[1] = CoilStatus>>8 & 0xff ;
}
/*****************************************
*功能: 读DI STATUS
*****************************************/
void LoadDIStatusAnswer(unsigned int RegisterNumbers)
{
	unsigned int Crc=0;
	switch(Com0_Pack_Error){
		case Com0PackOk:{
        Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
        Uart_Tx_Buf[1]=Uart_Rx_Buf[1];
        if(RegisterNumbers>8) Uart_Tx_Buf[2]=2;
        	else Uart_Tx_Buf[2]=1;
        Crc = CRC16( Uart_Tx_Buf, 3 + Uart_Tx_Buf[2] );
        if(CRC16flag==CRC16HiLo)
        {
            Uart_Tx_Buf[3 + Uart_Tx_Buf[2] ]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[4 + Uart_Tx_Buf[2] ]=( unsigned char )  Crc ;
        }
        else if(CRC16flag==CRC16LoHi)
        {
            Uart_Tx_Buf[4 + Uart_Tx_Buf[2] ]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[3 + Uart_Tx_Buf[2] ]=( unsigned char )  Crc ;
        }
        Uart_Tx_Length=5 + Uart_Tx_Buf[2];
			break;
		}
		case Com0PackOkButDatAddrOutside:{
			Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
			Uart_Tx_Buf[1]=Uart_Rx_Buf[1]+0X80;
			Uart_Tx_Buf[2]=Com0_Pack_Error;
			Com0_Pack_Error=Com0PackOk;
			Crc = CRC16( Uart_Tx_Buf, 3);
        if(CRC16flag==CRC16HiLo)
        {
            Uart_Tx_Buf[3]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[4]=( unsigned char )  Crc ;
        }
        else if(CRC16flag==CRC16LoHi)
        {
            Uart_Tx_Buf[4]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[3]=( unsigned char )  Crc ;
        }
        Uart_Tx_Length=5;
			break;
		}
		default:{
			//Com0_Pack_Error=0;
			break;
		}
	}
}
void LoadDIStatus(unsigned int StartAddress, unsigned int Numbers, unsigned char *buf)
{
  unsigned int i=0;
  unsigned int DIStatus=0;
  unsigned int RegisterAddress=0;
  //unsigned int Index=0;
  unsigned int ValueInt=0;

  i=0;
  buf[0]=0;
  buf[1]=0;
  DIStatus=0;

  for(RegisterAddress=StartAddress;RegisterAddress<StartAddress+Numbers;RegisterAddress++)
  {
		switch(RegisterAddress){
			case DI0_Enum:{
				break;
			}
			case DI1_Enum:{
        if(RegDI.CHxx&BIT0){
        	DIStatus |= BIT0<<i;
        }
				break;
			}
			case DI2_Enum:{
        if(RegDI.CHxx&BIT1){
        	DIStatus |= BIT0<<i;
        }
				break;
			}
			case DI3_Enum:{
        if(RegDI.CHxx&BIT2){
        	DIStatus |= BIT0<<i;
        }
				break;
			}
			case DI4_Enum:{
        if(RegDI.CHxx&BIT3){
        	DIStatus |= BIT0<<i;
        }
				break;
			}
			case DI5_Enum:{
        if(RegDI.CHxx&BIT4){
        	DIStatus |= BIT0<<i;
        }
				break;
			}
			case DI6_Enum:{
        if(RegDI.CHxx&BIT5){
        	DIStatus |= BIT0<<i;
        }
				break;
			}
			case DI7_Enum:{
        if(RegDI.CHxx&BIT6){
        	DIStatus |= BIT0<<i;
        }
				break;
			}
			case DI8_Enum:{
        if(RegDI.CHxx&BIT7){
        	DIStatus |= BIT0<<i;
        }
				break;
			}
			case DI9_Enum:{
        if(RegDI.CHxx&BIT8){
        	DIStatus |= BIT0<<i;
        }
				break;
			}
			case DI10_Enum:{
        if(RegDI.CHxx&BIT9){
        	DIStatus |= BIT0<<i;
        }
				break;
			}
			case DI11_Enum:{
				break;
			}
			case DI12_Enum:{
				break;
			}
			case DI13_Enum:{
				break;
			}
			case DI14_Enum:{
				break;
			}
			case DI15_Enum:{
				break;
			}
			default:{
				Com0_Pack_Error=Com0PackOkButDatAddrOutside;
				break;
			}
	  }
    i++;
	  if(Com0_Pack_Error!=Com0PackOk){
	  	break;
	  }
	}
	buf[0] = DIStatus & 0xff;
	buf[1] = (DIStatus>>8) & 0xff;
}
/***************
* KEEP REGISTER
****************/
/*****************************************
*功能: 读数据
*****************************************/
void LoadKeepAnswer(unsigned int RegisterNumbers)
{
	unsigned int Crc=0;
	switch(Com0_Pack_Error){
		case Com0PackOk:{
        Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
        Uart_Tx_Buf[1]=Uart_Rx_Buf[1];
        Uart_Tx_Buf[2]=RegisterNumbers * 2;
        Crc = CRC16( Uart_Tx_Buf, 3 + RegisterNumbers * 2 );
        if(CRC16flag==CRC16HiLo)
        {
            Uart_Tx_Buf[3 + RegisterNumbers * 2 ]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[4 + RegisterNumbers * 2 ]=( unsigned char )  Crc ;
        }
        else if(CRC16flag==CRC16LoHi)
        {
            Uart_Tx_Buf[4 + RegisterNumbers * 2 ]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[3 + RegisterNumbers * 2 ]=( unsigned char )  Crc ;
        }
        Uart_Tx_Length=5 + RegisterNumbers * 2;
			break;
		}
		case Com0PackOkButDatAddrOutside:{
			Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
			Uart_Tx_Buf[1]=Uart_Rx_Buf[1]+0X80;
			Uart_Tx_Buf[2]=Com0_Pack_Error;
			Com0_Pack_Error=Com0PackOk;
			Crc = CRC16( Uart_Tx_Buf, 3);
        if(CRC16flag==CRC16HiLo)
        {
            Uart_Tx_Buf[3]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[4]=( unsigned char )  Crc ;
        }
        else if(CRC16flag==CRC16LoHi)
        {
            Uart_Tx_Buf[4]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[3]=( unsigned char )  Crc ;
        }
        Uart_Tx_Length=5;
			break;
		}
		default:{
			//Com0_Pack_Error=0;
			break;
		}
	}
}
void LoadKeepRegister(unsigned int StartAddress, unsigned int Numbers, unsigned char *buf)
{
  unsigned int i=0;
  unsigned int RegisterAddress=0;
  //unsigned int Index=0;
  unsigned int ValueInt=0;

  i=0;
  for(RegisterAddress=StartAddress;RegisterAddress<StartAddress+Numbers;RegisterAddress++)
  {
		switch(RegisterAddress){
			case U_BAT1_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[1]*ChannelAdjustValue[1]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT2_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[2]*ChannelAdjustValue[2]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT3_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[3]*ChannelAdjustValue[3]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT4_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[4]*ChannelAdjustValue[4]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT5_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[5]*ChannelAdjustValue[5]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT6_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[6]*ChannelAdjustValue[6]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT7_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[7]*ChannelAdjustValue[7]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT8_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[8]*ChannelAdjustValue[8]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT9_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[9]*ChannelAdjustValue[9]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT10_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[10]*ChannelAdjustValue[10]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT11_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[11]*ChannelAdjustValue[11]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT12_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[12]*ChannelAdjustValue[12]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT13_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[13]*ChannelAdjustValue[13]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT14_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[14]*ChannelAdjustValue[14]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT15_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[15]*ChannelAdjustValue[15]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT16_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[16]*ChannelAdjustValue[16]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT17_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[17]*ChannelAdjustValue[17]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT18_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[18]*ChannelAdjustValue[18]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT19_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[19]*ChannelAdjustValue[19]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT20_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[20]*ChannelAdjustValue[20]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT21_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[21]*ChannelAdjustValue[21]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT22_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[22]*ChannelAdjustValue[22]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT23_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[23]*ChannelAdjustValue[23]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT24_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[24]*ChannelAdjustValue[24]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT25_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[25]*ChannelAdjustValue[25]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT26_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[26]*ChannelAdjustValue[26]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT27_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[27]*ChannelAdjustValue[27]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT28_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[28]*ChannelAdjustValue[28]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT29_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[29]*ChannelAdjustValue[29]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT30_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[30]*ChannelAdjustValue[30]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT31_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[31]*ChannelAdjustValue[31]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT32_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[32]*ChannelAdjustValue[32]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT33_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[33]*ChannelAdjustValue[33]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT34_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[34]*ChannelAdjustValue[34]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT35_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[35]*ChannelAdjustValue[35]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT36_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[36]*ChannelAdjustValue[36]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT37_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[37]*ChannelAdjustValue[37]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT38_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[38]*ChannelAdjustValue[38]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT39_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[39]*ChannelAdjustValue[39]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT40_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[40]*ChannelAdjustValue[40]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT41_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[41]*ChannelAdjustValue[41]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT42_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[42]*ChannelAdjustValue[42]*1000);
				ValueInt *= 6.25;
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT43_Enum:{
				ValueInt = (unsigned int)(ChannelVolageValue[43]*ChannelAdjustValue[43]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case U_BAT44_Enum:{
				ValueInt = (ChannelVolageValue[44]*1000);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case TEPM1_Enum:{
				ValueInt = (0);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}
			case TEPM2_Enum:{
				ValueInt = (0);
				buf[i*2]=(ValueInt>>8)&0xff;
        buf[i*2+1]=ValueInt&0xff;
				break;
			}

			case DeviceAddr_Enum:{
				buf[i*2]=0;
        buf[i*2+1]=para.gAddress;//DeviceAddr;
				break;
			}
			case CommBaud_Enum:{
				buf[i*2]=0;
        buf[i*2+1]=para.baud;//CommBaud;
				break;
			}
			/*
			case DOStatusKeep_Enum:{
				buf[i*2]=0;
        buf[i*2+1]=DOStatusKeep;
				break;
			}
			case DIStaAvailableTime_Enum:{
				buf[i*2]=((DIStaAvailableTime*20)>>8)&0xff;
        buf[i*2+1]=(DIStaAvailableTime*20)&0xff;
				break;
			}
			*/
			case BAT41_Version_Enum:{
				buf[i*2]= 0;
        buf[i*2+1]=(VERSION);
				break;
			}
			default:{
				Com0_Pack_Error=Com0PackOkButDatAddrOutside;
				break;
			}
	  }
    i++;
	  if(Com0_Pack_Error!=Com0PackOk){
	  	break;
	  }
	}

}
/**********************************************
*功能：signal coli control answer
**********************************************/
void SignalCoilControlAnswer(void)
{
	unsigned int Crc=0;
	switch(Com0_Pack_Error){
		case Com0PackOk:{
      Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
      Uart_Tx_Buf[1]=Uart_Rx_Buf[1];
      Uart_Tx_Buf[2]=Uart_Rx_Buf[2];
      Uart_Tx_Buf[3]=Uart_Rx_Buf[3];
      Uart_Tx_Buf[4]=Uart_Rx_Buf[4];
      Uart_Tx_Buf[5]=Uart_Rx_Buf[5];

      Crc = CRC16( Uart_Tx_Buf, 6 );
      if(CRC16flag==CRC16HiLo)
      {
          Uart_Tx_Buf[6]=( unsigned char ) ( Crc >> 8 );
          Uart_Tx_Buf[7]=( unsigned char ) ( Crc );
      }
      else if(CRC16flag==CRC16LoHi)
      {
          Uart_Tx_Buf[7]=( unsigned char ) ( Crc >> 8 );
          Uart_Tx_Buf[6]=( unsigned char ) ( Crc );
      }
      //Uart_Tx_Cnt=8;
      Uart_Tx_Length=8;
      //TXBUF0=Uart_Tx_Buf[Uart_Tx_Length];
			break;
		}
		case Com0PackErr:{
			Uart_Rx_Flag = UartReturnToReceive;
			break;
		}
		default:{
			Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
			Uart_Tx_Buf[1]=Uart_Rx_Buf[1]+0X80;
			Uart_Tx_Buf[2]=Com0_Pack_Error;
			Com0_Pack_Error=Com0PackOk;
			Crc = CRC16( Uart_Tx_Buf, 3);
        if(CRC16flag==CRC16HiLo)
        {
            Uart_Tx_Buf[3]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[4]=( unsigned char )  Crc ;
        }
        else if(CRC16flag==CRC16LoHi)
        {
            Uart_Tx_Buf[4]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[3]=( unsigned char )  Crc ;
        }
      //Uart_Tx_Cnt=5;
      Uart_Tx_Length=5;
      //TXBUF0=Uart_Tx_Buf[Uart_Tx_Length];
			break;
		}
	}
}
void SignalCoilControl(unsigned int StartAddress,unsigned char *buf)
{

  unsigned int temp=0;
  unsigned int i=0;
  unsigned int RegisterAddress=0;


  temp=buf[0];
  temp=temp*256;
  temp=temp+buf[1]; //bit opreation

  RegisterAddress = StartAddress;

  switch(RegisterAddress){
  	case DO1_Enum:{//addr
  		if(temp==0xff00){
  			RegDO.CHxx |= BIT0;
  		}
  		else if(temp==0){
  			RegDO.CHxx &=~ BIT0;
  		}
  		else{
  			Com0_Pack_Error=Com0PackOkButDatOutside;
  		}
  		break;
  	}
  	case DO2_Enum:{//addr
  		if(temp==0xff00){
  			RegDO.CHxx |= BIT1;
  		}
  		else if(temp==0){
  			RegDO.CHxx &=~ BIT1;
  		}
  		else{
  			Com0_Pack_Error=Com0PackOkButDatOutside;
  		}
  		break;
  	}
  	case DO3_Enum:{//addr
  		if(temp==0xff00){
  			RegDO.CHxx |= BIT2;
  		}
  		else if(temp==0){
  			RegDO.CHxx &=~ BIT2;
  		}
  		else{
  			Com0_Pack_Error=Com0PackOkButDatOutside;
  		}
  		break;
  	}
  	case DO4_Enum:{//addr
  		if(temp==0xff00){
  			RegDO.CHxx |= BIT3;
  		}
  		else if(temp==0){
  			RegDO.CHxx &=~ BIT3;
  		}
  		else{
  			Com0_Pack_Error=Com0PackOkButDatOutside;
  		}
  		break;
  	}
  	case DO5_Enum:{//addr
  		if(temp==0xff00){
  			RegDO.CHxx |= BIT4;
  		}
  		else if(temp==0){
  			RegDO.CHxx &=~ BIT4;
  		}
  		else{
  			Com0_Pack_Error=Com0PackOkButDatOutside;
  		}
  		break;
  	}
  	case DO6_Enum:{//addr
  		if(temp==0xff00){
  			RegDO.CHxx |= BIT5;
  		}
  		else if(temp==0){
  			RegDO.CHxx &=~ BIT5;
  		}
  		else{
  			Com0_Pack_Error=Com0PackOkButDatOutside;
  		}
  		break;
  	}
  	case DO7_Enum:{//addr
  		if(temp==0xff00){
  			RegDO.CHxx |= BIT6;
  		}
  		else if(temp==0){
  			RegDO.CHxx &=~ BIT6;
  		}
  		else{
  			Com0_Pack_Error=Com0PackOkButDatOutside;
  		}
  		break;
  	}
  	case DO8_Enum:{//addr
  		if(temp==0xff00){
  			RegDO.CHxx |= BIT7;
  		}
  		else if(temp==0){
  			RegDO.CHxx &=~ BIT7;
  		}
  		else{
  			Com0_Pack_Error=Com0PackOkButDatOutside;
  		}
  		break;
  	}
  	case DO9_Enum:{//addr
  		if(temp==0xff00){
  			RegDO.CHxx |= BIT8;
  		}
  		else if(temp==0){
  			RegDO.CHxx &=~ BIT8;
  		}
  		else{
  			Com0_Pack_Error=Com0PackOkButDatOutside;
  		}
  		break;
  	}
  	case DO10_Enum:{//addr
  		if(temp==0xff00){
  			RegDO.CHxx |= BIT9;
  		}
  		else if(temp==0){
  			RegDO.CHxx &=~ BIT9;
  		}
  		else{
  			Com0_Pack_Error=Com0PackOkButDatOutside;
  		}
  		break;
  	}
  	//
  	case DO11_Enum:{//addr
  		break;
  	}
  	case DO12_Enum:{//addr
  		break;
  	}
  	case DO13_Enum:{//addr
  		break;
  	}
  	case DO14_Enum:{//addr
  		break;
  	}
  	case DO15_Enum:{//addr
  		break;
  	}
  	case DO16_Enum:{//addr
  		break;
  	}


	  default:{
  		Com0_Pack_Error=Com0PackOkButDatAddrOutside;
  		break;
    }
  }

}
/**********************************************
*功能：coli control answer
**********************************************/
void ForceMultipleCoilAnswer(void)
{
	unsigned int Crc=0;
	switch(Com0_Pack_Error){
		case Com0PackOk:{
      Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
      Uart_Tx_Buf[1]=Uart_Rx_Buf[1];
      Uart_Tx_Buf[2]=Uart_Rx_Buf[2];
      Uart_Tx_Buf[3]=Uart_Rx_Buf[3];
      Uart_Tx_Buf[4]=Uart_Rx_Buf[4];
      Uart_Tx_Buf[5]=Uart_Rx_Buf[5];

      Crc = CRC16( Uart_Tx_Buf, 6 );
      if(CRC16flag==CRC16HiLo)
      {
          Uart_Tx_Buf[6]=( unsigned char ) ( Crc >> 8 );
          Uart_Tx_Buf[7]=( unsigned char ) ( Crc );
      }
      else if(CRC16flag==CRC16LoHi)
      {
          Uart_Tx_Buf[7]=( unsigned char ) ( Crc >> 8 );
          Uart_Tx_Buf[6]=( unsigned char ) ( Crc );
      }
      //Uart_Tx_Cnt=8;
      Uart_Tx_Length=8;
      //TXBUF0=Uart_Tx_Buf[Uart_Tx_Length];
			break;
		}
		case Com0PackErr:{
			Uart_Rx_Flag = UartReturnToReceive;
			break;
		}
		default:{
			Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
			Uart_Tx_Buf[1]=Uart_Rx_Buf[1]+0X80;
			Uart_Tx_Buf[2]=Com0_Pack_Error;
			Com0_Pack_Error=Com0PackOk;
			Crc = CRC16( Uart_Tx_Buf, 3);
        if(CRC16flag==CRC16HiLo)
        {
            Uart_Tx_Buf[3]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[4]=( unsigned char )  Crc ;
        }
        else if(CRC16flag==CRC16LoHi)
        {
            Uart_Tx_Buf[4]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[3]=( unsigned char )  Crc ;
        }
      //Uart_Tx_Cnt=5;
      Uart_Tx_Length=5;
      //TXBUF0=Uart_Tx_Buf[Uart_Tx_Length];
			break;
		}
	}
}
void ForceMultipleCoil(unsigned int StartAddress,unsigned int Numbers,unsigned char *buf)
{

  unsigned int temp=0;
  //unsigned int bitinf=0;
  unsigned int i=0;
  unsigned int RegisterAddress=0;

  if(Uart_Rx_Buf[6]==1){
  	temp = buf[0];
  }
  else{
    temp=buf[1];
    temp=temp*256;
    temp=temp+buf[0]; //bit opreation
  }
  i=0;

  for(RegisterAddress=StartAddress;RegisterAddress<StartAddress+Numbers;RegisterAddress++){
    switch(RegisterAddress){
    	case DO1_Enum:{//addr
    		//bitinf = temp & (((uint)BIT0)<<i);
    		if(temp & (((uint)BIT0)<<i)){
    			RegDO.CHxx |= BIT0;
    		}
    		else{
    			RegDO.CHxx &=~ BIT0;
    		}
    		break;
    	}
    	case DO2_Enum:{//addr
    		//bitinf = temp & (BIT0<<i);
    		if(temp & (((uint)BIT0)<<i)){
    			RegDO.CHxx |= BIT1;
    		}
    		else{
    			RegDO.CHxx &=~ BIT1;
    		}
    		break;
    	}
    	case DO3_Enum:{//addr
    		//bitinf = temp & (BIT0<<i);
    		if(temp & (((uint)BIT0)<<i)){
    			RegDO.CHxx |= BIT2;
    		}
    		else{
    			RegDO.CHxx &=~ BIT2;
    		}
    		break;
    	}
    	case DO4_Enum:{//addr
    		//bitinf = temp & (BIT0<<i);
    		if(temp & (((uint)BIT0)<<i)){
    			RegDO.CHxx |= BIT3;
    		}
    		else{
    			RegDO.CHxx &=~ BIT3;
    		}
    		break;
    	}
    	case DO5_Enum:{//addr
    		//bitinf = temp & (BIT0<<i);
    		if(temp & (((uint)BIT0)<<i)){
    			RegDO.CHxx |= BIT4;
    		}
    		else{
    			RegDO.CHxx &=~ BIT4;
    		}
    		break;
    	}
    	case DO6_Enum:{//addr
    		//bitinf = temp & (BIT0<<i);
    		if(temp & (((uint)BIT0)<<i)){
    			RegDO.CHxx |= BIT5;
    		}
    		else{
    			RegDO.CHxx &=~ BIT5;
    		}
    		break;
    	}
    	case DO7_Enum:{//addr
    		//bitinf = temp & (BIT0<<i);
    		if(temp & (((uint)BIT0)<<i)){
    			RegDO.CHxx |= BIT6;
    		}
    		else{
    			RegDO.CHxx &=~ BIT6;
    		}
    		break;
    	}
    	case DO8_Enum:{//addr
    		//bitinf = temp & (BIT0<<i);
    		if(temp & (((uint)BIT0)<<i)){
    			RegDO.CHxx |= BIT7;
    		}
    		else{
    			RegDO.CHxx &=~ BIT7;
    		}
    		break;
    	}
    	case DO9_Enum:{//addr
    		//bitinf = temp & (BIT0<<i);
    		if(temp & (((uint)BIT0)<<i)){
    			RegDO.CHxx |= BIT8;
    		}
    		else{
    			RegDO.CHxx &=~ BIT8;
    		}
    		break;
    	}
    	case DO10_Enum:{//addr
    		//bitinf = temp & (BIT0<<i);
    		if(temp & (((uint)BIT0)<<i)){
    			RegDO.CHxx |= BIT9;
    		}
    		else{
    			RegDO.CHxx &=~ BIT9;
    		}
    		break;
    	}
    	//
    	case DO11_Enum:{//addr
    		break;
    	}
    	case DO12_Enum:{//addr
    		break;
    	}
    	case DO13_Enum:{//addr
    		break;
    	}
    	case DO14_Enum:{//addr
    		break;
    	}
    	case DO15_Enum:{//addr
    		break;
    	}
    	case DO16_Enum:{//addr
    		break;
    	}
  	  default:{
    		Com0_Pack_Error=Com0PackOkButDatAddrOutside;
    		break;
      }
    }
  	i++;
  	if(Com0_Pack_Error!=Com0PackOk){
	  	break;
	  }
  }
}
/**********************************************
*功能：preset signal reg
**********************************************/
void PresetSignalRegAnswer(void)
{
	unsigned int Crc=0;
	switch(Com0_Pack_Error){
		case Com0PackOk:{
      Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
      Uart_Tx_Buf[1]=Uart_Rx_Buf[1];
      Uart_Tx_Buf[2]=Uart_Rx_Buf[2];
      Uart_Tx_Buf[3]=Uart_Rx_Buf[3];
      Uart_Tx_Buf[4]=Uart_Rx_Buf[4];
      Uart_Tx_Buf[5]=Uart_Rx_Buf[5];
      Crc = CRC16( Uart_Tx_Buf, 6 );
      if(CRC16flag==CRC16HiLo)
      {
          Uart_Tx_Buf[6]=( unsigned char ) ( Crc >> 8 );
          Uart_Tx_Buf[7]=( unsigned char ) ( Crc );
      }
      else if(CRC16flag==CRC16LoHi)
      {
          Uart_Tx_Buf[7]=( unsigned char ) ( Crc >> 8 );
          Uart_Tx_Buf[6]=( unsigned char ) ( Crc );
      }
      //Uart_Tx_Cnt=8;
      Uart_Tx_Length=8;
      //TXBUF0=Uart_Tx_Buf[Uart_Tx_Length];
			break;
		}
		case Com0PackErr:{
			Uart_Rx_Flag = UartReturnToReceive;
			break;
		}
		default:{
			Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
			Uart_Tx_Buf[1]=Uart_Rx_Buf[1]+0X80;
			Uart_Tx_Buf[2]=Com0_Pack_Error;
			Com0_Pack_Error=Com0PackOk;
			Crc = CRC16( Uart_Tx_Buf, 3);
        if(CRC16flag==CRC16HiLo)
        {
            Uart_Tx_Buf[3]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[4]=( unsigned char )  Crc ;
        }
        else if(CRC16flag==CRC16LoHi)
        {
            Uart_Tx_Buf[4]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[3]=( unsigned char )  Crc ;
        }
      //Uart_Tx_Cnt=5;
      Uart_Tx_Length=5;
      //TXBUF0=Uart_Tx_Buf[Uart_Tx_Length];
			break;
		}
	}
}
unsigned char PresetSignalReg(unsigned int StartAddress,unsigned char *buf)
{

  unsigned int temp=0;
  //unsigned int i=0;
  unsigned int RegisterAddress=0;
  unsigned char DataSaveFlag=0;

  DataSaveFlag=Logic_False;
  //i=0;

	temp=buf[0];
  temp=temp*256;
  temp=temp+buf[1]; //以 字进行操作
  RegisterAddress = StartAddress;
  SaveDataToEEPROM(RegisterAddress,temp);
  /*
  switch(RegisterAddress){
  	  case U_BAT1_Enum:{
    		ChannelAdjustValue[1] = temp;
        ChannelAdjustValue[1] /= 1000;
        ChannelAdjustValue[1] /= ChannelVolageValue[1];
        if((ChannelAdjustValue[1]>1.2)||(ChannelAdjustValue[1]<0.8)){
        	ChannelAdjustValue[1] = 1;
        }
        WrAdjust();
				break;
			}
      case DeviceAddr_Enum:{//addr
    		if( ( temp == 0 ) || ( temp == 0xff ) ){
    			Com0_Pack_Error=Com0PackOkButDatOutside;
        }
        else{
        	para.gAddress = temp & 0xff;
        	WrPara();
        }
    		break;
    	}

    	case CommBaud_Enum:{
    		if( Uart_Rx_Buf[0]==0xff ){
    			Com0_Pack_Error=1;//save data but not answer
    			if( temp < 28800 ){
        		switch(temp){
        			case 1200:{
        				para.baud = Baud1200;
        				break;
        			}
        			case 2400:{
        				para.baud = Baud2400;
        				break;
        			}
        			case 4800:{
        				para.baud = Baud4800;
        				break;
        			}
        			case 9600:{
        				para.baud = Baud9600;
        				break;
        			}
        			case 14400:{
        				para.baud = Baud14400;
        				break;
        			}
        			default:{
        				para.baud = Baud9600;
        				break;
        			}
        		}
        		WrPara();
          }
        }
        else{
        	if( temp < 28800 ){
        		switch(temp){
        			case 1200:{
        				para.baud = Baud1200;
        				break;
        			}
        			case 2400:{
        				para.baud = Baud2400;
        				break;
        			}
        			case 4800:{
        				para.baud = Baud4800;
        				break;
        			}
        			case 9600:{
        				para.baud = Baud9600;
        				break;
        			}
        			case 14400:{
        				para.baud = Baud14400;
        				break;
        			}
        			default:{
        				break;
        			}
        		}
        		WrPara();
          }
          else{
          	Com0_Pack_Error=Com0PackOkButDatOutside;
          }
        }
    		break;
    	}
  	/
  	case DOStatusKeep_Enum:{
  		if(temp < 2){
  			DOStatusKeep= temp&0xff;
  			DataSaveFlag=Logic_True;
  		}
  		else{
  			Com0_Pack_Error=Com0PackOkButDatOutside;
  		}
  		break;
  	}
  	case DIStaAvailableTime_Enum:{
  		if((temp > 20)&&(temp <= 10000)){
  			DIStaAvailableTime = temp/20;
  			DataSaveFlag=Logic_True;
  		}
  		else{
  			Com0_Pack_Error=Com0PackOkButDatOutside;
  		}
  		break;
  	}
  	/
	  default:{
  		Com0_Pack_Error=Com0PackOkButDatAddrOutside;
  		break;
    }
  }
  */
  return DataSaveFlag;
}
/**********************************************
*功能：preset multiple register
**********************************************/
void ModelbusWriteAnswer(void)
{
	unsigned int Crc=0;
	switch(Com0_Pack_Error){
		case Com0PackOk:{
      Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
      Uart_Tx_Buf[1]=Uart_Rx_Buf[1];
      Uart_Tx_Buf[2]=Uart_Rx_Buf[2];
      Uart_Tx_Buf[3]=Uart_Rx_Buf[3];
      Uart_Tx_Buf[4]=Uart_Rx_Buf[4];
      Uart_Tx_Buf[5]=Uart_Rx_Buf[5];
      Crc = CRC16( Uart_Tx_Buf, 6 );
      if(CRC16flag==CRC16HiLo)
      {
          Uart_Tx_Buf[6]=( unsigned char ) ( Crc >> 8 );
          Uart_Tx_Buf[7]=( unsigned char ) ( Crc );
      }
      else if(CRC16flag==CRC16LoHi)
      {
          Uart_Tx_Buf[7]=( unsigned char ) ( Crc >> 8 );
          Uart_Tx_Buf[6]=( unsigned char ) ( Crc );
      }
      //Uart_Tx_Cnt=8;
      Uart_Tx_Length=8;
      //TXBUF0=Uart_Tx_Buf[Uart_Tx_Length];
			break;
		}
		case Com0PackErr:{
			Uart_Rx_Flag = UartReturnToReceive;
			break;
		}
		default:{
			Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
			Uart_Tx_Buf[1]=Uart_Rx_Buf[1]+0X80;
			Uart_Tx_Buf[2]=Com0_Pack_Error;
			Com0_Pack_Error=Com0PackOk;
			Crc = CRC16( Uart_Tx_Buf, 3);
        if(CRC16flag==CRC16HiLo)
        {
            Uart_Tx_Buf[3]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[4]=( unsigned char )  Crc ;
        }
        else if(CRC16flag==CRC16LoHi)
        {
            Uart_Tx_Buf[4]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[3]=( unsigned char )  Crc ;
        }
      //Uart_Tx_Cnt=5;
      Uart_Tx_Length=5;
      //TXBUF0=Uart_Tx_Buf[Uart_Tx_Length];
			break;
		}
	}
}
/*
OPERATION: SAVE DATA
*/
void SaveDataToEEPROM(uint Address,uint value)
{
	uchar i;
	//uint value;
	//value = data;
	switch(Address){
    	case U_BAT1_Enum:{
    		ChannelAdjustValue[1] = value;
        ChannelAdjustValue[1] /= 1000;
        ChannelAdjustValue[1] /= ChannelVolageValue[1];
        if((ChannelAdjustValue[1]>1.2)||(ChannelAdjustValue[1]<0.8)){
        	ChannelAdjustValue[1] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT2_Enum:{
    		ChannelAdjustValue[2] = value;
        ChannelAdjustValue[2] /= 1000;
        ChannelAdjustValue[2] /= ChannelVolageValue[2];
        if((ChannelAdjustValue[2]>1.2)||(ChannelAdjustValue[2]<0.8)){
        	ChannelAdjustValue[2] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT3_Enum:{
    		ChannelAdjustValue[3] = value;
        ChannelAdjustValue[3] /= 1000;
        ChannelAdjustValue[3] /= ChannelVolageValue[3];
        if((ChannelAdjustValue[3]>1.2)||(ChannelAdjustValue[3]<0.8)){
        	ChannelAdjustValue[3] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT4_Enum:{
    		ChannelAdjustValue[4] = value;
        ChannelAdjustValue[4] /= 1000;
        ChannelAdjustValue[4] /= ChannelVolageValue[4];
        if((ChannelAdjustValue[4]>1.2)||(ChannelAdjustValue[4]<0.8)){
        	ChannelAdjustValue[4] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT5_Enum:{
    		ChannelAdjustValue[5] = value;
        ChannelAdjustValue[5] /= 1000;
        ChannelAdjustValue[5] /= ChannelVolageValue[5];
        if((ChannelAdjustValue[5]>1.2)||(ChannelAdjustValue[5]<0.8)){
        	ChannelAdjustValue[5] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT6_Enum:{
    		ChannelAdjustValue[6] = value;
        ChannelAdjustValue[6] /= 1000;
        ChannelAdjustValue[6] /= ChannelVolageValue[6];
        if((ChannelAdjustValue[6]>1.2)||(ChannelAdjustValue[6]<0.8)){
        	ChannelAdjustValue[6] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT7_Enum:{
    		ChannelAdjustValue[7] = value;
        ChannelAdjustValue[7] /= 1000;
        ChannelAdjustValue[7] /= ChannelVolageValue[7];
        if((ChannelAdjustValue[7]>1.2)||(ChannelAdjustValue[7]<0.8)){
        	ChannelAdjustValue[7] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT8_Enum:{
    		ChannelAdjustValue[8] = value;
        ChannelAdjustValue[8] /= 1000;
        ChannelAdjustValue[8] /= ChannelVolageValue[8];
        if((ChannelAdjustValue[8]>1.2)||(ChannelAdjustValue[8]<0.8)){
        	ChannelAdjustValue[8] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT9_Enum:{
    		ChannelAdjustValue[9] = value;
        ChannelAdjustValue[9] /= 1000;
        ChannelAdjustValue[9] /= ChannelVolageValue[9];
        if((ChannelAdjustValue[9]>1.2)||(ChannelAdjustValue[9]<0.8)){
        	ChannelAdjustValue[9] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT10_Enum:{
    		ChannelAdjustValue[10] = value;
        ChannelAdjustValue[10] /= 1000;
        ChannelAdjustValue[10] /= ChannelVolageValue[10];
        if((ChannelAdjustValue[10]>1.2)||(ChannelAdjustValue[10]<0.8)){
        	ChannelAdjustValue[10] = 1;
        }
        WrAdjust();
				break;
			}

			case U_BAT11_Enum:{
    		ChannelAdjustValue[11] = value;
        ChannelAdjustValue[11] /= 1000;
        ChannelAdjustValue[11] /= ChannelVolageValue[11];
        if((ChannelAdjustValue[11]>1.2)||(ChannelAdjustValue[11]<0.8)){
        	ChannelAdjustValue[11] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT12_Enum:{
    		ChannelAdjustValue[12] = value;
        ChannelAdjustValue[12] /= 1000;
        ChannelAdjustValue[12] /= ChannelVolageValue[12];
        if((ChannelAdjustValue[12]>1.2)||(ChannelAdjustValue[12]<0.8)){
        	ChannelAdjustValue[12] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT13_Enum:{
    		ChannelAdjustValue[13] = value;
        ChannelAdjustValue[13] /= 1000;
        ChannelAdjustValue[13] /= ChannelVolageValue[13];
        if((ChannelAdjustValue[13]>1.2)||(ChannelAdjustValue[13]<0.8)){
        	ChannelAdjustValue[13] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT14_Enum:{
    		ChannelAdjustValue[14] = value;
        ChannelAdjustValue[14] /= 1000;
        ChannelAdjustValue[14] /= ChannelVolageValue[14];
        if((ChannelAdjustValue[14]>1.2)||(ChannelAdjustValue[14]<0.8)){
        	ChannelAdjustValue[14] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT15_Enum:{
    		ChannelAdjustValue[15] = value;
        ChannelAdjustValue[15] /= 1000;
        ChannelAdjustValue[15] /= ChannelVolageValue[15];
        if((ChannelAdjustValue[15]>1.2)||(ChannelAdjustValue[15]<0.8)){
        	ChannelAdjustValue[15] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT16_Enum:{
    		ChannelAdjustValue[16] = value;
        ChannelAdjustValue[16] /= 1000;
        ChannelAdjustValue[16] /= ChannelVolageValue[16];
        if((ChannelAdjustValue[16]>1.2)||(ChannelAdjustValue[16]<0.8)){
        	ChannelAdjustValue[16] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT17_Enum:{
    		ChannelAdjustValue[17] = value;
        ChannelAdjustValue[17] /= 1000;
        ChannelAdjustValue[17] /= ChannelVolageValue[17];
        if((ChannelAdjustValue[17]>1.2)||(ChannelAdjustValue[17]<0.8)){
        	ChannelAdjustValue[17] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT18_Enum:{
    		ChannelAdjustValue[18] = value;
        ChannelAdjustValue[18] /= 1000;
        ChannelAdjustValue[18] /= ChannelVolageValue[18];
        if((ChannelAdjustValue[18]>1.2)||(ChannelAdjustValue[18]<0.8)){
        	ChannelAdjustValue[18] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT19_Enum:{
    		ChannelAdjustValue[19] = value;
        ChannelAdjustValue[19] /= 1000;
        ChannelAdjustValue[19] /= ChannelVolageValue[19];
        if((ChannelAdjustValue[19]>1.2)||(ChannelAdjustValue[19]<0.8)){
        	ChannelAdjustValue[19] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT20_Enum:{
    		ChannelAdjustValue[20] = value;
        ChannelAdjustValue[20] /= 1000;
        ChannelAdjustValue[20] /= ChannelVolageValue[20];
        if((ChannelAdjustValue[20]>1.2)||(ChannelAdjustValue[20]<0.8)){
        	ChannelAdjustValue[20] = 1;
        }
        WrAdjust();
				break;
			}

			case U_BAT21_Enum:{
    		ChannelAdjustValue[21] = value;
        ChannelAdjustValue[21] /= 1000;
        ChannelAdjustValue[21] /= ChannelVolageValue[21];
        if((ChannelAdjustValue[21]>1.2)||(ChannelAdjustValue[21]<0.8)){
        	ChannelAdjustValue[21] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT22_Enum:{
    		ChannelAdjustValue[22] = value;
        ChannelAdjustValue[22] /= 1000;
        ChannelAdjustValue[22] /= ChannelVolageValue[22];
        if((ChannelAdjustValue[22]>1.2)||(ChannelAdjustValue[22]<0.8)){
        	ChannelAdjustValue[22] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT23_Enum:{
    		ChannelAdjustValue[23] = value;
        ChannelAdjustValue[23] /= 1000;
        ChannelAdjustValue[23] /= ChannelVolageValue[23];
        if((ChannelAdjustValue[23]>1.2)||(ChannelAdjustValue[23]<0.8)){
        	ChannelAdjustValue[23] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT24_Enum:{
    		ChannelAdjustValue[24] = value;
        ChannelAdjustValue[24] /= 1000;
        ChannelAdjustValue[24] /= ChannelVolageValue[24];
        if((ChannelAdjustValue[24]>1.2)||(ChannelAdjustValue[24]<0.8)){
        	ChannelAdjustValue[24] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT25_Enum:{
    		ChannelAdjustValue[25] = value;
        ChannelAdjustValue[25] /= 1000;
        ChannelAdjustValue[25] /= ChannelVolageValue[25];
        if((ChannelAdjustValue[25]>1.2)||(ChannelAdjustValue[25]<0.8)){
        	ChannelAdjustValue[25] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT26_Enum:{
    		ChannelAdjustValue[26] = value;
        ChannelAdjustValue[26] /= 1000;
        ChannelAdjustValue[26] /= ChannelVolageValue[26];
        if((ChannelAdjustValue[26]>1.2)||(ChannelAdjustValue[26]<0.8)){
        	ChannelAdjustValue[26] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT27_Enum:{
    		ChannelAdjustValue[27] = value;
        ChannelAdjustValue[27] /= 1000;
        ChannelAdjustValue[27] /= ChannelVolageValue[27];
        if((ChannelAdjustValue[27]>1.2)||(ChannelAdjustValue[27]<0.8)){
        	ChannelAdjustValue[27] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT28_Enum:{
    		ChannelAdjustValue[28] = value;
        ChannelAdjustValue[28] /= 1000;
        ChannelAdjustValue[28] /= ChannelVolageValue[28];
        if((ChannelAdjustValue[28]>1.2)||(ChannelAdjustValue[28]<0.8)){
        	ChannelAdjustValue[28] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT29_Enum:{
    		ChannelAdjustValue[29] = value;
        ChannelAdjustValue[29] /= 1000;
        ChannelAdjustValue[29] /= ChannelVolageValue[29];
        if((ChannelAdjustValue[29]>1.2)||(ChannelAdjustValue[29]<0.8)){
        	ChannelAdjustValue[29] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT30_Enum:{
    		ChannelAdjustValue[30] = value;
        ChannelAdjustValue[30] /= 1000;
        ChannelAdjustValue[30] /= ChannelVolageValue[30];
        if((ChannelAdjustValue[30]>1.2)||(ChannelAdjustValue[30]<0.8)){
        	ChannelAdjustValue[30] = 1;
        }
        WrAdjust();
				break;
			}

			case U_BAT31_Enum:{
    		ChannelAdjustValue[31] = value;
        ChannelAdjustValue[31] /= 1000;
        ChannelAdjustValue[31] /= ChannelVolageValue[31];
        if((ChannelAdjustValue[31]>1.2)||(ChannelAdjustValue[31]<0.8)){
        	ChannelAdjustValue[31] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT32_Enum:{
    		ChannelAdjustValue[32] = value;
        ChannelAdjustValue[32] /= 1000;
        ChannelAdjustValue[32] /= ChannelVolageValue[32];
        if((ChannelAdjustValue[32]>1.2)||(ChannelAdjustValue[32]<0.8)){
        	ChannelAdjustValue[32] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT33_Enum:{
    		ChannelAdjustValue[33] = value;
        ChannelAdjustValue[33] /= 1000;
        ChannelAdjustValue[33] /= ChannelVolageValue[33];
        if((ChannelAdjustValue[33]>1.2)||(ChannelAdjustValue[33]<0.8)){
        	ChannelAdjustValue[33] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT34_Enum:{
    		ChannelAdjustValue[34] = value;
        ChannelAdjustValue[34] /= 1000;
        ChannelAdjustValue[34] /= ChannelVolageValue[34];
        if((ChannelAdjustValue[34]>1.2)||(ChannelAdjustValue[34]<0.8)){
        	ChannelAdjustValue[34] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT35_Enum:{
    		ChannelAdjustValue[35] = value;
        ChannelAdjustValue[35] /= 1000;
        ChannelAdjustValue[35] /= ChannelVolageValue[35];
        if((ChannelAdjustValue[35]>1.2)||(ChannelAdjustValue[35]<0.8)){
        	ChannelAdjustValue[35] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT36_Enum:{
    		ChannelAdjustValue[36] = value;
        ChannelAdjustValue[36] /= 1000;
        ChannelAdjustValue[36] /= ChannelVolageValue[36];
        if((ChannelAdjustValue[36]>1.2)||(ChannelAdjustValue[36]<0.8)){
        	ChannelAdjustValue[36] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT37_Enum:{
    		ChannelAdjustValue[37] = value;
        ChannelAdjustValue[37] /= 1000;
        ChannelAdjustValue[37] /= ChannelVolageValue[37];
        if((ChannelAdjustValue[37]>1.2)||(ChannelAdjustValue[37]<0.8)){
        	ChannelAdjustValue[37] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT38_Enum:{
    		ChannelAdjustValue[38] = value;
        ChannelAdjustValue[38] /= 1000;
        ChannelAdjustValue[38] /= ChannelVolageValue[38];
        if((ChannelAdjustValue[38]>1.2)||(ChannelAdjustValue[38]<0.8)){
        	ChannelAdjustValue[38] = 1;
        }
        WrAdjust();
				break;
			}
      case U_BAT39_Enum:{
    		ChannelAdjustValue[39] = value;
        ChannelAdjustValue[39] /= 1000;
        ChannelAdjustValue[39] /= ChannelVolageValue[39];
        if((ChannelAdjustValue[39]>1.2)||(ChannelAdjustValue[39]<0.8)){
        	ChannelAdjustValue[39] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT40_Enum:{
    		ChannelAdjustValue[40] = value;
        ChannelAdjustValue[40] /= 1000;
        ChannelAdjustValue[40] /= ChannelVolageValue[40];
        if((ChannelAdjustValue[40]>1.2)||(ChannelAdjustValue[40]<0.8)){
        	ChannelAdjustValue[40] = 1;
        }
        WrAdjust();
				break;
			}

			case U_BAT41_Enum:{
    		ChannelAdjustValue[41] = value;
        ChannelAdjustValue[41] /= 1000;
        ChannelAdjustValue[41] /= ChannelVolageValue[41];
        if((ChannelAdjustValue[41]>1.2)||(ChannelAdjustValue[41]<0.8)){
        	ChannelAdjustValue[41] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT42_Enum:{//total volage
    		ChannelAdjustValue[42] = value;
        ChannelAdjustValue[42] /= 1000;
        ChannelAdjustValue[42] /= 6.25;
        ChannelAdjustValue[42] /=ChannelVolageValue[42];
        if((ChannelAdjustValue[42]>1.2)||(ChannelAdjustValue[42]<0.8)){
        	ChannelAdjustValue[42] = 1;
        }
        WrAdjust();
				break;
			}
			case U_BAT43_Enum:{//current
    		ChannelAdjustValue[43] = value;
        ChannelAdjustValue[43] /= 1000;
        ChannelAdjustValue[43] /= ChannelVolageValue[43];
        if((ChannelAdjustValue[43]>1.2)||(ChannelAdjustValue[43]<0.8)){
        	ChannelAdjustValue[43] = 1;
        }
        WrAdjust();
				break;
			}


			case EvenCHCalibration:{
				if(value == U_BAT1_Enum){
					RegData.buf[3] = 1;//need calibration for even channel
				}
				else if(value == U_BAT2_Enum){
					RegData.buf[3] = 2;//need calibration for odd channel
				}
				else{
					RegData.buf[3] = 3;//need calibration for odd channel
				}
				break;
			}
			case OddCHCalibration:{
				if(RegData.buf[3] == 1){
					for(i=1;i<(CHANNEL_MAX-2);i+=2){
						ChannelAdjustValue[i] = value;
            ChannelAdjustValue[i] /= 1000;
            ChannelAdjustValue[i] /= ChannelVolageValue[i];
            if((ChannelAdjustValue[i]>1.2)||(ChannelAdjustValue[i]<0.8)){
            	ChannelAdjustValue[i] = 1;
            }
					}
				}
				else if(RegData.buf[3] == 2){
					for(i=2;i<(CHANNEL_MAX-2);i+=2){
						ChannelAdjustValue[i] = value;
            ChannelAdjustValue[i] /= 1000;
            ChannelAdjustValue[i] /= ChannelVolageValue[i];
            if((ChannelAdjustValue[i]>1.2)||(ChannelAdjustValue[i]<0.8)){
            	ChannelAdjustValue[i] = 1;
            }
					}
				}
				else{
					Com0_Pack_Error=Com0PackOkButDatOutside;
				}
				break;
			}
    	case DeviceAddr_Enum:{//addr
    		if( ( value == 0 ) || ( value == 0xff ) ){
    			Com0_Pack_Error=Com0PackOkButDatOutside;
        }
        else{
        	para.gAddress = value & 0xff;
        	WrPara();
        }
    		break;
    	}

    	case CommBaud_Enum:{
    		if( Uart_Rx_Buf[0]==0xff ){
    			Com0_Pack_Error=1;//save data but not answer
    			if( value < 28800 ){
        		switch(value){
        			case 1200:{
        				para.baud = Baud1200;
        				break;
        			}
        			case 2400:{
        				para.baud = Baud2400;
        				break;
        			}
        			case 4800:{
        				para.baud = Baud4800;
        				break;
        			}
        			case 9600:{
        				para.baud = Baud9600;
        				break;
        			}
        			case 14400:{
        				para.baud = Baud14400;
        				break;
        			}
        			default:{
        				para.baud = Baud9600;
        				break;
        			}
        		}
        		WrPara();
          }
        }
        else{
        	if( value < 28800 ){
        		switch(value){
        			case 1200:{
        				para.baud = Baud1200;
        				break;
        			}
        			case 2400:{
        				para.baud = Baud2400;
        				break;
        			}
        			case 4800:{
        				para.baud = Baud4800;
        				break;
        			}
        			case 9600:{
        				para.baud = Baud9600;
        				break;
        			}
        			case 14400:{
        				para.baud = Baud14400;
        				break;
        			}
        			default:{
        				break;
        			}
        		}
        		WrPara();
          }
          else{
          	Com0_Pack_Error=Com0PackOkButDatOutside;
          }
        }
    		break;
    	}
    	/*
    	case DOStatusKeep_Enum:{
    		if(temp < 2){
    			DOStatusKeep= temp&0xff;
    			DataSaveFlag=Logic_True;
    		}
    		else{
    			Com0_Pack_Error=Com0PackOkButDatOutside;
    		}
    		break;
    	}
    	case DIStaAvailableTime_Enum:{

    		if((temp > 20)&&(temp <= 10000)){
    			DIStaAvailableTime = temp/20;
    			DataSaveFlag=Logic_True;
    		}
    		else{
    			Com0_Pack_Error=Com0PackOkButDatOutside;
    		}
    		break;
    	}
    	*/

  	  default:{
    		Com0_Pack_Error=Com0PackOkButDatAddrOutside;
    		break;
      }
    }
}
unsigned char SaveRegister(unsigned int StartAddress,unsigned int Numbers,unsigned char *buf)
{

  unsigned int temp=0;
  unsigned int i=0;
  unsigned int RegisterAddress=0;
  unsigned char DataSaveFlag=0;

  DataSaveFlag=Logic_False;
  i=0;
  for(RegisterAddress=StartAddress;RegisterAddress<StartAddress+Numbers;RegisterAddress++){
  	temp=buf[i*2];
    temp=temp*256;
    temp=temp+buf[i*2+1]; //以 字进行操作
    SaveDataToEEPROM(RegisterAddress,temp);
  	i++;
  	if(Com0_Pack_Error!=Com0PackOk){
	  	break;
	  }
  }
  return DataSaveFlag;
}



/***********************************
1.每20毫秒查询一次,如果UART有接收动作,
则再等20豪秒后关闭UART接收;
2.关闭UART接收后立即进行数据处理;
***********************************/
void UartDataProcess(void)
{
	unsigned char BackupBaud;
  //unsigned int i,CRC;
  //unsigned int recvCRC1, recvCRC2;
  switch(Uart_Rx_Flag){
  	case UartSleep:{
  		break;
  	}
  	case DataPacketReceived:{
  		Uart_Rx_Flag = DataPacketOk;
  		/*Uart had receive data*/
  		break;
  	}
  	case DataPacketOk:{
  		/*Uart Receive Process >=50mS */
  		//P2_1=1;/*ATM2483 translate is open, receive is close*/
  		//Adm2483SendEnable;
  		ES  =0;      //disable uart receive interrupt
  		Uart_Rx_Flag = DataPacketProcess;

  		break;
  	}
  	case DataPacketProcess:{/*50mS Interrupt*/
  		BackupBaud=para.baud;
      UartReceiveProcess();
  		if(Uart_Tx_Length>0){
  			/*发送数据*/
  			Uart_Rx_Flag=DataPacketSendWait;
  			Adm2483SendEnable;
  			Uart_Tx_Cnt=0;
  			ES  =1;/*enable uart int*/
  			TI=0;
  			RI=0;
        SBUF=Uart_Tx_Buf[Uart_Tx_Cnt];
  		}
  		else{
  			/*不需要发送数据*/
        Uart_Rx_Flag=UartReturnToReceive;
      }
  		break;
  	}
  	case DataPacketSendWait:{
  		/*wait uart send data complete*/
  		break;
  	}
  	case UartReturnToReceive:{
  		ResetSerialToRxd();
  		if(BackupBaud!=para.baud){
  			BPSset();
  		}
  		/*Uart restart receiver */
  		break;
  	}
  	default :{
      ResetSerialToRxd();
      if(BackupBaud!=para.baud){
  			BPSset();
  		}
  		break;
  	}
  }
}


void ResetSerialToRxd(void)
{
      /*ATM2483 translate is close, receive is open*/
      Adm2483ReceiveEnable;
      //Adm2483SendEnable;
  		Uart_Tx_Cnt=0;
  		Uart_Rx_Cnt=0;
  		Uart_Rx_Length=0;
  		Uart_Tx_Length=0;
  		Uart_Rx_Flag = UartSleep;
  		Com0_Pack_Error=Com0PackOk;
  		ES  =1;/*enable uart int*/
  	  TI=0;
  		RI=0;

  		TXD_RUN =1;
  		RXD_RUN =1;
}
void BPSset(void)
{
	TR1 = 0;
	switch(para.baud){
		case Baud1200:
			TH1 = 0xe8;			// 1200
			TL1 = 0xe8;
			break;
		case Baud2400:
			TH1 = 0xf4;			// 2400
			TL1 = 0xf4;
			break;
		case Baud4800:
			TH1 = 0xfa;			// 4800
			TL1 = 0xfa;
			break;
		case Baud9600:
			TH1 = 0xfd;			// 9600
			TL1 = 0xfd;
			break;
		case Baud14400:
      TH1 = 0xfe;			// 14400
			TL1 = 0xfe;
			break;
		default:
			TH1 = 0xfd;			// 9600
			TL1 = 0xfd;
			break;
		}
		TR1 = 1;
}
/*******************************************
*功能: CRC 计算
参数: 数据指针 PushMsg、数据长度 DataLen
返回: 整数
*******************************************/
unsigned int CRC16(unsigned char *PuchMsg,unsigned int DataLen)
{
  unsigned char ucCRCHi=0xff;
  unsigned char ucCRCLo=0xff;
  unsigned char ucIndex;
  unsigned char ucdat;
  unsigned int uiCRC;
  //WDI_Clr();
  while(DataLen--)
  {
    ucdat=(unsigned char)(*PuchMsg++);
    ucIndex=ucCRCHi^ucdat;
    ucCRCHi=ucCRCLo^CRCHi[ucIndex];
    ucCRCLo=CRCLo[ucIndex];
  }
  uiCRC=ucCRCHi;
  uiCRC=uiCRC<<8;
  uiCRC+=ucCRCLo;
  return (uiCRC);
}


/*************************************
* 功能:CRC 校验
* Ploy=X16+X15+X13+1
* 初始值:0xff
*************************************/

const unsigned char code CRCHi[256]={
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};
const unsigned char code CRCLo[256]={
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

