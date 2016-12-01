#include <iostream> 
#include <fstream>
#include <windows.h>
// Bibliotecas Boost
#include <boost/interprocess/ipc/message_queue.hpp>
// Bibliotecas do projeto
#include "fila_servidor_historiador.hpp"
#include "fila_historiador_servidor.hpp"
#include "Servidor_Historiador.hpp"
#include "gateway_historiador.h"
#include "util.hpp"

using namespace std;
using namespace boost::interprocess;

void thread_procedimento_servidor_historiador()
{
	// Stream (Arquivo) de leitura de dados sobre um id
	ifstream arquivo_binario; 
	Fila_Servidor_Historiador fila_servidor_historiador;
	Fila_Historiador_Servidor fila_historiador_servidor;

	while (true)
	{
		string filename("");
		vector<struct position_t> vectoraux;
		struct historical_data_request_t data_request;
		struct historical_data_reply_t data_reply;
		size_t msg_size(0);

		msg_size = fila_servidor_historiador.read_historical_data_request_t(data_request);

		if (msg_size > 0)
		{
			filename.insert(0, DIRETORIO);

			filename.append("/" + to_string(data_request.id));

			arquivo_binario.open(filename, ios::in | ios::binary);

			if (arquivo_binario.is_open())
			{
				while (!arquivo_binario.eof())
				{
					struct position_t posicao_t_do_arquivo;
					le_postiont_bin(posicao_t_do_arquivo, arquivo_binario);
					imprime_position_t(posicao_t_do_arquivo);
					vectoraux.push_back(posicao_t_do_arquivo);
				}
			}
			else
			{
				DWORD lastError = GetLastError();
				std::cout << "Erro na abertura do arquivo.\nCodigo do erro: " << std::hex
					<< lastError << "." << endl;
			}

			arquivo_binario.close();

			int i(0);
			while ((i < data_request.num_samples) && (i < MAX_POSITION_SAMPLES) && (!vectoraux.empty()))
			{
				struct position_t posicao_t_do_arquivo = vectoraux.pop_back;
				imprime_position_t(posicao_t_do_arquivo);
				data_reply.data[i] = posicao_t_do_arquivo;
				i++;
				data_reply.num_samples_available = i;
			}
		}

		filename.clear();
	}

}
