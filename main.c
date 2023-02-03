#define F_CPU 7372800UL

#include <time.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "SSD1306.h"


int main(void)
{

		OLED_Init();
		OLED_Clear();
		OLED_SetCursor(4, 30);
		OLED_Printf("Hello world!");

}