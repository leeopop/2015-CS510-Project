/*
 * library.c
 *
 *  Created on: Jun 20, 2015
 *      Author: leeopop
 */


#include "library.h"

void stop(void)
{
	asm volatile ("l.trap 0");
}

unsigned int * exception(unsigned int *registers, unsigned int reason)
{
	return registers;
}
