#include <string>
#include <fstream>

class Relokacija
{
	int adresa;
	std::string tip;
	int redniBroj;
	
	
	
public:
	Relokacija* next;

	Relokacija(int, std::string, int);
	
	~Relokacija();
	
	
	void ispisiRelokaciju(std::ofstream&);
	
	
	void postaviAdresu(int); 
		
	void postaviTip(std::string);
		
	void postaviRedniBroj(int);

	
	int dohvatiAdresu();
	
	std::string dohvatiTip();

	int dohvatiRedniBroj();

};
