#include <iostream> 
#include <fstream>
#include <windows.h>
// Bibliotecas Boost
#include <boost/interprocess/ipc/message_queue.hpp>
// Bibliotecas do projeto
#include "gateway_historiador.hpp"
#include "fila_gateway_historiador.hpp"
#include "util.hpp"

using namespace std;
using namespace boost::interprocess;

void thread_procedimento_gateway_historiador()
{
	// Arquivo/Stream de saída de dados de posição recebidos do
	// processo gateway
	ofstream arquivo_binario;
	// Arquivo/Stream de entrada para teste
	ifstream arquivo_binario1;
	Fila_Gateway_Historiador fila_gateway_historiador;

	// Cria a fila de mensagens para a comunicação entre gateway e o historiador
	try 
	{
		while (true)
		{
			struct position_t *nova_posicao = new struct position_t();

			size_t msg_size = fila.read_position_t(*nova_posicao);

			imprime_position_t(*nova_posicao);
		}
	}
	catch (interprocess_exception &ex) 
	{
		cout << ex.what() << endl;
		return;
	}

}
