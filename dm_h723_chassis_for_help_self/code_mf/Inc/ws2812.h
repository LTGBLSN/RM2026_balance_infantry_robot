//
// Created by 21481 on 2026/1/29.
//

#ifndef DM_H723_LIB_WS2812_H
#define DM_H723_LIB_WS2812_H

#include "main.h"


#define WS2812_SPI_UNIT     hspi6
extern SPI_HandleTypeDef WS2812_SPI_UNIT;

void WS2812_Ctrl(uint8_t r, uint8_t g, uint8_t b);


#endif //DM_H723_LIB_WS2812_H
