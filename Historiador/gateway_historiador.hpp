#ifndef GATEWAY_HISTORIADOR_INCLUDE
#define GATEWAY_HISTORIADOR_INCLUDE

void le_postiont_bin(struct position_t& position, std::ifstream& in);

void escreve_postiont_bin(struct position_t& position, std::ofstream& out);

void thread_procedimento_gateway_historiador();

#endif // !GATEWAY_HISTORIADOR_INCLUDE
