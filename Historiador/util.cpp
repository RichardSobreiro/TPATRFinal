#include <iostream>
#include <fstream>
#include <boost/regex.hpp>

#include "util.hpp"

using namespace std;
using namespace boost;

bool get_args(string msg, vector<string>& values)
{
	const char* pattern("-?[0-9]*\\.?[0-9]+");

	boost::regex re(pattern);

	boost::sregex_iterator it(msg.begin(), msg.end(), re);
	boost::sregex_iterator end;

	for (int i = 0; it != end; ++it, i++)
	{
		values.push_back(it->str());
	}

	if (values.empty()) return false;
	else return true;
}

void imprime_position_t(struct position_t& nova_posicao)
{
	cout << "---------------------------------" << endl;
	cout << "ID: " << nova_posicao.id << endl;
	cout << "Latitude: " << nova_posicao.latitude << endl;
	cout << "Longitude: " << nova_posicao.longitude << endl;
	cout << "Velocidade: " << nova_posicao.speed << endl;
	struct tm *tm = new struct tm;
	gmtime_s(tm, &(nova_posicao.timestamp));
	char date[30];
	strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm);
	cout << "Timestamp: " << date << endl;
	cout << "---------------------------------" << endl;
}

struct position_t& preenche_posiont_t(vector<string> &args)
{
	std::string::size_type sz;

	struct position_t *nova_posicao = new struct position_t();

	nova_posicao->id = atoi(args[0].c_str());
	nova_posicao->timestamp = (time_t)atoll(args[1].c_str());
	nova_posicao->longitude = stod(args[2], &sz);
	nova_posicao->latitude = stod(args[3], &sz);
	nova_posicao->speed = atoi(args[4].c_str());

	return (*nova_posicao);
}

void le_postiont_bin(struct position_t& position, std::ifstream& in)
{
	in.read((char*)&position.id, sizeof(int));
	in.read((char*)&position.timestamp, sizeof(time_t));
	in.read((char*)&position.latitude, sizeof(double));
	in.read((char*)&position.longitude, sizeof(double));
	in.read((char*)&position.speed, sizeof(int));
}

void escreve_postiont_bin(struct position_t& position, std::ofstream& out)
{
	out.write((char*)&position.id, sizeof(int));
	out.write((char*)&position.timestamp, sizeof(time_t));
	out.write((char*)&position.latitude, sizeof(double));
	out.write((char*)&position.longitude, sizeof(double));
	out.write((char*)&position.speed, sizeof(int));
}