#include <string>
//#include <iostream>
#include "TabelaSimbola.h"

using namespace std;	
	
TabelaSimbola::TabelaSimbola()
{
	pocetak = NULL;
	kraj = NULL;
}
	

TabelaSimbola::~TabelaSimbola()
{
	pocetak = NULL;
	kraj = NULL;
}

	
	
void TabelaSimbola::dodajSimbol(Simbol* s)
{
	Elem* novi = new Elem(s);
	
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
	
void TabelaSimbola::ispisiTabeluSimbola(ofstream& izlaz)
{
	izlaz << "#TabelaSimbola" << endl;
	Elem* tek = pocetak;
	while(tek != NULL)
	{
		if(tek->simbol->dohvatiTipSimbola() == "SYM") { tek->simbol->ispisiSimbol(izlaz); }
		if(tek->simbol->dohvatiTipSimbola() == "SEG") { tek->simbol->ispisiSekciju(izlaz); }
		tek = tek->next;
	}
}

Simbol* TabelaSimbola::pronadjiPoBroju(int n)
{
	Elem* tek = pocetak;
	while (tek != NULL) 
	{ 
		if (tek->simbol->dohvatiRedniBroj() == n) { return tek->simbol; }
		else { tek = tek->next; }
	}
	return 0;
}

Simbol* TabelaSimbola::pronadjiPoNazivu(string s)
{
	Elem* tek = pocetak;
	while (tek != NULL) 
	{ 
		if (tek->simbol->dohvatiNaziv() == s) { return tek->simbol; }
		else { tek = tek->next; }
	}
	return 0;
}



