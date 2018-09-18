//#include <iostream>
#include <fstream>
#include "TabelaSimbola.h"
#include <vector>
#include <string>
#include "TabelaRelokacija.h"

//using namespace std;

class Asembler
{

std::ifstream ulaz;
std::ofstream izlaz;

int lc;
int rb;
int greska;
std::string poruka;
int trenutnaSekcija;
int relokacija;
int globalniSimbol;

TabelaSimbola tabelaSimbola;

TabelaRelokacija tabelaRelokacija;
	
public:

	Asembler();
	
	~Asembler();
	
	int proveriIzraz(std::vector<std::string>);
	
	int izracunajKonstantu(std::vector<std::string>);
	
	void prviProlaz();
	
	void drugiProlaz();
	
	void ispisiSekciju(std::vector<int>);
	
};
