/*
 * module.hpp
 *
 *  Created on: Jun 21, 2015
 *      Author: leeopop
 */

#ifndef SRC_MODULE_HPP_
#define SRC_MODULE_HPP_

#include <E/E_Common.hpp>
#include <E/E_TimerModule.hpp>
#include <E/E_Log.hpp>
#include <E/E_System.hpp>
#include "altor32__Syms.h"

class VRunner : public E::Module
{
private:
	altor32* cpu;
	uint8_t* memory;
	size_t memory_start;

protected:
	virtual Module::Message* messageReceived(Module* from, Module::Message* message) final;
	virtual void messageFinished(Module* to, Module::Message* message, Module::Message* response) final;
	virtual void messageCancelled(Module* to, Module::Message* message) final;

	enum MessageType
	{
		NEXT_CLOCK,
		RESPONSE_IMEM,
		RESPONSE_DMEM,
	};
	class Message : public Module::Message
	{
	public:
		enum MessageType type = NEXT_CLOCK;
		uint32_t data = 0;
		int flag = 0;
	};

	uint32_t next_idata = 0;
	int next_iflag = 0;
	uint32_t next_ddata = 0;
	int next_dflag = 0;

public:
	static const E::Time imem_delay;
	static const E::Time dmem_delay;

	VRunner(size_t memory_start, size_t memory_size, altor32* cpu, E::System* system);
	void writeAt(size_t addr, uint8_t val);
	virtual ~VRunner();
};


#endif /* SRC_MODULE_HPP_ */
