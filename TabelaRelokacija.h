//#include <string.h>
#include <fstream>
#include "Relokacija.h"

class TabelaRelokacija
{

public:

	struct ElemRelok
	{
		Relokacija* relokacija;
		ElemRelok* next;
		ElemRelok(Relokacija* r) { relokacija = r; next = 0; }
	};
	
	ElemRelok* pocetak;
	ElemRelok* kraj;
	
	TabelaRelokacija();
	
	~TabelaRelokacija();
	
	
	void dodajRelokaciju(Relokacija*);
	
	void ispisiTabeluRelokacija(std::ofstream&, std::string);
	
	void isprazniTebelu();
	
};
