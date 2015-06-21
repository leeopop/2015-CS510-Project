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

int int_buffer[16];
#define int_buffer_size (sizeof(int_buffer)/sizeof(int))

int test_main()
{

	serial_putstr("Hello world.\n");
	int k;
	for(k=0; k<10; k++)
	{
		serial_putstr("Loop!\n");
	}

	for(k=0; k<int_buffer_size; k++)
		int_buffer[k] = k;

	for(k=0; k<(int_buffer_size); k++)
	{
		int j = int_buffer[k];
		j = j * j;
		int_buffer[k] = j;
	}

	for(k=0; k<(int_buffer_size); k++)
	{
		char print = (char)int_buffer[k];
		print = print % 10;
		serial_putchar(print + '0');
	}
	serial_putchar('\n');

	serial_putstr("Bye world.\n");
	stop();
	return 0;
}
