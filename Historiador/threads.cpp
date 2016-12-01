#include <iostream> 
#include <fstream>
// Bibliotecas Boost
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
// Threads
#include "threads.hpp"
// Classes fila
#include "fila_servidor_historiador.hpp"
#include "fila_historiador_servidor.hpp"
#include "fila_gateway_historiador.hpp"
// 
#include "util.hpp"

using namespace std;
using namespace boost::interprocess;

boost::mutex mutex_arquivos;

// Thread que gerencia a comunicação entre o gateway e o historiador
void thread_procedimento_gateway_historiador()
{
	// Arquivo/Stream de saída de dados de posição recebidos do
	// processo gateway
	ofstream arquivo_binario;

	Fila_Gateway_Historiador fila_gateway_historiador;

	try
	{
		while (true)
		{
			struct position_t *nova_posicao = new struct position_t();
			string filename("");
			size_t msg_size(0);

			msg_size = fila_gateway_historiador.read_position_t(*nova_posicao);
			if(msg_size > 0)
			{
				filename.insert(0, DIRETORIO);
				filename.append("/" + to_string(nova_posicao->id));

				{
					boost::mutex::scoped_lock lock(mutex_arquivos);

					arquivo_binario.open(filename, ios::out | ios::binary);

					if (arquivo_binario.is_open())
					{
						escreve_postiont_bin(*nova_posicao, arquivo_binario);

						arquivo_binario.close();
					}
				}
			}

			nova_posicao = NULL;
		}
	}
	catch (interprocess_exception &ex)
	{
		cout << ex.what() << endl;
		return;
	}

}

// Thread que gerencia a comunicação entre o servidor e o historiador
void thread_procedimento_servidor_historiador()
{
	// Stream (Arquivo) de leitura de dados sobre um id
	ifstream arquivo_binario;
	Fila_Servidor_Historiador fila_servidor_historiador;
	Fila_Historiador_Servidor fila_historiador_servidor;

	try
	{
		while (true)
		{
			string filename("");
			vector<struct position_t> vectoraux;
			struct historical_data_request_t *data_request = new struct historical_data_request_t();
			struct historical_data_reply_t *data_reply = new struct historical_data_reply_t();
			size_t msg_size(0);

			msg_size = fila_servidor_historiador.read_historical_data_request_t(*data_request);

			if (msg_size > 0)
			{
				filename.insert(0, DIRETORIO);
				filename.append("/" + to_string(data_request->id));

				{
					boost::mutex::scoped_lock lock(mutex_arquivos);

					arquivo_binario.open(filename, ios::in | ios::binary);

					if (arquivo_binario.is_open())
					{
						while (!arquivo_binario.eof())
						{
							struct position_t posicao_t_do_arquivo;

							le_postiont_bin(posicao_t_do_arquivo, arquivo_binario);
							// TODO: Remover imprime
							imprime_position_t(posicao_t_do_arquivo);

							vectoraux.push_back(posicao_t_do_arquivo);
						}

						arquivo_binario.close();
					}
				}

				int i(0);
				while ((i < data_request->num_samples) && (i < MAX_POSITION_SAMPLES) && (!vectoraux.empty()))
				{
					// TODO: Remover imprime
					imprime_position_t(vectoraux[i]);

					data_reply->data[i].id = vectoraux[i].id;
					data_reply->data[i].latitude = vectoraux[i].latitude;
					data_reply->data[i].longitude = vectoraux[i].longitude;
					data_reply->data[i].speed = vectoraux[i].speed;
					data_reply->data[i].timestamp = vectoraux[i].timestamp;

					vectoraux.pop_back();
					i++;
					data_reply->num_samples_available--;
				}

				fila_historiador_servidor.write_historical_data_reply_t(*data_reply);
			}

			free(data_reply);
			free(data_request);

			filename.clear();
		}
	}
	catch (interprocess_exception &ex)
	{
		cout << ex.what() << endl;
		return;
	}


}