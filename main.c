#define F_CPU 7372800UL

#include <time.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "SSD1306.h"

uint8_t kom_x;
uint8_t kom_y;
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

	kom_x = rand() % 128;
	kom_y = 0;
	player_x = 64;
	player_y = 7;
}

void draw_screen(uint8_t kx, uint8_t ky, uint8_t px, uint8_t py) {
	OLED_Clear();
	OLED_SetCursor(ky, kx);
	OLED_Printf("O");
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
		

		if(kom_x >= player_x && kom_x <= (player_x + 4) && (kom_y == player_y)) game_over();


		draw_screen(kom_x, kom_y, player_x, player_y);

		kom_y++;
		if(kom_y == 8){
			kom_y = 0;
			kom_x = rand() % 128;
		}
		
	}
}
