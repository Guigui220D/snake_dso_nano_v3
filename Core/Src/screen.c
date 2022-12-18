/*
 * screen.c
 *
 *  Created on: 9 juin 2022
 *      Author: Guillaume
 */

#include "screen.h"

#define IFCTL 0xF6
#define MADCTL 0x36
#define COLMOD 0x3A
#define RAMWR 0x2C
#define PTLON 0x12

#define PASET 0x2B
#define CASET 0x2A

void set_interface() {
	screen_command(IFCTL);
	//screen_param(0);
	screen_param(0b00000000); // TO INVESTIGATE (p192 of the docs, EOR)
	//screen_param(0b00000001);
	screen_param(0b00000000);
	screen_param(0b00000000);
}

void set_mem_access() {
	screen_command(MADCTL);
	screen_param(0b10000000);
}

void screen_init() {
	HAL_GPIO_WritePin(sWR__GPIO_Port, sWR__Pin, 1);
	HAL_GPIO_WritePin(sRD__GPIO_Port, sRD__Pin, 1);
	HAL_GPIO_WritePin(sRS_GPIO_Port, sRS_Pin, 1);

	HAL_GPIO_WritePin(sRST__GPIO_Port, sRST__Pin, 1);
	HAL_Delay(10);
	HAL_GPIO_WritePin(sRST__GPIO_Port, sRST__Pin, 0);
	HAL_Delay(10);
	HAL_GPIO_WritePin(sRST__GPIO_Port, sRST__Pin, 1);
	HAL_Delay(50);

	screen_command(COLMOD);
	screen_param(0b01010101); // 16bpp
	screen_param(0b01010101); // 16bpp


	set_interface();
	//set_mem_access();

	screen_command(SLPOUT);
	screen_command(DISPON);
	//screen_command(DINVON);

	// partial mode
	screen_command(PTLON);
}

void screen_command(unsigned short command) {
	GPIOE->ODR = command;

	HAL_GPIO_WritePin(sRS_GPIO_Port, sRS_Pin, 0);

	HAL_GPIO_WritePin(sWR__GPIO_Port, sWR__Pin, 0);
	HAL_GPIO_WritePin(sWR__GPIO_Port, sWR__Pin, 1);

	HAL_GPIO_WritePin(sRS_GPIO_Port, sRS_Pin, 1);

	HAL_Delay(1);

	screen_param(0);
}

inline void screen_param(unsigned short data) {
	GPIOE->ODR = data;

	GPIOD->BSRR = sWR__Pin;
	GPIOD->BRR = sWR__Pin;


	//HAL_Delay(1);
}

void screen_start_drawing() {
	screen_command(RAMWR);
	//screen_param(0);
}

void screen_set_window(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2) {
	screen_command(PASET);
	screen_param(y1 >> 8);
	screen_param(y1 & 0xFF);
	screen_param(y2 >> 8);
	screen_param(y2 & 0xFF);

	screen_command(CASET);
	screen_param(x1 >> 8);
	screen_param(x1 & 0xFF);
	screen_param(x2 >> 8);
	screen_param(x2 & 0xFF);
}

void screen_reset_window() {
	screen_set_window(0, 0, 0xEF, 0x13F);
}
