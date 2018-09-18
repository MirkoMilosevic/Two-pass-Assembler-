#include "Simbol.h"
#include <string.h>
#include <iostream>

using namespace std;

Simbol::Simbol(string ts, int rb, string n, int s, int a, int v, string f, int def)
{
	tipSimbola = ts;
	redniBroj = rb;
	naziv = n;
	sekcija = s;
	adresa = a;
	velicina = v;
	flegovi = f;
	next = NULL;
	definisan = def;
}

Simbol::~Simbol()
{
	delete next;
}
	
void Simbol::ispisiSimbol(ofstream& izlaz)
{
	izlaz << tipSimbola << " " << redniBroj << " " << naziv << " " << sekcija << " " << hex << "0x" << adresa << dec << " " << flegovi << endl;
}


void Simbol::ispisiSekciju(ofstream& izlaz)
{
	izlaz << tipSimbola << " " << redniBroj << " " << naziv << " " << sekcija << " " << hex << "0x" << adresa << " " << "0x" << velicina << dec << " " << flegovi << endl;
}


void Simbol::postaviTipSimbola(string ts) 
{ tipSimbola = ts; }
		
void Simbol::postaviRedniBroj(int rb) 
{ redniBroj = rb; }
		
void Simbol::postaviNaziv(string n) 
{ naziv = n; }
		
void Simbol::postaviSekciju(int s) 
{ sekcija = s; }
		
void Simbol::postaviAdresu(int a) 
{ adresa = a; }
	
void Simbol::postaviVelicinu(int v) 
{ velicina = v; }
	
void Simbol::postaviFlegove(string f) 
{ flegovi = f; }

void Simbol::postaviDefinisan() 
{ definisan = 1; }


string Simbol::dohvatiTipSimbola()
{ return tipSimbola; }
	
int Simbol::dohvatiRedniBroj()
{ return redniBroj; }

string Simbol::dohvatiNaziv()
{ return naziv; }

int Simbol::dohvatiSekciju()
{ return sekcija; }

int Simbol::dohvatiAdresu()
{ return adresa; }

int Simbol::dohvatiVelicinu()
{ return velicina; }

string Simbol::dohvatiFlegove()
{ return flegovi; }

int Simbol::dohvatiDefinisan()
{ return definisan; }
