#pragma once

#include <iostream>

#include <boost/regex.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/thread/mutex.hpp>

using namespace std;
using namespace boost::interprocess;

#define DIRETORIO "./DiretorioDeDados/"

struct position_t
{
	int id;
	std::time_t timestamp;
	double latitude;
	double longitude;
	int speed;
};

struct active_users_t
{
	active_users_t() : num_active_users(0)
	{
		for (unsigned i = 0; i < LIST_SIZE; ++i)
		{
			list[i].id = -1;
		}
	}
	int num_active_users;
	enum { LIST_SIZE = 1000000 };
	position_t list[LIST_SIZE];
	boost::interprocess::interprocess_mutex mutex;
};

static const unsigned MAX_POSITION_SAMPLES = 30;

struct historical_data_request_t
{
	int id;
	int num_samples;
};

struct historical_data_reply_t
{
	int num_samples_available;
	position_t data[MAX_POSITION_SAMPLES];
};

bool get_args(string msg, vector<string>& values);

struct position_t& preenche_posiont_t(vector<string> &args);

void imprime_position_t(struct position_t& nova_posicao);

void le_postiont_bin(struct position_t& position, std::ifstream& in);

void escreve_postiont_bin(struct position_t& position, std::ofstream& out);