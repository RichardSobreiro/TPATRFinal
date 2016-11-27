#pragma once

#include <iostream>
#include <string>

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/scoped_ptr.hpp>

#include "util.hpp"

using namespace std;
using namespace boost::interprocess;


class fila_gateway_historiador
{
	string nome = "gateway_historiador";
	boost::scoped_ptr<message_queue> mq;
public:
	fila_gateway_historiador()
	{
		message_queue::remove(nome.c_str());
		mq.reset(new message_queue(create_only, nome.c_str(), 1000000, sizeof(struct position_t)));
	}
	~fila_gateway_historiador()
	{
		message_queue::remove(nome.c_str());
	}

	void write_position_t(struct position_t &position_t)
	{
		try
		{
			(*mq).send(&position_t, sizeof(position_t), 1);
		}
		catch (interprocess_exception &ex)
		{
			message_queue::remove(nome.c_str());
			cout << ex.what() << endl;
			return;
		}
	}

	size_t read_position_t(struct position_t &item)
	{
		size_t msg_size;
		unsigned int priority;

		try
		{
			(*mq).receive(&item, sizeof(item), msg_size, priority);
			return msg_size;
		}
		catch (interprocess_exception &ex)
		{
			message_queue::remove(nome.c_str());
			cout << ex.what() << endl;
			return 0;
		}
		catch (const std::exception &e)
		{
			cerr << "Erro: " << e.what() << endl;
			return 0;
		}
	}
};