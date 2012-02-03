#include <LPD8806.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>

void init_lpd(uint8_t n)
{
	pixels = (int *)malloc(n * 3);
	memset(pixels, 0x80, n * 3); // Init to RGB 'off' state
	numLeds = n;
	pause = 3;
	
	init_spi();
	write_zeros(3 * ((numLeds + 63) / 64));
}

void init_spi(void)
{
	DDRB = ((1<<DDB2)|(1<<DDB1)|(1<<DDB0)); //spi pins on port b MOSI SCK,SS outputs
	SPCR = ((1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<CPOL)|(1<<CPHA));  // SPI enable, Master, f/16 
}
 
uint8_t spi_transfer(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)))
		;
	return SPDR; 
}

void write_zeros(uint8_t n)
{
	while(n--) spi_transfer(0);
}

void lpd_show(void)
{
	uint8_t i, n = numLeds * 3;
	
	for (i=0; i < n; i++)
	{
		spi_transfer(pixels[i]);
	}
	
	write_zeros(3 * ((numLeds + 63) / 64));
	
	_delay_ms(pause);
}

void setPixelColor(uint16_t n, uint32_t c) {
  if (n >= numLeds) return; // '>=' because arrays are 0-indexed

  pixels[n*3 ] = (c >> 16) | 0x80;
  pixels[n*3+1] = (c >> 8) | 0x80;
  pixels[n*3+2] = c | 0x80;
}

uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  // Take the lowest 7 bits of each value and append them end to end
  // We have the top bit set high (its a 'parity-like' bit in the protocol
  // and must be set!)
  return 0x808080 | ((uint32_t)g << 16) | ((uint32_t)r << 8) | (uint32_t)b;
}

