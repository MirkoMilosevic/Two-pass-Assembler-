//using namespace std;
#include <string>
//#include <iostream>
#include "Simbol.h"
#include <fstream>

class TabelaSimbola
{

public:
	
	struct Elem
	{
		Simbol* simbol;
		Elem* next;
		Elem(Simbol* s) { simbol = s; next = 0; }
	};
	
	Elem* pocetak;
	Elem* kraj;
	
	
	TabelaSimbola();
	
	~TabelaSimbola();
	
	
	void dodajSimbol(Simbol*);
	
	void ispisiTabeluSimbola(std::ofstream&);
	
	Simbol* pronadjiPoBroju(int);
	
	Simbol* pronadjiPoNazivu(std::string);
	
};
