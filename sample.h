#ifndef __SAMPLE__H
#define __SAMPLE__H

#define TotalSampleTime 5
#define RefVolageFre  (42712)
#define SP_TIMES  (10)

extern unsigned char xdata SampleTime;
extern unsigned char xdata SampleTimeCnt;

extern unsigned char SampleCalcFlag;

extern unsigned char xdata gCountH,gCountL;
extern unsigned char xdata gChannel;
//extern unsigned long xdata gStandardCount;
extern unsigned int xdata SampleTotalCout;
extern unsigned int xdata Samples[SP_TIMES+1];
extern unsigned int xdata gInputValue[CHANNEL_MAX+1];
extern float xdata ChannelVolageValue[CHANNEL_MAX+1];
extern float xdata ChannelAdjustValue[CHANNEL_MAX+1];

void SampleProcess(void);
void SelectChannel( void );

#endif