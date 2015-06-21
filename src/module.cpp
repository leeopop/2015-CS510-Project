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

#if 1
		//if(cpu->imem_cyc == 1)
		{
			if(cpu->imem_stb == 1)
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
				//cpu->imem_stall=0;
				printf("Inst addr:%x, Inst content: %x, Sel: %d\n",
						cpu->imem_addr, cpu->imem_data, cpu->imem_sel);
			}
			else
			{
				//cpu->imem_stall=0;
				cpu->imem_ack=0;
			}
		}

		//if(cpu->dmem_cyc == 1)
		{
			if(cpu->dmem_stb == 1)
			{
				if(cpu->dmem_we ==1)
				{
					size_t addr = cpu->dmem_address - this->memory_start;
					uint32_t data = cpu->dmem_data_w;
					this->memory[addr+0] = (data >> 24) & 0xFF;
					this->memory[addr+1] = (data >> 16) & 0xFF;
					this->memory[addr+2] = (data >> 8) & 0xFF;
					this->memory[addr+3] = (data >> 0) & 0xFF;
				}
				else
				{
					size_t addr = cpu->dmem_address - this->memory_start;
					uint32_t sum = this->memory[addr+0];
					sum <<= 8;
					sum += this->memory[addr+1];
					sum <<= 8;
					sum += this->memory[addr+2];
					sum <<= 8;
					sum += this->memory[addr+3];
					cpu->dmem_data_r = sum;
				}
				//cpu->dmem_stall = 0;
				cpu->dmem_ack=1;
				printf("Data addr:%x, Is write: %d, Sel: %d\n",
						cpu->dmem_address, cpu->dmem_we, cpu->dmem_sel);
			}
			else
			{
				//cpu->dmem_stall = 0;
				cpu->dmem_ack=0;
			}
		}
#endif

		// CLK->H
		cpu->clk_i = 1;
		cpu->eval();

#if 0
		printf("Debug: dmem_sel[%x], dmem_cti[%x], dmem_we[%x], dmem_stb[%x], dmem_cyc[%x]\n"
				"dmem_address[%x], dmem_data_w[%x], imem_addr[%x]\n"
				"imem_sel[%x], imem_stb[%x], imem_cyc[%x], imem_cti[%x]\n",
				cpu->dmem_sel,
				cpu->dmem_cti,
				cpu->dmem_we,
				cpu->dmem_stb,
				cpu->dmem_cyc,
				cpu->dmem_address,
				cpu->dmem_data_w,
				cpu->imem_addr,
				cpu->imem_sel,
				cpu->imem_stb,
				cpu->imem_cyc,
				cpu->imem_cti);
#endif


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
