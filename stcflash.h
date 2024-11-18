#ifndef STCFLASH__H
#define STCFLASH__H

/*STC FLASH DEFINE*/
//#define ENABLE_ISP 0x83      //<12MHz
//#define DATA_FLASH_START_ADDRESS  0x8200  //stc90c58ad

#define StcFlashSec0Add      0
#define StcFlashSec1Add      512


#define EEPROMCHECK    (0x20121215)

/*
extern unsigned int DLT645DataAddress;
extern unsigned char DLT645DataLength;
extern unsigned int xdata StartAddressOfData[13];
extern const unsigned int code DataFileLengthTable[13][2];
*/
#define DListTotalLength   16

#define DListVersionIDAddr       0
#define DListDeviceAddrAddr      4
#define DListCommBaudAddr        5
#define DListRelayStatusKeep     6
#define DListDOStatusAddr             7
#define DListDIStaAvailableTime       9

#define DListWdgTimeForExternalMach      11
//===========================================================



#define NOP _nop_();_nop_();_nop_();_nop_();

#define ADDRADJUST     0x8200
#define ADDRDS18B20ID     0x8400
#define ADDRPARA     0x8600
/* 定义命令 */
#define READ_AP_and_Data_Memory_Command				0x01        /*  字节读数据存储区   */
#define PROGRAM_AP_and_Data_Memory_Command          0x02        /*  字节编程数据存储区 */
#define SECTOR_ERASE_AP_and_Data_Memory_Command     0x03        /*  扇区擦除数据存储区 */

typedef     unsigned char	INT8U;		/* 8 bit 无符号整型  */
typedef     unsigned int    INT16U;     /* 16 bit 无符号整型 */
#define		DELAY_CONST         60000

/* 定义常量 */
#define ERROR   0
#define OK      1


/* 定义Flash 操作等待时间 */
//#define        MCU_CLOCK_40MHz
//#define         MCU_CLOCK_20MHz
#define        MCU_CLOCK_10MHz
//#define        MCU_CLOCK_5MHz
#ifdef MCU_CLOCK_40MHz
        #define WAIT_TIME        0x00
#endif
#ifdef MCU_CLOCK_20MHz
        #define WAIT_TIME        0x01
#endif
#ifdef MCU_CLOCK_10MHz
        #define WAIT_TIME        0x02
#endif
#ifdef MCU_CLOCK_5MHz
        #define WAIT_TIME        0x03
#endif

/* 调试控制项 */
#define         DEBUG_STC89C_LE58RD

//#define         DEBUG_STC89C_LE52RC

//#define USED_BYTE_QTY_IN_ONE_SECTOR                1
//#define USED_BYTE_QTY_IN_ONE_SECTOR                2
//#define USED_BYTE_QTY_IN_ONE_SECTOR                4
//#define USED_BYTE_QTY_IN_ONE_SECTOR                8
//#define USED_BYTE_QTY_IN_ONE_SECTOR                16
//#define USED_BYTE_QTY_IN_ONE_SECTOR                32
//#define USED_BYTE_QTY_IN_ONE_SECTOR                64
//#define USED_BYTE_QTY_IN_ONE_SECTOR                128
//#define USED_BYTE_QTY_IN_ONE_SECTOR                256
#define USED_BYTE_QTY_IN_ONE_SECTOR                512

//INT8U xdata protect_buffer[USED_BYTE_QTY_IN_ONE_SECTOR];

#ifdef DEBUG_STC89C_LE58RD                        //STC89C58RD+,  89LE58RD+
        #define DEBUG_AP_Memory_Begin_Sector_addr		0x0000
        #define DEBUG_AP_Memory_End_Sector_addr         0x7e00
        #define DEBUG_AP_Memory_End_Byte_addr           0x7fff

        #define DEBUG_Data_Memory_Begin_Sector_addr     0x8000
#endif
//=============================================================

void ISP_IAP_enable(void);
void ISP_IAP_disable(void);
void StcResetCheck(void);
uchar byte_read(uint byte_addr);
//uchar Byte_Read(uint address);
//void Sector_Erase(uint address);
//void Byte_Program(uint address,uchar ch);

//void MoreByteProg(uint add,uchar ByteAmount,uchar *ptr);
//void Moreyte_Read(  unsigned int adr,
//            unsigned char size,
//            unsigned char *dat );
void eep_write_sector(uint begin_addr, uint counter, uchar array[]);

//void DataInitFromStcflash(void);
//void SaveDatToStcflash(unsigned char SaveDatFlag);
void RdPara(void);
void WrPara(void);
void WrAdjust(void);
void RdAdjust(void);
void RomInit(void);



/*
void GetDListStartAddressPoweron(void);
void GetStartAddrAndLength(unsigned int Index);
*/
#endif