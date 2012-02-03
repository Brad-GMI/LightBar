#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <LPD8806.h>
#include <stdlib.h>
#include <stdint.h>
#include <uart.h>

#define BAUD_RATE 19200

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

void colorChase(uint32_t c, uint8_t wait);
uint32_t get_color(uint8_t c);

int main(void)
{
	uint8_t cmd_pos;
	uint8_t valid;
	
	CPU_PRESCALE(0);  // run at 16 MHz
	uart_init(BAUD_RATE);
	
	init_lpd(32);
	
	lpd_show();
	
	colorChase(Color(0,0,127), 10);
	
	while (1)
	{			
		if (uart_available())
		{
			uint8_t c = uart_getchar();
			
			if (c=='M')
			{
				cmd_pos = 0;
				valid = 0;
			}
			else if ((c >= '0') && (c <= '9'))
			{
				setPixelColor(cmd_pos, get_color(c));
				cmd_pos++;
			}
			else if (c == '!')
			{
				valid = 1;
			}
			else
			{
				cmd_pos = 0;
			}
		}
		
		if (valid)
		{
			lpd_show();
			valid = 0;
		}
		
	}
}

void colorChase(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < numLeds; i++) {
    setPixelColor(i, 0); // turn all pixels off
  }
  
  for (i=0; i < numLeds; i++) {
      setPixelColor(i, c);
      if (i == 0) {
        setPixelColor(numLeds-1, 0);
      } else {
        setPixelColor(i-1, 0);
      }
      lpd_show();
      _delay_ms(wait);
  }
}

uint32_t get_color(uint8_t c)
{
	uint32_t temp;
	
	switch (c - '0')
	{
		case 0:
			//black
			temp = 0;
			break;
		case 1:
			//blue	
			temp = Color(0, 0, 127);
			break;	
		case 2:
			//green
			temp = Color(0,127,0);
			break;
		case 3:
			//teal
			temp = Color(0,127,127);
			break;
		case 4:
			//red
			temp = Color(127,0,0);
			break;
		case 5:
			//violet
			temp = Color(127,0,127);
			break;
		case 6:
			//yellow
			temp = Color(127,127,0);
			break;
		case 7:
			//white
			temp = Color(127,127,127);
			break;
		case 8:
			temp = 0;
			break;
		case 9:
			temp = 0;
			break;
		default:
			temp = 0;
			break;
	}
	
	return temp;
}