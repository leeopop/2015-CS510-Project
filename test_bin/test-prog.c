/*
 * test.c
 *
 *  Created on: Jun 20, 2015
 *      Author: leeopop
 */

#include "serial.h"
#include "cache.h"
#include "irq.h"
#include "mem_map.h"
#include "library.h"

char __buffer[1024];

#define float_buffer_size (sizeof(__buffer)/sizeof(float))
#define char_buffer_size (sizeof(__buffer)/sizeof(char))
#define int_buffer_size (sizeof(__buffer)/sizeof(int))

int test_main()
{
	float* float_buffer = (float*)__buffer;
	char* char_buffer = (char*)__buffer;
	int* int_buffer = (int*)__buffer;

	serial_putstr("Hello world.\n");
	int k;
	for(k=0; k<10; k++)
	{
		serial_putstr("Loop!\n");
	}
	for(k=0; k<sizeof(__buffer); k++)
		__buffer[k] = (char)k;
	for(k=0; k<(char_buffer_size); k++)
		char_buffer[k] *= char_buffer[k];
	for(k=0; k<(int_buffer_size); k++)
		int_buffer[k] *= int_buffer[k];
	for(k=0; k<(float_buffer_size); k++)
		float_buffer[k] += float_buffer[k];

	serial_putstr("Bye world.\n");
	stop();
	return 0;
}
