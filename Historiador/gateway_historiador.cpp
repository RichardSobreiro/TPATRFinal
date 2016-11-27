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
	fila_gateway_historiador fila;

	// Cria a fila de mensagens para a comunicação entre gateway e o historiador
	try 
	{
		struct position_t *nova_posicao = new struct position_t();

		size_t msg_size = fila.read_position_t(*nova_posicao);

		imprime_position_t(*nova_posicao);
	}
	catch (interprocess_exception &ex) 
	{
		cout << ex.what() << endl;
		return;
	}

}

//void thread_procedimento_gateway_historiador()
//{
//	// Arquivo/Stream de saída de dados de posição recebidos do
//	// processo gateway
//	ofstream arquivo_binario;
//	// Arquivo/Stream de entrada para teste
//	ifstream arquivo_binario1;
//
//	// Cria a fila de mensagens para a comunicação entre gateway e o historiador
//	try {
//		// Apaga fila anterior caso a mesma ainda exista
//		message_queue::remove("gateway_historiador");
//
//		// Cria a fila de mensagens gateway_historiador
//		message_queue fila_gateway_historiador
//			(create_only					// Apenas cria a fila de mensagens
//			, "gateway_historiador"         // Nome da fila de mensagens
//			, 100000                        // Número máximo de mensagens
//			, sizeof(struct position_t)     // Tamanho máximo da mensagem
//			);
//
//		// Cria o diretório de dados caso ele ainda não exista
//		if (CreateDirectory(DIRETORIO, NULL) ||
//			ERROR_ALREADY_EXISTS == GetLastError()) 
//		{
//			
//		}
//		else
//		{
//			DWORD lastError = GetLastError();
//			cout << "Erro na criacao do diretorio. Codigo do erro: " << hex
//				<< lastError << ".";
//			return;
//		}
//
//		while (true)
//		{
//			string filename("");
//			struct position_t nova_posicao;
//			size_t msg_size;
//			unsigned int priority;
//
//			// Thread permanece aguardando a chegada de uma nova informação de posição
//			// proveniente do processo gateway
//			fila_gateway_historiador.receive(	&nova_posicao, 
//												sizeof(struct position_t),
//												msg_size, priority);
//
//			// Monta o nome do arquivo a ser criado/aberto
//			filename.assign(to_string(nova_posicao.id));
//			filename.append(".bin");
//			filename.insert(0, DIRETORIO);
//
//			// Escreve a nova informação no final do arquivo pertencente
//			// ao ID da posição
//			arquivo_binario.open(filename, ios::out | ios::app | ios::binary);
//			escreve_postiont_bin(nova_posicao, arquivo_binario);
//			arquivo_binario.close();
//
//			// -----------------------------------------------------------
//			// Teste da operação realizada acima
//			struct position_t posicao_t_do_arquivo;
//
//			arquivo_binario1.open(filename, ios::in | ios::binary);
//			le_postiont_bin(posicao_t_do_arquivo, arquivo_binario1);
//			arquivo_binario1.close();
//
//			imprime_position_t(posicao_t_do_arquivo);
//			// -----------------------------------------------------------
//		}
//
//	}
//	catch (interprocess_exception &ex) {
//		cout << ex.what() << endl;
//		return;
//	}
//
//}