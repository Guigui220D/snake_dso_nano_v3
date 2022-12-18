/*
 * screen.h
 *
 *  Created on: 9 juin 2022
 *      Author: Guillaume
 */

#ifndef INC_SCREEN_H_
#define INC_SCREEN_H_

#include "main.h"

#define SLPOUT 0x11
#define DINVON 0x21
#define DISPON 0x29

#define RGB(_R,_G,_B) (((_R & 0x3E) >> 1) | ((_G & 0x3f) << 5) | ((_B & 0x3e) << 10))

void screen_init(void);
void screen_command(unsigned short command);
void screen_param(unsigned short param);
void screen_start_drawing();
void screen_set_window(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void screen_reset_window();
void set_cursor(unsigned short x, unsigned short y);

#endif /* INC_SCREEN_H_ */
