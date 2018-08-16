#include "USART.h"
#include "GloablVar.h"

void DealMLStat(u8 bNum, USART_DATA *usart_data) {
	if (oldStatMLB[bNum-41][0] != usart_data->rdBuff[1] || oldStatMLB[bNum-41][1] != usart_data->rdBuff[2]) {
			newStatMLB[wrPtrNewML].bNum = bNum-41;
			newStatMLB[wrPtrNewML].stat1 = usart_data->rdBuff[1];
			newStatMLB[wrPtrNewML].stat2 = usart_data->rdBuff[2];
			wrPtrNewML++;
	}
}

