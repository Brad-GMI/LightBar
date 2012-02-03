#ifndef LPD8806_h__
#define LPD8806_h__

#include <stdint.h>

void init_lpd(uint8_t n);
void init_spi(void);
uint8_t spi_transfer(uint8_t data);
void write_zeros(uint8_t n);
void lpd_show(void);
//void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
void setPixelColor(uint16_t n, uint32_t c);
uint32_t Color(uint8_t r, uint8_t g, uint8_t b);

uint16_t *pixels;
uint8_t pause;
uint8_t numLeds;

#endif