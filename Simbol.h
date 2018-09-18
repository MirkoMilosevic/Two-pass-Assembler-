//using namespace std;
#include <string>
#include <fstream>
//#include <iostream>

class Simbol
{
	//za sekcije SEG, za simbole SYM
	std::string tipSimbola;
	int redniBroj;
	std::string naziv;
	int sekcija;
	//adresa pocetka sekcije za SEG, vrednost za SYM
	int adresa;
	//velicina sekcije, ne koristi se za SYM
	int velicina;
	//flegovi za SEG, G/L za SYM
	std::string flegovi;
	
	int definisan;
	
	
	
public:
	Simbol* next;
	
	Simbol(std::string, int, std::string, int, int, int, std::string, int);
	
	~Simbol();
	
	
	void ispisiSekciju(std::ofstream&);
	
	void ispisiSimbol(std::ofstream&);
	
	
	void postaviTipSimbola(std::string); 
		
	void postaviRedniBroj(int);
		
	void postaviNaziv(std::string);
		
	void postaviSekciju(int); 
		
	void postaviAdresu(int); 
	
	void postaviVelicinu(int); 
	
	void postaviFlegove(std::string);
	
	void postaviDefinisan(); 

	
	std::string dohvatiTipSimbola();
	
	int dohvatiRedniBroj();

	std::string dohvatiNaziv();

	int dohvatiSekciju();

	int dohvatiAdresu();

	int dohvatiVelicinu();

	std::string dohvatiFlegove();
	
	int dohvatiDefinisan();
};
