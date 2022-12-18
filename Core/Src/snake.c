/*
 * snake.c
 *
 *  Created on: Jun 13, 2022
 *      Author: Guillaume
 */

#include "snake.h"
#include "screen.h"
#include "drawing.h"
#include "random2.h"

#define CHERRYCOL1 0b00000001111111111
#define CHERRYCOL2 0xFFFF

static unsigned int snake_score = 0;

const unsigned short cherry_colors[] = {
		0b0000011000000000,
		0,
		CHERRYCOL1,
		CHERRYCOL2,
};
const unsigned short cherry_sprite[] = {
	0x5540,0x0005,0x0410,0x0550,0x0400,
	0x1ba4,0x1050,0x1aa5,0x55a4,0x6a9a,
	0xaaa4,0x6a9b,0xaea9,0x6a6f,0xaaa9,
	0x1a6f,0xaaa9,0x1a6e,0xaaa9,0x066a,
	0xaaa4,0x015a,0xaaa4,0x001a,0x9a50,
	0x0005,0x4500,0x0000,
};

const unsigned char head_sprite[8] = {
	0b01111110,
	0b10000001,
	0b10100101,
	0b10100101,
	0b10011001,
	0b10111101,
	0b10000001,
	0b01111110,
};

void snake_draw_part(unsigned short x, unsigned short y) {
	x *= 8;
	y *= 8;
	screen_set_window(x, y, x + 7, y + 7);
	screen_start_drawing();
	for (int i = 0; i < 7; i++) {
		screen_param(0);
		for (int j = 0; j < 8; j++)
			screen_param(0b0000011111111111);
	}
	screen_param(0);
}

void snake_draw_head(unsigned short x, unsigned short y) {
	x *= 8;
	y *= 8;
	screen_set_window(x, y, x + 7, y + 7);
	screen_start_drawing();
	for (int i = 0; i < 8; i++) {
		unsigned char line = head_sprite[i];
		for (int j = 0; j < 8; j++) {
			if (line & 1) {
				screen_param(0);
			} else {
				screen_param(0b00000001111111111);
			}
			line >>= 1;
		}
	}
}

void cherry_draw(unsigned short x, unsigned short y) {
	x *= 16;
	y *= 16;
	y += 2;
	screen_set_window(x, y, x + 15, y + 13);
	screen_start_drawing();
	for (int i = 0; i < 28; i++) {
		unsigned int data = cherry_sprite[i];
		for (int j = 0; j < 8; j++) {
			char c = data & 0b11;
			data >>= 2;
			screen_param(cherry_colors[c]);
		}
	}
}

void cherry_erase(unsigned short x, unsigned short y) {
	x *= 16;
	y *= 16;
	y += 2;
	redraw_bg(x, y, x + 15, y + 13);
}

void snake_erase_part(unsigned short x, unsigned short y) {
	x *= 8;
	y *= 8;
	redraw_bg(x, y, x + 7, y + 7);
}

int snake_check_collision(const snake_t * snake, unsigned char newx, unsigned char newy) {
	if (snake->head == snake->tail)
		return 0;
	if (snake->head > snake->tail) {
		for (unsigned int i = snake->tail; i <= snake->head; i++) {
			if (snake->parts[i].x == newx && snake->parts[i].y == newy)
				return 1;
		}
		return 0;
	} else {
		for (unsigned int i = snake->tail; i < SNAKE_MAX_LEN; i++) {
			if (snake->parts[i].x == newx && snake->parts[i].y == newy)
				return 1;
		}
		for (unsigned int i = 0; i <= snake->head; i++) {
			if (snake->parts[i].x == newx && snake->parts[i].y == newy)
				return 1;
		}
		return 0;
	}
}

int snake_update(snake_t * snake) {
	//cherry_draw(0, 0);
	if (!snake->to_grow) {
		snake_erase_part(snake->parts[snake->tail].x, snake->parts[snake->tail].y);
		if (snake->parts[snake->tail].x >> 1 == snake->cx && snake->parts[snake->tail].y >> 1 == snake->cy) {
			cherry_draw(snake->cx, snake->cy);
		}
		snake->tail++;
	} else {
		snake->to_grow--;
		snake_score++;
		//if (snake->to_grow == 0)
			draw_score(snake_score);
	}
	if (snake->tail == SNAKE_MAX_LEN)
		snake->tail = 0;
	unsigned char head_x = snake->parts[snake->head].x;
	unsigned char head_y = snake->parts[snake->head].y;
	char dir = snake->dir;
	if (dir == 0) {
		head_x++;
		if (head_x >= 30)
			head_x -= 30;
	}
	if (dir == 1) {
		head_y++;
		if (head_y >= 40)
			head_y -= 38;
	}
	if (dir == 2) {
		if (head_x == 0)
			head_x = 30;
		head_x--;
	}
	if (dir == 3) {
		if (head_y == 2)
			head_y = 40;
		head_y--;
	}
	if (snake_check_collision(snake, head_x, head_y))
		return 1;
	snake_draw_part(snake->parts[snake->head].x, snake->parts[snake->head].y);
	if (head_x >> 1 == snake->cx && head_y >> 1 == snake->cy) {
		snake->to_grow += 3;
		cherry_erase(snake->cx, snake->cy);
		snake->cx = xorshift() & 0b1111;
		snake->cy = xorshift() & 0b11111;
		if (snake->cx >= 15)
			snake->cx -= 15;
		if (snake->cy >= 20)
			snake->cy -= 20;
		if (snake->cx == 0)
			snake->cx = 14;
		if (snake->cy == 0)
			snake->cy = 19;
		cherry_draw(snake->cx, snake->cy);
	}
	snake->head++;
	if (snake->head == SNAKE_MAX_LEN)
		snake->head = 0;
	snake_draw_head(head_x, head_y);
	snake->parts[snake->head].x = head_x;
	snake->parts[snake->head].y = head_y;
	return 0;
}

void snake_reset_score() {
	draw_score(snake_score);
	snake_score = 0;

}

unsigned int get_score() {
	return snake_score;
}
