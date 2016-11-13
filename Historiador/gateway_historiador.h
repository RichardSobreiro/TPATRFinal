#ifndef GATEWAY_HISTORIADOR_INCLUDE
#define GATEWAY_HISTORIADOR_INCLUDE

#define DIRETORIO "./DiretorioDeDados/"

struct position_t
{
	int id;
	time_t timestamp;
	double latitude;
	double longitude;
	int speed;
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

void le_postiont_bin(struct position_t& position, std::ifstream& in);

void escreve_postiont_bin(struct position_t& position, std::ofstream& out);

void thread_procedimento_gateway_historiador();

// Teste

void imprime_position_t(struct position_t& nova_posicao);

#endif // !GATEWAY_HISTORIADOR_INCLUDE
