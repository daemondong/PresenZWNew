#include "USART.h"
#include "GloablVar.h"

extern IOSTAT  ioStat[1200] ;   //共5类 报警
extern IOSTAT  errStat[1200] ;   //共5类 故障


void DealPowerStat(u8 bNum, u8 status) {
	u8 tmpVal,i;
	
	if (newStatPB != status) {
		if (status & 0x01) MAINPOWERON;
		else MAINPOWEROFF;
		if (status & 0x02) BACKPOWERON;
		else BACKPOWEROFF;
		
		if (status & 0x04) MAINPOWERERROR;
		else MAINPOWERERROROFF;
		if (status & 0x08) BACKPOWERERROR;
		else BACKPOWERERROROFF;

		tmpVal= ((newStatPB ^ status) & 0xFC);
		i=0;
		while (tmpVal!=0) {
			if ((tmpVal & 0x01) !=0) {
				if (((status >> i) & 0x01) == 1)
					SeterrStat(myConf1.mNum, bNum, 0, 0, 200+i);
				else
					DelerrStat(myConf1.mNum,bNum, 0, 0, 200+i);
			}
			i++;
			tmpVal = tmpVal >> 1;
		}
	  newStatPB = status;
	}
}

