/*
 * drawing.h
 *
 *  Created on: 13 juin 2022
 *      Author: Guillaume
 */

#ifndef INC_DRAWING_H_
#define INC_DRAWING_H_

void draw_background(void);
void redraw_bg(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void draw_menu(int selected);
void erase_menu();
void draw_char(unsigned short x, unsigned short y, char c);
void draw_string(unsigned short x, unsigned short y, const char * str);
void draw_score(unsigned int i);
void draw_header();

#endif /* INC_DRAWING_H_ */
