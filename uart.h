#ifndef UART__H
#define UART__H

/*Uart Define*/
#define CRC16HiLo 1
#define CRC16LoHi 2


#define Adm2483ReceiveEnable      En485=0
#define Adm2483SendEnable      En485=1


enum
{
	U_BAT1_Enum = 40001,
	U_BAT2_Enum,
	U_BAT3_Enum,
	U_BAT4_Enum,
	U_BAT5_Enum,
	U_BAT6_Enum,
	U_BAT7_Enum,
	U_BAT8_Enum,
	U_BAT9_Enum,
	U_BAT10_Enum,
	U_BAT11_Enum,
	U_BAT12_Enum,
	U_BAT13_Enum,
	U_BAT14_Enum,
	U_BAT15_Enum,
	U_BAT16_Enum,
	U_BAT17_Enum,
	U_BAT18_Enum,
	U_BAT19_Enum,
	U_BAT20_Enum,
	U_BAT21_Enum,
	U_BAT22_Enum,
	U_BAT23_Enum,
	U_BAT24_Enum,
	U_BAT25_Enum,
	U_BAT26_Enum,
	U_BAT27_Enum,
	U_BAT28_Enum,
	U_BAT29_Enum,
	U_BAT30_Enum,
	U_BAT31_Enum,
	U_BAT32_Enum,
	U_BAT33_Enum,
	U_BAT34_Enum,
	U_BAT35_Enum,
	U_BAT36_Enum,
	U_BAT37_Enum,
	U_BAT38_Enum,
	U_BAT39_Enum,
	U_BAT40_Enum,
	U_BAT41_Enum,
	U_BAT42_Enum, //total volage
	U_BAT43_Enum, //current
	U_BAT44_Enum, //refernce volage
	TEPM1_Enum,
	TEPM2_Enum,





	DeviceAddr_Enum = 40301,
	CommBaud_Enum,
	Undefine_Enum,
	BAT41_Version_Enum,

	EvenCHCalibration =40401,
  OddCHCalibration,
  //for dido mechine
	DI0_Enum,
	DI1_Enum,
	DI2_Enum,
	DI3_Enum,
	DI4_Enum,
  DI5_Enum,
	DI6_Enum,
	DI7_Enum,
	DI8_Enum,
	DI9_Enum,
	DI10_Enum,
	DI11_Enum,
	DI12_Enum,
	DI13_Enum,
	DI14_Enum,
	DI15_Enum,


	DO1_Enum,
	DO2_Enum,
	DO3_Enum,
	DO4_Enum,
  DO5_Enum,
	DO6_Enum,
	DO7_Enum,
	DO8_Enum,
	DO9_Enum,
	DO10_Enum,
	DO11_Enum,
	DO12_Enum,
	DO13_Enum,
	DO14_Enum,
	DO15_Enum,
	DO16_Enum,

	//DeviceAddr_Enum,
	//CommBaud_Enum,
	DOStatusKeep_Enum,
	DIStaAvailableTime_Enum,

};
enum
{
	Baud1200,
	Baud2400,
	Baud4800,
	Baud9600,
	Baud14400,
	//Baud19200,
};
enum
{
	UartSleep,
	DataPacketReceived,
	DataPacketOk,
	DataPacketProcess,
	DataPacketSendWait,
	UartReturnToReceive,
};
enum
{
	Com0PackOk,
	Com0PackErr,
	Com0PackOkButDatAddrOutside,
	Com0PackOkButDatOutside,
};

extern const unsigned char code CRCHi[256];
extern const unsigned char code CRCLo[256];


/*******************************************
*串口0 对应变量
********************************************/
extern  unsigned char Uart_Rx_Cnt;			//串口接收数据计数
extern  unsigned int Uart_Tx_Cnt;			//发送数据长度
extern  unsigned int Uart_Rx_Length;		//接收数据包长度
extern  unsigned int Uart_Tx_Length;		//发送数据包长度
extern  unsigned char xdata Uart_Rx_Buf[100];
extern  unsigned char xdata Uart_Tx_Buf[100];

extern  unsigned char Com0_Pack_Error;
extern  unsigned char Uart_Length_Flag;
extern  unsigned char Uart_Rx_Flag;
//extern  unsigned int Constant_Angle;

extern  unsigned char Uart_Read_Flag;
extern  unsigned char Uart_Write_Flag;
//extern  unsigned int icount;//leo add
extern  unsigned char CRC16flag;

//function assert

void init_serialcomm( void );
void serial ();
void UartReceiveProcess(void);
void LoadCoilStatus(unsigned int StartAddress, unsigned int Numbers, unsigned char *buf);
void LoadCoilStatusAnswer(unsigned int RegisterNumbers);
void LoadDIStatus(unsigned int StartAddress, unsigned int Numbers, unsigned char *buf);
void LoadDIStatusAnswer(unsigned int RegisterNumbers);
void LoadKeepRegister(unsigned int StartAddress, unsigned int Numbers, unsigned char *buf);
void LoadKeepAnswer(unsigned int RegisterNumbers);

void SignalCoilControlAnswer(void);
void SignalCoilControl(unsigned int StartAddress,unsigned char *buf);

void ForceMultipleCoilAnswer(void);
void ForceMultipleCoil(unsigned int StartAddress,unsigned int Numbers,unsigned char *buf);

void PresetSignalRegAnswer(void);
unsigned char PresetSignalReg(unsigned int StartAddress,unsigned char *buf);
void ModelbusWriteAnswer(void);
void SaveDataToEEPROM(uint Address,uint value);
unsigned char SaveRegister(unsigned int StartAddress,unsigned int Numbers,unsigned char *buf);

void UartDataProcess(void);
void ResetSerialToRxd(void);
void BPSset(void);
unsigned int CRC16(unsigned char *PuchMsg,unsigned int DataLen);






#endif