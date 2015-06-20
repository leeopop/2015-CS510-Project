/*
 * module.cpp
 *
 *  Created on: Jun 21, 2015
 *      Author: leeopop
 */


#include "module.hpp"

using namespace E;


VRunner::VRunner(altor32* cpu, System* system) : Module(system)
{
	this->cpu = cpu;
	Message* m = new Message;
	m->type = NEXT_CLOCK;
	this->sendMessage(this, m, 1);
}

VRunner::~VRunner()
{
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

		// CLK->H
		cpu->clk_i = 1;
		cpu->eval();

		if(Verilated::gotFinish() || cpu->fault_o || cpu->break_o)
			break;
		else
		{
			Message* m = new Message;
			m->type = NEXT_CLOCK;
			this->sendMessage(this, m, 2);
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
