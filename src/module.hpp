/*
 * module.hpp
 *
 *  Created on: Jun 21, 2015
 *      Author: leeopop
 */

#ifndef SRC_MODULE_HPP_
#define SRC_MODULE_HPP_


#include <E/E_TimerModule.hpp>
#include <E/E_Log.hpp>
#include <E/E_System.hpp>
#include "altor32__Syms.h"

class VRunner : public E::Module, private E::Log
{
private:
	altor32* cpu;

protected:
	virtual Module::Message* messageReceived(Module* from, Module::Message* message) final;
	virtual void messageFinished(Module* to, Module::Message* message, Module::Message* response) final;
	virtual void messageCancelled(Module* to, Module::Message* message) final;

	enum MessageType
	{
		NEXT_CLOCK,
	};
	class Message : public Module::Message
	{
	public:
		enum MessageType type;
	};

public:
	VRunner(altor32* cpu, E::System* system);
	virtual ~VRunner();
};


#endif /* SRC_MODULE_HPP_ */
