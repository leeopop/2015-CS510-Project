//-----------------------------------------------------------------
//                           AltOR32 
//                Alternative Lightweight OpenRisc 
//                            V2.0
//                     Ultra-Embedded.com
//                   Copyright 2011 - 2013
//
//               Email: admin@ultra-embedded.com
//
//                       License: LGPL
//-----------------------------------------------------------------
//
// Copyright (C) 2011 - 2013 Ultra-Embedded.com
//
// This source file may be used and distributed without         
// restriction provided that this copyright statement is not    
// removed from the file and that any derivative work contains  
// the original copyright notice and the associated disclaimer. 
//
// This source file is free software; you can redistribute it   
// and/or modify it under the terms of the GNU Lesser General   
// Public License as published by the Free Software Foundation; 
// either version 2.1 of the License, or (at your option) any   
// later version.
//
// This source is distributed in the hope that it will be       
// useful, but WITHOUT ANY WARRANTY; without even the implied   
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      
// PURPOSE.  See the GNU Lesser General Public License for more 
// details.
//
// You should have received a copy of the GNU Lesser General    
// Public License along with this source; if not, write to the 
// Free Software Foundation, Inc., 59 Temple Place, Suite 330, 
// Boston, MA  02111-1307  USA
//-----------------------------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "top_if.h"

#include "altor32__Syms.h"
#include "verilated.h"

#include <E/E_System.hpp>
#include "module.hpp"

#if VM_TRACE
#include <verilated_vcd_c.h>
#endif

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define MEMORY_START        0x10000000
#define MEMORY_SIZE         (1024 * 1024 * 1024)

#define OR32_BUBBLE_OPCODE  0xFC000000

#define CPU_INSTANCE        top->v->u_cpu->u1_cpu

//-----------------------------------------------------------------
// Locals
//-----------------------------------------------------------------
static altor32 *top;
static unsigned int         _stop_pc = 0xFFFFFFFF;
static E::System* sim_system;
static VRunner* runner;

#if VM_TRACE
static unsigned int        main_time = 0;
static VerilatedVcdC*      tfp;
#endif

//-----------------------------------------------------------------
// top_init
//-----------------------------------------------------------------
int top_init(void)
{
    top = new altor32();
    sim_system = new E::System;
    runner = new VRunner((size_t) MEMORY_START, (size_t)MEMORY_SIZE, top, sim_system);

#if VM_TRACE                  
    // If verilator was invoked with --trace
    Verilated::traceEverOn(true);
    VL_PRINTF("Enabling GTKWave Trace Output...\n");
    tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("wave_dump.vcd");
#endif

    // Initial
    top->clk_i = 0;
    top->rst_i = 1;
    top->intr_i = 0;
    top->eval();

    // Reset
    top->clk_i = 1;
    top->rst_i = 1;
    top->eval();

    top->clk_i = 0;
    top->rst_i = 0;
    top->eval();

    return 0;
}

//-----------------------------------------------------------------
// top_load
//-----------------------------------------------------------------
int top_load(unsigned int addr, unsigned char val)
{
    if (addr >= (MEMORY_SIZE - MEMORY_START))
        return -1;

    addr -= MEMORY_START;    

    runner->writeAt(addr, val);

    return 0;
}
//-----------------------------------------------------------------
// top_setbreakpoint
//-----------------------------------------------------------------
int top_setbreakpoint(int bp, unsigned int pc)
{
    if (bp != 0)
        return -1;
    else
    {
        _stop_pc = pc;
        return 0;
    }
}
//-----------------------------------------------------------------
// top_run
//-----------------------------------------------------------------

int top_run(unsigned int pc, int cycles, int intr_after_cycles)
{

	sim_system->run(intr_after_cycles);

    printf("Cycles = %lu\n", sim_system->getCurrentTime() + 1);

    // Fault
    if (top->fault_o)
    {
        printf("FAULT PC!\n");
        return TOP_RES_FAULT;
    }
    // Number of cycles reached
    else if (sim_system->getCurrentTime() >= cycles)
        return TOP_RES_MAX_CYCLES;
    // No error
    else
        return TOP_RES_OK;
}
//-----------------------------------------------------------------
// top_done
//-----------------------------------------------------------------
void top_done(void)
{
    top->final();
    delete runner;
    delete sim_system;
#if VM_TRACE
    if (tfp)
    {
        tfp->close();
        tfp = NULL;
    }
#endif
}
