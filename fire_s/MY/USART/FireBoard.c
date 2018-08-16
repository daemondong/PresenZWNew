#include "USART.h"
#include "GloablVar.h"

extern IOSTAT  ioStat[1200] ;   //¹²5Àà ±¨¾¯

void DealFireStat(u8 bNum, USART_DATA *usart_data) {
	u8 tmpVal,i;
	
	newStatFireB[bNum-31] = usart_data->rdBuff[1];
	if (newStatFireB[bNum-31] != oldStatFireB[bNum-31]) {
		tmpVal= newStatFireB[bNum-31] ^ oldStatFireB[bNum-31];
		i=0;
		while (tmpVal!=0) {
			if ((tmpVal & 0x01) !=0)
				SetioStat(myConf1.mNum, bNum, 0, 0, i);   //»ð¾¯
			i++;
			tmpVal = tmpVal >> 1;
		}
		oldStatFireB[bNum-31]=newStatFireB[bNum-31];
		dispLine=3;
	}
}

