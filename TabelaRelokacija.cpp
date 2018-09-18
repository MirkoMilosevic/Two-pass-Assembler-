#include <string>
//#include <iostream>
#include "TabelaRelokacija.h"

using namespace std;
		
TabelaRelokacija::TabelaRelokacija()
{
	pocetak = NULL;
	kraj = NULL;
}
	

TabelaRelokacija::~TabelaRelokacija()
{
	pocetak = NULL;
	kraj = NULL;
}

	
	
void TabelaRelokacija::dodajRelokaciju(Relokacija* r)
{
	ElemRelok* novi = new ElemRelok(r);
	
	if(pocetak == NULL)
	{ 
		pocetak = novi;
		kraj = novi; 
	}
	else
	{
		kraj->next = novi;
		kraj = kraj->next;
	}
}
	
void TabelaRelokacija::ispisiTabeluRelokacija(ofstream& izlaz, string sekcija)
{
	izlaz << "#rel" << sekcija << endl;
	ElemRelok *tek = pocetak;
	while(tek != NULL)
	{
		tek->relokacija->ispisiRelokaciju(izlaz);
		tek = tek->next;
	}
	
	izlaz << endl;
}

void TabelaRelokacija::isprazniTebelu()
{
	ElemRelok *tek = pocetak;
	while(tek != NULL)
	{
		pocetak = pocetak->next;
		delete tek;
		tek = pocetak;
	}
	pocetak = 0;
	kraj = 0;
}


