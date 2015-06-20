/*
 * module.cpp
 *
 *  Created on: Jun 21, 2015
 *      Author: leeopop
 */


#include "module.hpp"

using namespace E;

VRunner::VRunner(size_t memory_start, size_t memory_size, altor32* cpu, System* system) : Module(system)
{
	this->cpu = cpu;
	Message* m = new Message;
	m->type = NEXT_CLOCK;
	this->sendMessage(this, m, 1);
	this->memory = new uint8_t[memory_size];
	memset(this->memory, 0, memory_size);
	this->memory_start = memory_start;
}

VRunner::~VRunner()
{
	delete this->memory;
}

void VRunner::writeAt(size_t addr, uint8_t val)
{
	//printf("write %x, %x\n", addr, val);
	this->memory[addr] = val;
}

Module::Message* VRunner::messageReceived(Module* from, Module::Message* message)
{
	Message *runMessage = dynamic_cast<Message*>(message);

	switch(runMessage->type)
	{
	case NEXT_CLOCK:
	{
		// CLK->L
		cpu->clk_i = 0;
		cpu->eval();

		int prev_memclk = cpu->imem_cyc;

		// CLK->H
		cpu->clk_i = 1;
		cpu->eval();

		printf("%d %d, %d, %d, %d, %d\n", cpu->imem_ack,
				cpu->imem_stb, prev_memclk, cpu->imem_cyc, cpu->imem_cti, cpu->imem_sel
		);

		if(cpu->imem_stb == 1 && cpu->imem_cyc == 1)
		{
			size_t addr = cpu->imem_addr - this->memory_start;
			uint32_t sum = this->memory[addr];
			sum <<= 8;
			sum += this->memory[addr+1];
			sum <<= 8;
			sum += this->memory[addr+2];
			sum <<= 8;
			sum += this->memory[addr+3];

			cpu->imem_data=sum;
			cpu->imem_ack=1;
			cpu->imem_stall=0;
		}
		else if(cpu->imem_cti != 2)
		{
			//cpu->imem_stall=0;
			//cpu->imem_ack=0;
		}

		printf("%x %x %x\n", cpu->dmem_address, cpu->imem_addr, cpu->imem_data);


		if(Verilated::gotFinish() || cpu->fault_o || cpu->break_o)
			break;
		else
		{
			Message* m = new Message;
			m->type = NEXT_CLOCK;
			this->sendMessage(this, m, 1);
		}
		break;
	}
	}
	return nullptr;
}
void VRunner::messageFinished(Module* to, Module::Message* message, Module::Message* response)
{
	assert(response == nullptr);
	delete message;
}
void VRunner::messageCancelled(Module* to, Module::Message* message)
{
	delete message;
}
