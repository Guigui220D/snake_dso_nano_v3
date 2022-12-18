/*
 * snake.h
 *
 *  Created on: Jun 13, 2022
 *      Author: Guillaume
 */

#ifndef INC_SNAKE_H_
#define INC_SNAKE_H_

#define SNAKE_MAX_LEN 512

typedef struct {
	char x, y;
} snake_part_t;

typedef struct {
	snake_part_t parts[SNAKE_MAX_LEN];
	unsigned int head, tail;
	char dir;
	char to_grow;
	char cx, cy;
} snake_t;

int snake_update(snake_t * snake);
void snake_reset_score();
unsigned int get_score();

#endif /* INC_SNAKE_H_ */
