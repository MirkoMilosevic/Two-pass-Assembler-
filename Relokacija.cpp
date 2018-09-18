#include <string.h>
#include <iostream>
#include "Relokacija.h"

using namespace std;


Relokacija::Relokacija(int a, string t, int rb)
{
	adresa = a;
	tip = t;
	redniBroj = rb;
	next = NULL;
}

Relokacija::~Relokacija()
{
	delete next;
}
	
	
void Relokacija::ispisiRelokaciju(ofstream& izlaz)
{
	izlaz << "0x" << std::hex << adresa << std::dec << " " << tip << " " << redniBroj << endl;
}
	
	
void Relokacija::postaviAdresu(int a)
{ adresa = a; } 
		
void Relokacija::postaviTip(string t)
{ tip = t; }
		
void Relokacija::postaviRedniBroj(int rb)
{ redniBroj = rb; }

	
int Relokacija::dohvatiAdresu()
{ return adresa; }
	
string Relokacija::dohvatiTip()
{ return tip; }

int Relokacija::dohvatiRedniBroj()
{ return redniBroj; }
