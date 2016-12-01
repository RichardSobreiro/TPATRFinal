#pragma once

#include <iostream>
#include <string>

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/scoped_ptr.hpp>

#include "util.hpp"

using namespace std;
using namespace boost::interprocess;


class Fila_Gateway_Historiador
{
	string nome = "gateway_historiador";
	boost::scoped_ptr<message_queue> mq;
public:
	Fila_Gateway_Historiador()
	{
		message_queue::remove(nome.c_str());
		mq.reset(new message_queue(create_only, nome.c_str(), 1000000, sizeof(struct position_t)));
	}
	~Fila_Gateway_Historiador()
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
		size_t msg_size(0);
		unsigned int priority(1);

		try
		{
			(*mq).try_receive(&item, sizeof(item), msg_size, priority);
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