#include "Data_Bus.h"

void Write_Data(unsigned int wd) {
	GPIO_WriteBit(GPIOD, GPIO_Pin_14,(BitAction)((wd >> 0) & 0x01) );
	GPIO_WriteBit(GPIOD, GPIO_Pin_15,(BitAction)((wd >> 1) & 0x01) );
	GPIO_WriteBit(GPIOD, GPIO_Pin_0,(BitAction)((wd >> 2) & 0x01) );
	GPIO_WriteBit(GPIOD, GPIO_Pin_1,(BitAction)((wd >> 3) & 0x01) );
	GPIO_WriteBit(GPIOE, GPIO_Pin_7,(BitAction)((wd >> 4) & 0x01) );
	GPIO_WriteBit(GPIOE, GPIO_Pin_8,(BitAction)((wd >> 5) & 0x01) );
	GPIO_WriteBit(GPIOE, GPIO_Pin_9,(BitAction)((wd >> 6) & 0x01) );
	GPIO_WriteBit(GPIOE, GPIO_Pin_10,(BitAction)((wd >> 7) & 0x01) );
	GPIO_WriteBit(GPIOE, GPIO_Pin_11,(BitAction)((wd >> 8) & 0x01) );
	GPIO_WriteBit(GPIOE, GPIO_Pin_12,(BitAction)((wd >> 9) & 0x01) );
	GPIO_WriteBit(GPIOE, GPIO_Pin_13,(BitAction)((wd >> 10) & 0x01) );
	GPIO_WriteBit(GPIOE, GPIO_Pin_14,(BitAction)((wd >> 11) & 0x01) );
	GPIO_WriteBit(GPIOE, GPIO_Pin_15,(BitAction)((wd >> 12) & 0x01) );
	GPIO_WriteBit(GPIOD, GPIO_Pin_8,(BitAction)((wd >> 13) & 0x01) );
	GPIO_WriteBit(GPIOD, GPIO_Pin_9,(BitAction)((wd >> 14) & 0x01) );
	GPIO_WriteBit(GPIOD, GPIO_Pin_10,(BitAction)((wd >> 15) & 0x01) );
}
