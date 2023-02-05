#define F_CPU 7372800UL

#include <time.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "SSD1306.h"

uint8_t kom[8][2];
uint16_t brojKometa;
uint8_t player_x;
uint8_t player_y;

void delay(uint16_t timeout) {
	uint16_t i;
	for(i = 0; i < timeout; i++) {
		_delay_ms(1);
	}
}

void initialize_lcd(void) {
	// Initialize LCD here
	// Example:
	OLED_Init();
	OLED_Clear();
	brojKometa = 1;
	kom[0][0] = rand() % 122;
	kom[0][1] = 0;
	//kom_x = rand() % 128;
	//kom_y = 0;
	player_x = 64;
	player_y = 7;
}

void draw_screen(uint8_t kom[][2], uint8_t px, uint8_t py) {
	OLED_Clear();
	for (uint8_t i = 0; i < brojKometa; i++)
	{	OLED_SetCursor(kom[i][1], kom[i][0]);
		OLED_Printf("O");
	}
	OLED_SetCursor(py, px);
	OLED_Printf("x");
}

void game_over () {
	OLED_Clear();
	OLED_SetCursor(3, 36);
	OLED_Printf("Game over");
	_delay_ms(2000);
	initialize_lcd();
}

ISR(INT0_vect) {
	player_x -= 5;
	if(player_x >= 123) player_x = 0;
	
	debounce();
}

ISR(INT1_vect) {

	player_x += 5;
	if(player_x >= 123) player_x = 122;

	debounce();
}

void debounce() {
	_delay_ms(200);
	GIFR = _BV(INTF0) | _BV(INTF1);
}


int main(void)
{
	PORTB = _BV(0) | _BV(1);
	DDRB = 0x00;
	
	MCUCR = _BV(ISC01) | _BV(ISC11);
	GICR = _BV(INT0) | _BV(INT1);
	sei();
	
	srand(time(NULL));

	initialize_lcd();

	while (1) {
		

		for (uint8_t i = 0; i < brojKometa; i++)
		{	
			if(kom[i][0] >= (player_x - 4) && kom[i][0] <= (player_x + 4) && (kom[i][1] == player_y)) 

game_over();
		}

		draw_screen(kom, player_x, player_y);

		for (uint8_t i = 0; i < brojKometa; i++)
		{	kom[i][1]++;
		}
		if(kom[0][1] == 8){
			
			if (brojKometa <= 7) brojKometa++;
			
			for (uint8_t i = 0; i < brojKometa; i++)
			{	kom[i][1] = 0;
				kom[i][0] = rand() % 122;
			}
		}
		
	}
}
