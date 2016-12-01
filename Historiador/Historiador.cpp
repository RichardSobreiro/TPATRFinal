#include <iostream>
#include <boost/thread.hpp>

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

#include "threads.hpp"

using namespace std;

int main()
{
	boost::thread thread_gateway_historiador{ thread_procedimento_gateway_historiador };
	boost::thread thread_servidor_historiador{ thread_procedimento_servidor_historiador};

	thread_gateway_historiador.join();
	thread_servidor_historiador.join();

	return 0;
}