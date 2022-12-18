/*
 * random.c
 *
 *  Created on: 22 juin 2022
 *      Author: Guillaume
 */

#include "random2.h"

static unsigned int xorshift_state;

unsigned int xorshift() {
	unsigned int x = xorshift_state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return xorshift_state = x;
}

void xorshift_set_state(unsigned int st) {
	if (st == 0)
		st++;
	xorshift_state = st;
}
