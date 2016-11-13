#include <iostream> 
#include <fstream>
#include <windows.h>
// Bibliotecas Boost
#include <boost/interprocess/ipc/message_queue.hpp>
// Bibliotecas do projeto
#include "Servidor_Historiador.h"
#include "gateway_historiador.h"

using namespace std;
using namespace boost::interprocess;

void thread_procedimento_servidor_historiador()
{
	// Stream (Arquivo) de leitura de dados sobre um id
	ifstream arquivo_binario; 

	try {
		// Apaga fila anterior caso a mesma ainda exista
		message_queue::remove("servapp_historiador");

		// Cria a fila de mensagens servidor de aplicação para o historiador
		message_queue fila_servidor_historiador
			(create_only								// Cria a fila de mensagens
			, "servapp_historiador"						// Nome da fila de mensagens
			, 100000									// Número máximo de mensagens
			, sizeof(struct historical_data_request_t)	// Tamanho máximo da mensagem
			);

		// Apaga fila anterior caso a mesma ainda exista
		message_queue::remove("historiador_servapp");

		// Cria a fila de mensagens historiador para o servidor de aplicação
		message_queue fila_historiador_servidor
		(create_only									// Cria a fila de mensagens
			, "historiador_servapp"						// Nome da fila de mensagens
			, 100000									// Número máximo de mensagens
			, sizeof(struct historical_data_reply_t)	// Tamanho máximo da mensagem
		);

		while (true)
		{
			string filename("");
			struct historical_data_request_t data_request;
			struct historical_data_reply_t data_reply;
			size_t msg_size;
			unsigned int priority;

			if (fila_servidor_historiador.try_receive(&data_request, sizeof(struct historical_data_request_t), msg_size, priority))
			{
				filename.insert(0, DIRETORIO);
				filename.append(to_string(data_request.id));

				arquivo_binario.open(filename, ios::in | ios::binary);

				if (arquivo_binario.is_open())
				{
					vector<struct position_t> vectoraux;

					while (!arquivo_binario.eof())
					{
						struct position_t posicao_t_do_arquivo;
						le_postiont_bin(posicao_t_do_arquivo, arquivo_binario);
						imprime_position_t(posicao_t_do_arquivo);
						vectoraux.push_back(posicao_t_do_arquivo);
					}

					arquivo_binario.close();

					int i(0);
					while ((i < data_request.num_samples) 
						&& (i < MAX_POSITION_SAMPLES) 
						&& (!vectoraux.empty))
					{
						imprime_position_t(vectoraux.pop_back);
						i++;
					}
					//return;
				}
				else
				{
					DWORD lastError = GetLastError();
					std::cout << "Erro na abertura do arquivo.\nCodigo do erro: " << std::hex
						<< lastError << "." << endl;
				}

				filename.clear();
			}
		}

	}
	catch (interprocess_exception &ex) {
		cout << ex.what() << endl;
		return;
	}

}
