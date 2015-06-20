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

char buffer[1024*16];

int test_main()
{
	serial_putstr("Hello world.\n");
	int k;
	for(k=0; k<10; k++)
	{
		serial_putstr("Loop!\n");
	}
	for(k=0; k<sizeof(buffer); k++)
		buffer[k] = 1;
	if(buffer[0] > 0)
		serial_putstr("Bye world.\n");
	stop();
	return 0;
}
