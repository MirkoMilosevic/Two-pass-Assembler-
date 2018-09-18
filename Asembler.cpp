#include <iostream>
//#include <fstream>
#include <sstream>
#include <string>
//#include <stdio.h>
#include "Asembler.h"
#include "Mapiranje.h"
#include <iomanip>

using namespace std;

Asembler::Asembler()
{
	lc = 0;
	rb = 0;
	greska = 0;
	poruka = "";
	trenutnaSekcija = 0;
	
	relokacija = -1;
	globalniSimbol = -1;
	
	ulaz.open("ulaz.txt");
	izlaz.open("izlaz.txt");	
}

Asembler::~Asembler()
{
	ulaz.close();
	izlaz.close();
}


//metoda koja ispisuje sadrzaj sekcija
void Asembler::ispisiSekciju(vector<int> sekcija)
{
	Simbol *temp = tabelaSimbola.pronadjiPoBroju(trenutnaSekcija);
    string y = temp->dohvatiNaziv();
    izlaz << "<" << y << ">" << endl;

	for(int t=0; t<sekcija.size(); t++)
    { 
    	int prviH = (sekcija[t] & 0xf0) >> 4;
    	int drugiH = sekcija[t] & 0x0f;
    	
    	izlaz << std::hex << prviH << drugiH << " " << std::dec;
    	if((t % 16) == 15) { izlaz << endl; }
    }
    izlaz << endl;
}


//funkcija koja proverava da li je izraz izracunljiv
int Asembler::proveriIzraz(vector<string> izraz)
{
	int index[100];
	for(int j =0; j<100; j++) { index[j] = 0; }
	int stek[32];
	int sp = 0;
	
	int i = 0;
	while(i<izraz.size())
	{
		string s = izraz[i];
		string znak;
		
		//naisao je simbol
		if(isalpha(s[0]) || (s[0] == '.'))
		{
			Simbol *p = tabelaSimbola.pronadjiPoNazivu(s);
			
    		if(p != 0)
    		{
    			if(p->dohvatiSekciju() != -1)
    			{
    				if(i<izraz.size()-1) 
    				{ 
    					if((izraz[i+1] == "*") || (izraz[i+1] == "/")) 
    					{ 
    						greska = 1;
    						poruka = "Zabranjeno je mnozenje neapsolutnih simbola!"; 
    						return -1; 
    					} 
    				}
    				if(i>0) 
    				{ 
    					if((izraz[i-1] == "*") || (izraz[i-1] == "/")) 
    					{ 
    						greska = 1;
    						poruka = "Zabranjeno je mnozenje neapsolutnih simbola!"; 
    						return -1; 
    					} 
    				}
    			}
    			
    			int sekcija = p->dohvatiSekciju();
    			
    			//globalni simbol sme da se pojavi samo jednom, bez obzira na operaciju
    			if(sekcija == 0) { index[0]++; globalniSimbol = p->dohvatiRedniBroj(); }
    			else if(sekcija > 0)
    			{
    				if(sp == 0)
    				{
    					if(i == 0) { index[sekcija]++; }
    					else
    					{
    						znak = izraz[i-1];
    						if((znak == "+") || (znak == "-"))
    						{
    							if(znak == "+") { index[sekcija]++; }
    							if(znak == "-") { index[sekcija]--; }
    						}
    						else { greska == 1; poruka = "Zabranjeno je mnozenje neapsolutnih simbola!"; return -1; }
    					}
    				}
    				else
    				{
    					int z = stek[sp];
    					znak = izraz[i-1];
    					
    					if((znak == "*") || (znak == "/")) { greska == 1; poruka = "Zabranjeno je mnozenje neapsolutnih simbola!"; return -1; }
    					
    					if(znak == "-") { z = (-1)*z; }
    					index[sekcija] = index[sekcija] + z;
    				}
    			}
    		}
    		else { greska == 1; poruka = "Simbol " + s + " ne postoji!"; return -1; }
		}
		
		//naisao je placeholder, potrebno je azurirati pojavljivanje simbola iz tekuce sekcije
		if(s == "$")
		{
			Simbol *p = tabelaSimbola.pronadjiPoBroju(trenutnaSekcija);
    		if(p != 0)
    		{
    			if(i<izraz.size()-1) { if((izraz[i+1] == "*") || (izraz[i+1] == "/")) { greska = 1; return -1; } }
    			if(i>0) { if((izraz[i-1] == "*") || (izraz[i-1] == "/")) { greska = 1; return -1; } }
    			
    			if(sp == 0)
    			{
    				if(i == 0) { index[trenutnaSekcija]++; }
    				else
    				{
    					znak = izraz[i-1];
    					if((znak == "+") || (znak == "-"))
    					{
    						if(znak == "+") { index[trenutnaSekcija]++; }
    						if(znak == "-") { index[trenutnaSekcija]--; }
    					}
    					else { greska == 1; poruka = "Zabranjeno je mnozenje neapsolutnih simbola(placeholder)!"; return -1; }
    				}
    			}
    			else
    			{
    				int z = stek[sp];
    				znak = izraz[i-1];
    					
    				if((znak == "*") || (znak == "/")) { greska == 1; return -1; }
    					
    				if(znak == "-") { z = (-1)*z; }
    				index[trenutnaSekcija] = index[trenutnaSekcija] + z;
    			}	
    		}
    		else { greska == 1; return -1; }
		}
		
		//naisla je leva zagrada
		if( s[0] == '(' )
		{
			if(i == 0) { sp++; stek[sp] = 1; }
			else if(izraz[i-1] == "(" ) 
			{ 
				int z = stek[sp]; 
				sp++; stek[sp] = z;  
			}
			else if(izraz[i-1] == "+" ) 
			{ 
				int z; 
				if(sp != 0) 
				{ 
					z = stek[sp]; 
					sp++; 
					stek[sp] = z; 
				} 
				else 
				{ 
					z=1; 
					sp++; 
					stek[sp] = z; 
				} 
			}
			else if(izraz[i-1] == "-" ) 
			{ 
				int z; 
				if(sp != 0) 
				{ 
					z = stek[sp]*(-1); 
					sp++; 
					stek[sp] = z; 
				} 
				else 
				{ 
					z=-1; 
					sp++; 
					stek[sp] = z; 
				} 
			}
			else if((izraz[i-1] == "*") || (izraz[i-1] == "/"))
			{
				int brojZagrada = 1;
				vector<string> rekurzija;
				rekurzija.push_back(izraz[i]);
				while(brojZagrada != 0)
				{
					i++;
					rekurzija.push_back(izraz[i]);
					if(izraz[i] == "(") { brojZagrada++; }
					if(izraz[i] == ")") { brojZagrada--; }
				}
				
				//rekurzivni poziv da se proveri da li je izraz u zagradi konstantan
				int provera = proveriIzraz(rekurzija);
				if(provera != 0) { greska = 1; poruka = "Izraz u zagradi nije konstantan."; return -1; }
			}
		}
		
		//naisla desna zagrada
		if( s[0] == ')' ) 
		{  
			sp--; 
		}
		
		i++;
	} 
	
	int nasao = 0;
	int relok = 0;
	
	for(int k=0; k<100; k++)
	{
		if(index[k] != 0)
		{ 
			if(nasao == 1) { greska = 1; poruka = "Izraz zavisi od vise simbola!"; return -1; }
			if((index[k] == 1) || (index[k] == -1)) { nasao = 1; relok = k; }
			else { greska = 1; poruka = "Izraz se ne moze razresiti jednom relokacijom!"; return -1; }
		}
	}
	//izraz je konstantan
	if(nasao == 0) { return 0; }
	
	//izraz se moze razresiti relokacijom
	if(nasao == 1) { relokacija = relok; return 1; }
}


//funkcija koja racuna vrednost izraza
int Asembler::izracunajKonstantu(vector<string> input)
{
	string stek[64];
	int sp = 0;
	string x;
	
	//slucaj kada izraz ima samo jedan clan
	if(input.size() == 1)
	{
		int tmp;
		string temp = input[0];
		
		if((temp[0] == '0') && (temp[1] == 'x')) { tmp = vratiVrednostHex(temp); }
		else if((temp[0] == '0') && (temp[1] == 'b')) { tmp = vratiVrednostBin(temp); }
		else if(isdigit(temp[0]) || (temp[0] == '-')) { tmp = vratiVrednostDec(temp); }
		else if((int) temp[0] == 39) { tmp = vratiVrednostChar(temp); }
		else if(isalpha(temp[0]) || (temp[0] == '.'))
		{
			Simbol *p = tabelaSimbola.pronadjiPoNazivu(temp);
    		if(p != 0)
    		{
    			tmp = p->dohvatiAdresu();
    		}
    		else { greska == 1; poruka = "Simbol " + temp + " ne postoji!"; return -1; }
		}
		else if(temp == "$") { tmp = lc; }
		
		return tmp;
	}
	
	for(int i=0; i<input.size(); i++)
	{
		x = input[i];
		Simbol* p1 = 0;
		Simbol* p2 = 0;;
		
		if(isOperand(x)) 
		{ 
			sp++;
			stek[sp] = x;
		}
		else
		{
			string opr2 = stek[sp];
			sp--;
			string opr1 = stek[sp];
			sp--;
			
			int o1, o2;
			
			if((opr1[0] == '0') && (opr1[1] == 'x')) { o1 = vratiVrednostHex(opr1); }
			else if((opr1[0] == '0') && (opr1[1] == 'b')) { o1 = vratiVrednostBin(opr1); }
			else if(isdigit(opr1[0]) || (opr1[0] == '-')) { o1 = vratiVrednostDec(opr1); }
			else if((int) opr1[0] == 39) { o1 = vratiVrednostChar(opr1); }
			else if(isalpha(opr1[0]) || (opr1[0] == '.'))
			{	
				p1 = tabelaSimbola.pronadjiPoNazivu(opr1);
    			if(p1 != 0)
    			{
    				o1 = p1->dohvatiAdresu();
    			}
    			else { greska == 1; return -1; } //Greska! Ne postoji simbol!
			}
			else if(opr1 == "$") { o1 = lc; }
			
			
			if((opr2[0] == '0') && (opr2[1] == 'x')) { o2 = vratiVrednostHex(opr2); }
			else if((opr2[0] == '0') && (opr2[1] == 'b')) { o2 = vratiVrednostBin(opr2); }
			else if(isdigit(opr2[0]) || (opr2[0] == '-')) { o2 = vratiVrednostDec(opr2); }
			else if((int) opr2[0] == 39) { o2 = vratiVrednostChar(opr2); }
			else if(isalpha(opr2[0]) || (opr2[0] == '.'))
			{	
				p2 = tabelaSimbola.pronadjiPoNazivu(opr2);
    			if(p2 != 0)
    			{
    				o2 = p2->dohvatiAdresu();
    			}
    			else { greska == 1; return -1; } //Greska! Ne postoji simbol!
			}
			else if(opr2 == "$") { o2 = lc; }
			
			int rezultat;
			
			if(p1 != 0) { p1 = 0; }
			if(p2 != 0) { p2 = 0; }
			
			rezultat = izracunaj(x, o1, o2);
			
			stringstream ss;
			ss << rezultat;
			string strRez = ss.str();
			sp++;
			stek[sp] = strRez;
		}
	}
	
	int r = vratiVrednostDec(stek[sp]);
	sp--;
	
	if(sp == 0) { return r; }
	else { return -1; }
}

//------------------------------------------------------------------------------------
void Asembler::prviProlaz()
{	
	int imaORG = 0;
	int vrednostORG = 0;

	Simbol *s = new Simbol("SEG", rb++, "UND", 0, 0, 0, " ", 0);
    tabelaSimbola.dodajSimbol(s);  
    
    int krajPrograma = 0;   

	string linija;
	while(getline(ulaz, linija))
	{
		if((greska == 1) || (krajPrograma == 1)) { break; }
	
		int brojTokena;
    	vector<string> tokeni = obradiLiniju(linija); 
    	
    	int imaLabelu = 0;
    	int imaInstrukciju = 0;
    	int imaDirektivu = 0;
    	
    	imaORG--;
    	
    	for (int i=0; i<tokeni.size(); i++) 
    	{ 
    		string rec = tokeni[i];
    		brojTokena = vratiToken(rec);

//preskakanje komentara:

			if(rec[0] == ';') { break; }

//obrada labele:
   		
    		if(rec[rec.length()-1] == ':')
    		{
    			//Greska! Dve labele u liniji, ili labela posle instrukcije!
    			if((imaLabelu == 1) || (imaInstrukciju == 1) || (imaDirektivu == 1))  
    			{
    				greska = 1;
    				poruka = "Postoji greska u liniji sa labelama!";
    				break;
    			}
    		
    			string pom;
    			int j = 0;
    			while(j<rec.length()-1)
    			{
    				pom+=rec[j];
    				j++;
    			}
    		
    			//dodavanje labele u tabelu simbola
    			Simbol *p = tabelaSimbola.pronadjiPoNazivu(pom);
    			if(p == 0)
    			{
    				Simbol *s = new Simbol("SYM", rb++, pom, trenutnaSekcija, lc, 0, "L", 1);
    				tabelaSimbola.dodajSimbol(s);
    				imaLabelu = 1;
    				continue;
    			}
    			else
    			{ 
    			 	if((p->dohvatiFlegove() == "L") || (p->dohvatiDefinisan() == 1)) 
    			 	{ greska = 1; poruka = "Dva puta definisana ista labela ili aps. simbol!"; break; }
    				else 
    				{
    					p->postaviSekciju(trenutnaSekcija);
    					p->postaviAdresu(lc);
    					p->postaviDefinisan();
    					imaLabelu = 1;
    					continue;  
    				}
    			}
    		}
    		
//obrada instrukcije:
    		
    		if((brojTokena > 1) && (brojTokena < 35))
			{
				//Greska! Dve instrukcije ili instrukcija i direktiva u liniji!
				if((imaInstrukciju == 1) || (imaDirektivu == 1))
				{
					greska = 1;
					poruka = "Postoje dve instrukcije ili instrukcija i direktiva u liniji!";
					break;
				}
				
				int adr;
				
				//INT
				if(brojTokena == 2)
				{
					if(i<tokeni.size()) { i++; }
					else { greska = 1; break; }
					
					rec = tokeni[i];
					adr = vratiAdresiranje(rec);
					
					if(adr == 2) { lc+=4; }
					else { greska = 1; poruka = "Pogresno adresiranje za INT!"; break; }
					
					imaInstrukciju = 1;
					continue;	
				}
				
				
				//JMP i CALL
				if((brojTokena == 3) || (brojTokena == 4))
				{
					if(i<tokeni.size()) { i++; }
					else { greska = 1; break; }
					
					rec = tokeni[i];
					adr = vratiAdresiranje(rec);
					
					if((adr == 3) || (adr == 5)) { lc+=8; }
					else if(adr == 4) { lc+=4; }
					else { greska = 1; poruka = "Pogresno adresiranje za JMP ili CALL!"; break; }
					
					imaInstrukciju = 1;
					continue;
				}
				
				
				//RET
				if(brojTokena == 5)
				{
					lc+=4; 
					imaInstrukciju = 1;
					continue;
				}
				
				
				//JZ, JNZ, JGZ, JGEZ, JLZ, JLEZ
				if((brojTokena > 5) && (brojTokena < 12))
				{
					if(i<tokeni.size()) { i++; }
					else { greska = 1; break; }
					
					rec = tokeni[i];
					adr = vratiAdresiranje(rec);
					
					if(adr == 2)
					{
						if(i<tokeni.size()) { i++; }
						else { greska = 1; break; }
					
						rec = tokeni[i];
						adr = vratiAdresiranje(rec);
						
						if((adr == 3) || (adr == 5)) { lc+=8; }
						else if(adr == 4) { lc+=4; }
						else { greska = 1; poruka = "Pogresno adresiranje za skakacke instrukcije!"; break; }
						 
						imaInstrukciju = 1;
						continue;
					}
					else { greska = 1; poruka = "Pogresno adresiranje za skakacke instrukcije!"; break; }	
				}
				
				
				//LOAD
				if((brojTokena > 11) && (brojTokena < 17))
				{
					if(i<tokeni.size()) { i++; }
					else { greska = 1; break; }
					
					rec = tokeni[i];
					adr = vratiAdresiranje(rec);
					
					if(adr == 2)
					{
						if(i<tokeni.size()) { i++; }
						else { greska = 1; break; }
					
						rec = tokeni[i];
						adr = vratiAdresiranje(rec);
						
						if((adr == 1) || (adr == 3) || (adr == 5)) { lc+=8; }
						else if((adr == 2) || (adr == 4)) { lc+=4; }
						else { greska = 1; poruka = "Pogresno adresiranje za LOAD!"; break; }
						 
						imaInstrukciju = 1;
						continue;
					}
					else { greska = 1; poruka = "Pogresno adresiranje za LOAD!"; break; }	
				}
				
				
				//STORE
				if((brojTokena > 16) && (brojTokena < 20))
				{
					if(i<tokeni.size()) { i++; }
					else { greska = 1; break; }
					
					rec = tokeni[i];
					adr = vratiAdresiranje(rec);
					
					if(adr == 2)
					{
						if(i<tokeni.size()) { i++; }
						else { greska = 1; break; }
					
						rec = tokeni[i];
						adr = vratiAdresiranje(rec);
						
						if((adr == 3) || (adr == 5)) { lc+=8; }
						else if((adr == 2) || (adr == 4)) { lc+=4; }
						else { greska = 1; poruka = "Pogresno adresiranje za STORE!"; break; }
						
						imaInstrukciju = 1;
						continue;
					}
					else { greska = 1; poruka = "Pogresno adresiranje za STORE!"; break; }
				}
				
				
				//PUSH i POP
				if((brojTokena == 22) || (brojTokena == 23))
				{
					if(i<tokeni.size()) { i++; }
					else { greska = 1; break; }
					
					rec = tokeni[i];
					adr = vratiAdresiranje(rec);
					
					if(adr == 2) { lc+=4; }
					else { greska = 1; poruka = "Pogresno adresiranje za PUSH ili POP!"; break; }	
					
					imaInstrukciju = 1;
					continue;
				}
				
				
				//ADD, SUB, MUL, DIV, MOD, AND, OR, XOR, ASL, ASR
				if((brojTokena > 23) && (brojTokena < 34))
				{
					if(i<tokeni.size()) { i++; }
					else { greska = 1; break; }
					
					rec = tokeni[i];
					adr = vratiAdresiranje(rec);
					
					if(adr == 2)
					{
						if(i<tokeni.size()) { i++; }
						else { greska = 1; break; }
					
						rec = tokeni[i];
						adr = vratiAdresiranje(rec);
						
						if(adr == 2)
						{
							if(i<tokeni.size()) { i++; }
							else { greska = 1; break; }
					
							rec = tokeni[i];
							adr = vratiAdresiranje(rec);
							
							if(adr == 2) { lc+=4; }
							else { greska = 1; poruka = "Pogresno adresiranje za aritmeticke instrukcije!"; break; }
							 
							imaInstrukciju = 1;
							continue;
						}
						else { greska = 1; poruka = "Pogresno adresiranje za aritmeticke instrukcije!"; break; }
					}
					else { greska = 1; poruka = "Pogresno adresiranje za aritmeticke instrukcije!"; break; }
				}
				
				
				//NOT
				if(brojTokena == 34)
				{
					if(i<tokeni.size()) { i++; }
					else { greska = 1; break; }
					
					rec = tokeni[i];
					adr = vratiAdresiranje(rec);
					
					if(adr == 2)
					{
						if(i<tokeni.size()) { i++; }
						else { greska = 1; break; }
					
						rec = tokeni[i];
						adr = vratiAdresiranje(rec);
						
						if(adr == 2) { lc+=4; }
						else { greska = 1; poruka = "Pogresno adresiranje za NOT!"; break; }
						 
						imaInstrukciju = 1;
						continue;
					}
					else { greska = 1; poruka = "Pogresno adresiranje za NOT!"; break; }	
				}
				
			} //if za instrukcije
    		
//obrada sekcije:
    	
    	if((brojTokena > 132) && (brojTokena < 137))
    	{
    		//Greska! Dve direktive ili direktiva i instrukcija u liniji!
			if((imaInstrukciju == 1) || (imaDirektivu == 1))
			{
				greska = 1;
				poruka = "Postoje dve direktive ili direktiva i sekcija u liniji!";
				break;
			}
			
			//moguci flegovi za sekcije
			string f;
			if(brojTokena == 133) { f = "AXP"; }
			if(brojTokena == 134) { f = "ARWP"; }
			if(brojTokena == 135) { f = "ARP"; }
			if(brojTokena == 136) { f = "ARW"; } 
			
			//dodavanje sekcije u tabelu simbola, ako vec ne postoji
			Simbol *p = tabelaSimbola.pronadjiPoNazivu(rec);
    		if(p == 0)
    		{
    			if(trenutnaSekcija != 0)
    			{
    				Simbol *temp = tabelaSimbola.pronadjiPoBroju(trenutnaSekcija);
    				int y = temp->dohvatiAdresu();
    				temp->postaviVelicinu(lc-y);
    			}
    		
    			if(imaORG == 1) { lc = vrednostORG; imaORG = 0; }
    			else { lc = 0; }
				Simbol *s = new Simbol("SEG", rb, rec, rb, lc, 0, f, 1);
    			tabelaSimbola.dodajSimbol(s);
    			trenutnaSekcija = rb;
    			rb++;
    			imaDirektivu = 1;
    			continue;
    		}
    		else
    		{ 
    			greska = 1;
    			poruka = "Sekcija je dva puta definisana!";
    			break;
    		}
    						
    	} //if za sekcije
    	
//obrada .end direktive:

		if(brojTokena == 157)
		{
			//Greska! Dve direktive ili direktiva i instrukcija u liniji!
			if((imaInstrukciju == 1) || (imaDirektivu == 1))
			{
				greska = 1;
				poruka = "Postoje dve direktive ili direktiva i instrukcija u liniji!";
				break;
			}
			
			//postavljanje velicine poslednje sekcije
			if(trenutnaSekcija != 0)
    		{
    			Simbol *temp = tabelaSimbola.pronadjiPoBroju(trenutnaSekcija);
    			int y = temp->dohvatiAdresu();
    			temp->postaviVelicinu(lc-y);
    		}
    		
    		krajPrograma = 1;
    		break;
    			
		} // if za .end
    	
//obrada .global direktive:
		
		if(brojTokena == 156)
		{
			if((imaInstrukciju == 1) || (imaDirektivu == 1))
			{
				greska = 1;
				poruka = "Postoje dve direktive ili direktiva i instrukcija u liniji!";
				break;
			}
			
			if(i<tokeni.size()) { i++; }
			else { greska = 1; break; }
			string m = tokeni[i];
			int v = vratiToken(m);
			if(v != 0)
			{
				greska = 1;
				poruka = "Samo simbol moze biti globalan!";
				break;		
			}
			
			while(i<tokeni.size())
			{
				if(m[0] == ';') { break; }
				
				if(v != 0)
				{
					greska = 1;
					break;
				}
				
				//dodavanje globalnog simbola u tabelu ili izvozenje 
				Simbol *p = tabelaSimbola.pronadjiPoNazivu(m);
    			if(p == 0)
    			{
    				Simbol *s = new Simbol("SYM", rb++, m, 0, 0, 0, "G", 0);
    				tabelaSimbola.dodajSimbol(s);
    			}
    			else
    			{ 
    				if(p->dohvatiFlegove() == "L") { p->postaviFlegove("G"); }
    				else { greska = 1; break; }
    			}
    			
    			if(i<tokeni.size()-1) { i++; m = tokeni[i]; v = vratiToken(m); }
				else { break; }	 
    		}
    		
    		imaDirektivu = 1;
		
		} //if za .global
		
//obrada DEF direktive:
		
		if(brojTokena == 131)
		{
			string ime = tokeni[i-1];
			string izraz = "";
			
			if(vratiToken(ime) != 0) { greska = 1; poruka = "Samo simbol moze biti definisan kao apsolutni!"; break; }
			
			if(i<tokeni.size()) { i++; }
			else { greska = 1; break; }
			string pom = tokeni[i];
			
			//dohvatanje linije od def nadalje, do komentara ili kraja
			while((pom[0] != ';') && (i<tokeni.size())) 
			{ 
				izraz += pom;
				if(i<tokeni.size()-1) { i++; pom = tokeni[i]; }
				else { i++; } 
			}
			
			//provera da li je izraz konstantan
			vector<string> t1 = obradiIzraz(izraz);
			int provera = proveriIzraz(t1);
			if(provera != 0) { greska = 1; poruka = "Izraz u DEF nije konstantan!"; break; }
			vector<string> t2 = infix2postfix(t1);
			int vrednost = izracunajKonstantu(t2);
			
			//dodavanje apsolutnog simbola u tabelu ili definisanje ranije deklarisanog
			Simbol *p = tabelaSimbola.pronadjiPoNazivu(ime);
    		if(p == 0)
    		{
    			Simbol *s = new Simbol("SYM", rb++, ime, -1, vrednost, 0, "L", 1);
    			tabelaSimbola.dodajSimbol(s);
    			break;
    		}
    		else 
    		{
    			if(p->dohvatiDefinisan() == 1) { greska = 1; poruka = "Dva puta definisan isti simbol!"; break; }
    			else 
    			{
    				p->postaviSekciju(-1);
    				p->postaviAdresu(vrednost);
    				p->postaviDefinisan();
    				break;
    			}
    		}
			
		} //if za def
		
//obrada ORG direktive:

		if(brojTokena == 132)
		{
			//da bi imalo efekta samo u sledecoj liniji
			imaORG = 2;
			
			if(i<tokeni.size()) { i++; }
			else { greska = 1; break; }
			string pom = tokeni[i];
			string izraz = "";
			
			//dohvatanje linije od org do kraja ili komentara
			while((pom[0] != ';') && (i<tokeni.size())) 
			{ 
				izraz += pom;
				if(i<tokeni.size()-1) { i++; pom = tokeni[i]; }
				else { i++; } 
			}
			
			//provera da li je izraz konstanta
			vector<string> t1 = obradiIzraz(izraz);
			int provera = proveriIzraz(t1);
			if(provera != 0) { greska = 1; poruka = "Izraz u ORG nije konstantan!"; break; }
			vector<string> t2 = infix2postfix(t1);
			vrednostORG = izracunajKonstantu(t2);
			break;
			
		} //if za org		
    	
//obrada DECLARE direktive:

		if((brojTokena > 126) && (brojTokena < 130))
		{
			//velicina podataka koji se deklarisu
			int inkrement;
			int brojIteracija = 0;
			if(brojTokena == 127){ inkrement = 1; }
			if(brojTokena == 128){ inkrement = 2; }
			if(brojTokena == 129){ inkrement = 4; }
			
			//da li direktiva ima DUP
			int imaDUP = 0;
			int j;
			
			if(i<tokeni.size()-1) { j = i+1; }
			else { greska = 1; break; }
			string pom = tokeni[j];
			string izraz = "";
			
			//pretraga da li direktiva sadrzi i DUP direktivu
			while((pom[0] != ';') && (j<tokeni.size())) 
			{ 
				if((pom == "DUP") || (pom == "dup")) { imaDUP = 1; break; }
				else { j++; if(j<tokeni.size()) { pom = tokeni[j]; }  } 
			}
			
			//slucaj kada ima DUP
			if(imaDUP == 1)
			{
				if(i<tokeni.size()-1) { i++; }
				else { greska = 1; break; }
				string pom = tokeni[i];
				string izraz = "";
			
				//citanje izraza ispred DUP
				while((pom != "DUP") && (pom != "dup") && (i<tokeni.size())) 
				{ 
					izraz += pom;
					if(i<tokeni.size()-1) { i++; pom = tokeni[i]; }
					else { i++; } 
				}
			
				//racunanje vrednosti pre DUP
				vector<string> t1 = obradiIzraz(izraz);
				int provera = proveriIzraz(t1);
				if(provera != 0) { greska = 1; poruka = "Izraz pre DUP nije konstantan!"; break; }
				vector<string> t2 = infix2postfix(t1);
				brojIteracija = izracunajKonstantu(t2);
				
				if(brojIteracija<0) { greska = 1; poruka = "Izraz pre DUP je negativan!"; break; }
				
				if(i<tokeni.size()-1) { i++; }
				else { greska = 1; break; }
				pom = tokeni[i];
			
				//koliko ima komponenti iza DUP
				int brojKomponenti = 0;
				
				int jj = 0;
				while(jj<linija.length()) 
				{ 
				 	if((linija[jj] == 'd') && (linija[jj+1] == 'u') && (linija[jj+2] == 'p')) { break; }
				 	if((linija[jj] == 'D') && (linija[jj+1] == 'U') && (linija[jj+2] == 'P')) { break; }
				 	jj++; 
				}
				 	 
				jj+=3;
				
				pom = "";
				
				while((linija[jj] != ';') && (jj<linija.length())) 
				{ 
					pom+=linija[jj];
					jj++;
				}
				
				t1 = obradiIzrazSaZarezima(pom);
				brojKomponenti = t1.size();
				
				lc = lc + inkrement*brojIteracija*brojKomponenti;
				break;
			}
			else
			{
				int brojKomponenti = 0;
				
				int jj = 0;
				while(jj<linija.length()) 
				{ 
					if((linija[jj] == 'd') && (linija[jj+1] == 'b')) { break; }
				 	if((linija[jj] == 'D') && (linija[jj+1] == 'B')) { break; }
				 	if((linija[jj] == 'd') && (linija[jj+1] == 'w')) { break; }
				 	if((linija[jj] == 'D') && (linija[jj+1] == 'W')) { break; }
				 	if((linija[jj] == 'd') && (linija[jj+1] == 'd')) { break; }
				 	if((linija[jj] == 'D') && (linija[jj+1] == 'D')) { break; }
				 	jj++;
				}
				 	 
				jj+=2;
				
				pom = "";
				
				while((linija[jj] != ';') && (jj<linija.length())) 
				{ 
					pom+=linija[jj];
					jj++;
				}
				
				vector<string> t1 = obradiIzrazSaZarezima(pom);
				brojKomponenti = t1.size();
				
				lc = lc + inkrement*brojKomponenti;
				break;
			}
			
		} //if DECLARE direktive
    		
    	} //for petlja za jedan red
    	
	} //while petlja ulaznog fajla
	 
	if(greska == 1) 
	{
		izlaz << poruka << endl;
	 	return; 
	 }
	tabelaSimbola.ispisiTabeluSimbola(izlaz);
	
} //Kraj prvog prolaza

//----------------------------------------------------------------------------------------

void Asembler::drugiProlaz()
{
	if(greska == 1) { return; }

	ulaz.close();
	ulaz.open("ulaz.txt");
	
	int krajPrograma = 0;
	trenutnaSekcija = 0;
	lc = 0;
	
	vector<int> sekcija;  

	izlaz << endl;	
	
	string linija;
	while(getline(ulaz, linija))
	{
		if((greska == 1) || (krajPrograma == 1)) { break; }
	
		int brojTokena;
    	vector<string> tokeni = obradiLiniju(linija); 
    	
    	for (int i=0; i<tokeni.size(); i++) 
    	{
    		string rec = tokeni[i];
    		brojTokena = vratiToken(rec);
    		
    		//komentari
    		if(rec[0] == ';') { break; }
    		
    		//labele
    		if(rec[rec.length()-1] == ':') { continue; }
    		
    		
    		//sekcije
    		if((brojTokena > 132) && (brojTokena < 137)) 
    		{
    			if(trenutnaSekcija != 0)
    			{
    				Simbol *p = tabelaSimbola.pronadjiPoBroju(trenutnaSekcija);
    				
    				string sek = p->dohvatiNaziv();
    				
    				//provera da li je bss sekcija i da li ima sadrzaj
    				if(vratiToken(sek) == 136)
    				{
    					int pr = proveriBSS(sekcija);
    					if(pr == -1) { greska = 1; poruka = "BSS sekcija ima sadrzaj!"; break; }
    					sekcija.clear();
    				}
    				//ispis tabele relokacija i sadrzaja prethodne sekcije
    				else
    				{
    					if(tabelaRelokacija.pocetak != 0)
    					{ 
    						tabelaRelokacija.ispisiTabeluRelokacija(izlaz, p->dohvatiNaziv());
    						tabelaRelokacija.isprazniTebelu();
    					}
    					ispisiSekciju(sekcija);
    					sekcija.clear();
    				}
    			}
    			
    			Simbol *p = tabelaSimbola.pronadjiPoNazivu(rec);
    			trenutnaSekcija = p->dohvatiRedniBroj();
    			lc = p->dohvatiAdresu();
    			break; 
    		}
    		
    		
    		//end direktiva
    		if(brojTokena == 157) 
    		{ 
    			if(trenutnaSekcija != 0)
    			{	
    				Simbol *p = tabelaSimbola.pronadjiPoBroju(trenutnaSekcija);
    				
    				string sek = p->dohvatiNaziv();
    				
    				if(vratiToken(sek) == 136)
    				{
    					int pr = proveriBSS(sekcija);
    					if(pr == -1) { greska = 1; poruka = "BSS sekcija ima sadrzaj!"; break; }
    					sekcija.clear();
    					izlaz << "#end";
    				}
    				else
    				{
    					if(tabelaRelokacija.pocetak != 0)
    					{
    						tabelaRelokacija.ispisiTabeluRelokacija(izlaz, p->dohvatiNaziv());
    					}
    					ispisiSekciju(sekcija);
    					sekcija.clear();
    					izlaz << "#end";
    				}
    			}
    			
    			krajPrograma = 1; 
    			break; 
    		}
    		
    		
    		//global direktiva
    		if(brojTokena == 156) { break; }
    		
    		
    		//org direktiva
    		if(brojTokena == 132) { break; }
    		
    		
    		//def direktiva
    		if(brojTokena == 131) { break; }
    		
    		
    		//declare direktive
    		if((brojTokena > 126) && (brojTokena < 130))
			{
				int inkrement;
				int brojIteracija = 0;
				if(brojTokena == 127){ inkrement = 1; }
				if(brojTokena == 128){ inkrement = 2; }
				if(brojTokena == 129){ inkrement = 4; }
			
				int imaDUP = 0;
				int j;
			
				if(i<tokeni.size()-1) { j = i+1; }
				else { greska = 1; break; }
				string pom = tokeni[j];
				string izraz = "";
			
				while((pom[0] != ';') && (j<tokeni.size())) 
				{ 
					if((pom == "DUP") || (pom == "dup")) { imaDUP = 1; break; }
					else { j++; if(j<tokeni.size()) { pom = tokeni[j]; }  } 
				}
			
				if(imaDUP == 1)
				{
					if(i<tokeni.size()-1) { i++; }
					else { greska = 1; break; }
					string pom = tokeni[i];
					string izraz = "";
			
					while((pom != "DUP") && (pom != "dup") && (i<tokeni.size())) 
					{ 
						izraz += pom;
						if(i<tokeni.size()-1) { i++; pom = tokeni[i]; }
						else { i++; } 
					}
			
					vector<string> t1 = obradiIzraz(izraz);
					int provera = proveriIzraz(t1);
					if(provera != 0) { greska = 1; poruka = "Izraz pre DUP nije konstantan!"; break; }
					vector<string> t2 = infix2postfix(t1);
					brojIteracija = izracunajKonstantu(t2);
				
					if(i<tokeni.size()-1) { i++; }
					else { greska = 1; break; }
					pom = tokeni[i];
			
					int brojKomponenti = 0;
				
					int jj = 0;
				 	
				 	while(jj<linija.length()) 
				 	{ 
				 		if((linija[jj] == 'd') && (linija[jj+1] == 'u') && (linija[jj+2] == 'p')) { break; }
				 		if((linija[jj] == 'D') && (linija[jj+1] == 'U') && (linija[jj+2] == 'P')) { break; }
				 		jj++; 
				 	}
				 	 
					jj+=3;
				
					pom = "";
				
					while((linija[jj] != ';') && (jj<linija.length())) 
					{ 
						pom+=linija[jj];
						jj++;
					}
					
					t1 = obradiIzrazSaZarezima(pom);
					brojKomponenti = t1.size();
					int niz[brojKomponenti];
					int trebaRelok[brojKomponenti];
					int brojRelok[brojKomponenti];
					for(int brk = 0; brk < brojKomponenti; brk++) 
					{ 
						trebaRelok[brk] = 0;
						brojRelok[brk] = 0; 
					}
						
					//dohvatanje vrednosti iza DUP
					for(int kk=0; kk<t1.size(); kk++)
					{
						if(t1[kk] == "?")
						{
							niz[kk] = 0;
							continue;
						}	
						t2 = obradiIzraz(t1[kk]);
						provera = proveriIzraz(t2);
						if(provera == -1) { greska = 1; break; }
						else if(provera >= 0)
						{
							//izraz nije konstantan, ali je razresiv relokacijom
							if(provera == 1) 
							{ 
								trebaRelok[kk] = 1; 
								if(relokacija == 0) { brojRelok[kk] = globalniSimbol; }
								else { brojRelok[kk] = relokacija; }	 
							}
							vector<string> t3 = infix2postfix(t2);
							niz[kk] = izracunajKonstantu(t3);
						}
					}
					
					//upis vrednosti u memoriju i dodavanje ulaza u tabelu relokacija, ako je potrebno
					for(int mm=0; mm<brojIteracija; mm++)
					{
						for(int kk=0; kk<t1.size(); kk++)
						{
							if(inkrement == 1) 
							{
								sekcija.push_back(niz[kk]);
								
								if(trebaRelok[kk] == 1)
								{
									Relokacija *r = new Relokacija(lc, "A", brojRelok[kk]);
    								tabelaRelokacija.dodajRelokaciju(r);
								}
								
								lc+=1; 
							}
							else if(inkrement == 2)
							{
								int prvi = niz[kk] >> 8;
								int drugi = niz[kk] & 0xff;
								sekcija.push_back(drugi);
								sekcija.push_back(prvi);
								
								if(trebaRelok[kk] == 1)
								{
									Relokacija *r = new Relokacija(lc, "A", brojRelok[kk]);
    								tabelaRelokacija.dodajRelokaciju(r);
								}
								
								lc+=2;
							}
							else if(inkrement == 4)
							{
								int prvi = niz[kk] >> 24;
								int drugi = (niz[kk] & 0x00ff0000) >> 16;
								int treci = (niz[kk] & 0x0000ff00) >> 8;
								int cetvrti = niz[kk] & 0x000000ff;
								sekcija.push_back(cetvrti);
								sekcija.push_back(treci);
								sekcija.push_back(drugi);
								sekcija.push_back(prvi);
								
								if(trebaRelok[kk] == 1)
								{
									Relokacija *r = new Relokacija(lc, "A", brojRelok[kk]);
    								tabelaRelokacija.dodajRelokaciju(r);
								}
								
								lc+=4;
							}
						}
					}
					break;
				}
				else
				{
					int brojKomponenti = 0;
				
					int jj = 0;
					while(jj<linija.length()) 
				 	{ 
				 		if((linija[jj] == 'd') && (linija[jj+1] == 'b')) { break; }
				 		if((linija[jj] == 'D') && (linija[jj+1] == 'B')) { break; }
				 		if((linija[jj] == 'd') && (linija[jj+1] == 'w')) { break; }
				 		if((linija[jj] == 'D') && (linija[jj+1] == 'W')) { break; }
				 		if((linija[jj] == 'd') && (linija[jj+1] == 'd')) { break; }
				 		if((linija[jj] == 'D') && (linija[jj+1] == 'D')) { break; }
				 		jj++;
				 	}
				 	 
					jj+=2;
				
					pom = "";
				
					while((linija[jj] != ';') && (jj<linija.length())) 
					{	 
						pom+=linija[jj];
						jj++;
					}
				
					vector<string> t1 = obradiIzrazSaZarezima(pom);
					brojKomponenti = t1.size();
					
					//upis deklarisanih vrednosti u memoriju
					for(int kk=0; kk<t1.size(); kk++)
					{
						if(t1[kk] == "?")
						{
							if(inkrement == 1) 
							{ sekcija.push_back(0); }
							else if(inkrement == 2) 
							{ sekcija.push_back(0); sekcija.push_back(0); }
							else if(inkrement == 4)
							{ sekcija.push_back(0); sekcija.push_back(0); sekcija.push_back(0); sekcija.push_back(0); }
							
							lc+=inkrement;
							continue;
						}
					
						vector<string> t2 = obradiIzraz(t1[kk]);
						int provera = proveriIzraz(t2);
						if(provera == -1) { greska = 1; break; }
						else if(provera >= 0)
						{
							int relok;
							if(provera == 1)
							{
								if(relokacija == 0) { relok = globalniSimbol; }
								else { relok = relokacija; }
								Relokacija *r = new Relokacija(lc, "A", relok);
    							tabelaRelokacija.dodajRelokaciju(r);
							}
							
							vector<string> t3 = infix2postfix(t2);
							int vr = izracunajKonstantu(t3);
							
							if(inkrement == 1) 
							{
								sekcija.push_back(vr);
								lc+=1; 
							}
							else if(inkrement == 2)
							{
								int prvi = vr >> 8;
								int drugi = vr & 0xff;
								sekcija.push_back(drugi);
								sekcija.push_back(prvi);
								lc+=2;
							}
							else if(inkrement == 4)
							{
								int prvi = vr >> 24;
								int drugi = (vr & 0x00ff0000) >> 16;
								int treci = (vr & 0x0000ff00) >> 8;
								int cetvrti = vr & 0x000000ff;
								sekcija.push_back(cetvrti);
								sekcija.push_back(treci);
								sekcija.push_back(drugi);
								sekcija.push_back(prvi);
								lc+=4;
							}
						}
					}
					break;
				}
		} //kraj declare direktive
    		
    		
//obrada instrukcija - drugi prolaz:
    		
    		if((brojTokena > 1) && (brojTokena < 35))
			{
				Simbol *p = tabelaSimbola.pronadjiPoBroju(trenutnaSekcija);
				string nazivTrenutneSekcija = p->dohvatiNaziv();
				if((trenutnaSekcija == 0) || (vratiToken(nazivTrenutneSekcija) != 133))
				{ greska = 1; poruka = "Instrukcije van text sekcije nisu dozvoljene!"; break; }
			
				int adr;
				int instr = 0;
				int r0 = 0, r1 = 0, r2 = 0;
				
				//INT
				if(brojTokena == 2)
				{	
					i++;
					rec = tokeni[i];
					r0 = vratiRegistar(vratiToken(rec));
					if(r0 == -1) { greska = 1; poruka = "Registar ne postoji(INT)!"; break; }
					r0 = r0 << 16;
					lc+=4;
					
					//sklapanje instrukcije i upis u memoriju
					instr = sloziInstrukciju(INT, REG_DIR, r0, 0, 0, 0);
					
					int upis[4];
					upis[0] = instr >> 24;
					upis[1] = (instr & 0x00ff0000) >> 16;
					upis[2] = (instr & 0x0000ff00) >> 8;
					upis[3] = instr & 0x000000ff;
					for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
					
					break;
				}//kraj INT
				
				
				//JMP i CALL
				if((brojTokena == 3) || (brojTokena == 4))
				{
					i++;
					rec = tokeni[i];
					adr = vratiAdresiranje(rec);
					
					string pom = "";
					int instrukcija;
					if(brojTokena == 3) { instrukcija = JMP; }
					if(brojTokena == 4) { instrukcija = CALL; }
					
					if(adr == 3) 
					{ 
						adr = MEM_DIR;
						instr = sloziInstrukciju(instrukcija, MEM_DIR, 0, 0, 0, 0);
						
						int upis[4];
						upis[0] = instr >> 24;
						upis[1] = (instr & 0x00ff0000) >> 16;
						upis[2] = (instr & 0x0000ff00) >> 8;
						upis[3] = instr & 0x000000ff;
						for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
						
						while(i<tokeni.size())
						{
							rec = tokeni[i];
							if(rec[0] == ';') { break; }
							pom+=rec;
							i++;
						}
						
						vector<string> t1 = obradiIzraz(pom);
						int provera = proveriIzraz(t1);
						if(provera == -1) { greska = 1; break; }
						
						if(provera == 1)
						{
							int rbRelok = relokacija;
							if(relokacija == 0) { rbRelok = globalniSimbol; }
							
							Relokacija *r = new Relokacija(lc+4, "A", rbRelok);
    						tabelaRelokacija.dodajRelokaciju(r);
						}
						
						vector<string> t2 = infix2postfix(t1);
						int vr = izracunajKonstantu(t2);
						int prvi = vr >> 24;
						int drugi = (vr & 0x00ff0000) >> 16;
						int treci = (vr & 0x0000ff00) >> 8;
						int cetvrti = vr & 0x000000ff;
						sekcija.push_back(cetvrti);
						sekcija.push_back(treci);
						sekcija.push_back(drugi);
						sekcija.push_back(prvi);
						
						lc += 8;
						break;	
					}
					else if(adr == 5) 
					{ 	 
						adr = REG_IND_POM;
						
						//slucaj kada je reg_ind_pom zapisano sa $
						if(rec[0] == '$')
						{
							r0 = 0x11 << 16;
							instr = sloziInstrukciju(instrukcija, REG_IND_POM, r0, 0, 0, 0);
						
							int upis[4];
							upis[0] = instr >> 24;
							upis[1] = (instr & 0x00ff0000) >> 16;
							upis[2] = (instr & 0x0000ff00) >> 8;
							upis[3] = instr & 0x000000ff;
							for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
						
							int u = 0;
							pom = "";
							
							while((u<linija.length()) && (linija[u] != '$')) { u++; }
							u++;
							pom = "";
							while((u<linija.length()) && (linija[u] != ';')) 
							{ 
								pom+=linija[u]; u++; 
							}
							
							vector<string> t1 = obradiIzraz(pom);
							int provera = proveriIzraz(t1);
							if(provera == -1) { greska = 1; break; }
							
							vector<string> t2 = infix2postfix(t1);
							int vr = izracunajKonstantu(t2);
							
							if((provera == 1) && (relokacija != trenutnaSekcija))
							{
								int rbRelok = relokacija;
								if(relokacija == 0) { rbRelok = globalniSimbol; }
								Relokacija *r = new Relokacija(lc+4, "R", rbRelok);
    							tabelaRelokacija.dodajRelokaciju(r);
    							vr = vr - 4;
							}
							else if((provera == 1) && (relokacija == trenutnaSekcija))
							{
								vr = vr - (lc + 8);
							}
							else if(provera == 0)
							{
								Relokacija *r = new Relokacija(lc+4, "R", 0);
    							tabelaRelokacija.dodajRelokaciju(r);
    							vr = vr - 4;
							}
							
							int prvi = vr >> 24;
							int drugi = (vr & 0x00ff0000) >> 16;
							int treci = (vr & 0x0000ff00) >> 8;
							int cetvrti = vr & 0x000000ff;
							sekcija.push_back(cetvrti);
							sekcija.push_back(treci);
							sekcija.push_back(drugi);
							sekcija.push_back(prvi);
						
							lc+=8;
							break;
						}
						
						int j = 1;
						
						while((rec[j] != ' ') && (rec[j] != '+') && (rec[j] != '-') && (j<rec.length())) { pom += rec[j]; j++; }
						r0 = vratiRegistar(vratiToken(pom));
						if(r0 == -1) { greska = 1; break; }
						r0 = r0 << 16;
						
						instr = sloziInstrukciju(instrukcija, REG_IND_POM, r0, 0, 0, 0);
						
						int upis[4];
						upis[0] = instr >> 24;
						upis[1] = (instr & 0x00ff0000) >> 16;
						upis[2] = (instr & 0x0000ff00) >> 8;
						upis[3] = instr & 0x000000ff;
						for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
						
						pom = "";
						while((rec[j] != ']') && (j<rec.length())) 
						{ 
							pom+=rec[j]; 
							j++; 
						}
						
						vector<string> t1 = obradiIzraz(pom);
						int provera = proveriIzraz(t1);
						if(provera == -1) { greska = 1; break; }
						
						if(provera == 1)
						{
							int rbRelok = relokacija;
							if(relokacija == 0) { rbRelok = globalniSimbol; }
							
							Relokacija *r = new Relokacija(lc+4, "A", rbRelok);
    						tabelaRelokacija.dodajRelokaciju(r);
						}
						
						vector<string> t2 = infix2postfix(t1);
						int vr = izracunajKonstantu(t2); 
						
						int prvi = vr >> 24;
						int drugi = (vr & 0x00ff0000) >> 16;
						int treci = (vr & 0x0000ff00) >> 8;
						int cetvrti = vr & 0x000000ff;
						sekcija.push_back(cetvrti);
						sekcija.push_back(treci);
						sekcija.push_back(drugi);
						sekcija.push_back(prvi);
						
						lc+=8;	
					}
					else if(adr == 4) 
					{
						lc+=4;  
						adr = REG_IND;
						int j = 1;
						while((rec[j] != ']') && (j<rec.length())) { pom += rec[j]; j++; }
						r0 = vratiRegistar(vratiToken(pom));
						if(r0 == -1) { greska = 1; break; }
						r0 = r0 << 16;
						
						instr = sloziInstrukciju(instrukcija, REG_IND, r0, 0, 0, 0);
						
						int upis[4];
						upis[0] = instr >> 24;
						upis[1] = (instr & 0x00ff0000) >> 16;
						upis[2] = (instr & 0x0000ff00) >> 8;
						upis[3] = instr & 0x000000ff;
						for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
					}
					
					break;
				} //kraj JMP i CALL
				
				
				//RET
				if(brojTokena == 5)
				{
					lc+=4;
					instr = sloziInstrukciju(RET, 0, 0, 0, 0, 0);
					
					int upis[4];
					upis[0] = instr >> 24;
					upis[1] = (instr & 0x00ff0000) >> 16;
					upis[2] = (instr & 0x0000ff00) >> 8;
					upis[3] = instr & 0x000000ff;
					for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
					
					break;
				} //kraj RET
				
				
				//JZ, JNZ, JGZ, JGEZ, JLZ, JLEZ
				if((brojTokena > 5) && (brojTokena < 12))
				{
					string pom = "";
					int instrukcija;
					if(brojTokena == 6) { instrukcija = JZ; }
					if(brojTokena == 7) { instrukcija = JNZ; }
					if(brojTokena == 8) { instrukcija = JGZ; }
					if(brojTokena == 9) { instrukcija = JGEZ; }
					if(brojTokena == 10) { instrukcija = JLZ; }
					if(brojTokena == 11) { instrukcija = JLEZ; }
					
					i++;
					rec = tokeni[i];
					r0 = vratiRegistar(vratiToken(rec));
					if(r0 == -1) { greska = 1; break; }
					r0 = r0 << 16;
					
					i++;
					rec = tokeni[i];
					adr = vratiAdresiranje(rec);
						
					if(adr == 3)
					{ 
						adr = MEM_DIR;
						instr = sloziInstrukciju(instrukcija, MEM_DIR, r0, 0, 0, 0);
						
						int upis[4];
						upis[0] = instr >> 24;
						upis[1] = (instr & 0x00ff0000) >> 16;
						upis[2] = (instr & 0x0000ff00) >> 8;
						upis[3] = instr & 0x000000ff;
						for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
						
						while(i<tokeni.size())
						{
							rec = tokeni[i];
							if(rec[0] == ';') { break; }
							pom+=rec;
							i++;
						}
						
						vector<string> t1 = obradiIzraz(pom);
						int provera = proveriIzraz(t1);
						if(provera == -1) { greska = 1; break; }
						
						if(provera == 1)
						{
							int rbRelok = relokacija;
							if(relokacija == 0) { rbRelok = globalniSimbol; }
							
							Relokacija *r = new Relokacija(lc+4, "A", rbRelok);
    						tabelaRelokacija.dodajRelokaciju(r);
						}
						
						
						vector<string> t2 = infix2postfix(t1);
						int vr = izracunajKonstantu(t2);
						int prvi = vr >> 24;
						int drugi = (vr & 0x00ff0000) >> 16;
						int treci = (vr & 0x0000ff00) >> 8;
						int cetvrti = vr & 0x000000ff;
						sekcija.push_back(cetvrti);
						sekcija.push_back(treci);
						sekcija.push_back(drugi);
						sekcija.push_back(prvi);
						
						lc += 8;
						break;
					}
					else if(adr == 4)
					{ 
						lc+=4;  
						adr = REG_IND;
						int j = 1;
						while((rec[j] != ']') && (j<rec.length())) { pom += rec[j]; j++; }
						r1 = vratiRegistar(vratiToken(pom));
						if(r1 == -1) { greska = 1; break; }
						r1 = r1 << 11;
						
						instr = sloziInstrukciju(instrukcija, REG_IND, r0, r1, 0, 0);
						
						int upis[4];
						upis[0] = instr >> 24;
						upis[1] = (instr & 0x00ff0000) >> 16;
						upis[2] = (instr & 0x0000ff00) >> 8;
						upis[3] = instr & 0x000000ff;
						for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); } 
					}
					else if(adr == 5)
					{  
						adr = REG_IND_POM;
						
						if(rec[0] == '$')
						{
							r1 = 0x11 << 11;
							instr = sloziInstrukciju(instrukcija, REG_IND_POM, r0, r1, 0, 0);
						
							int upis[4];
							upis[0] = instr >> 24;
							upis[1] = (instr & 0x00ff0000) >> 16;
							upis[2] = (instr & 0x0000ff00) >> 8;
							upis[3] = instr & 0x000000ff;
							for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
							
							int u = 0;
							pom = "";
							
							while((u<linija.length()) && (linija[u] != '$')) { u++; }
							u++;
							pom = "";
							while((u<linija.length()) && (linija[u] != ';')) 
							{ 
								pom+=linija[u]; u++; 
							}
							
							vector<string> t1 = obradiIzraz(pom);
							int provera = proveriIzraz(t1);
							if(provera == -1) { greska = 1; break; }
							vector<string> t2 = infix2postfix(t1);
							int vr = izracunajKonstantu(t2);
							
							if((provera == 1) && (relokacija != trenutnaSekcija))
							{
								int rbRelok = relokacija;
								if(relokacija == 0) { rbRelok = globalniSimbol; }
								Relokacija *r = new Relokacija(lc+4, "R", rbRelok);
    							tabelaRelokacija.dodajRelokaciju(r);
    							vr = vr - 4;
							}
							else if((provera == 1) && (relokacija == trenutnaSekcija))
							{	 
								vr = vr - (lc + 8);
							}
							else if(provera == 0)
							{
								Relokacija *r = new Relokacija(lc+4, "R", 0);
    							tabelaRelokacija.dodajRelokaciju(r);
    							vr = vr - 4;
							}
							
							int prvi = vr >> 24;
							int drugi = (vr & 0x00ff0000) >> 16;
							int treci = (vr & 0x0000ff00) >> 8;
							int cetvrti = vr & 0x000000ff;
							sekcija.push_back(cetvrti);
							sekcija.push_back(treci);
							sekcija.push_back(drugi);
							sekcija.push_back(prvi);
						
							lc+=8;
							break;
						}
						
						int j = 1;
						while((rec[j] != ' ') && (rec[j] != '+') && (rec[j] != '-') && (j<rec.length())) { pom += rec[j]; j++; }
						r1 = vratiRegistar(vratiToken(pom));
						if(r1 == -1) { greska = 1; break; }
						r1 = r1 << 11;
						
						instr = sloziInstrukciju(instrukcija, REG_IND_POM, r0, r1, 0, 0);
						
						int upis[4];
						upis[0] = instr >> 24;
						upis[1] = (instr & 0x00ff0000) >> 16;
						upis[2] = (instr & 0x0000ff00) >> 8;
						upis[3] = instr & 0x000000ff;
						for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
						
						pom = "";
						while((rec[j] != ']') && (j<rec.length())) 
						{ 
							pom+=rec[j]; 
							j++; 
						}
						
						vector<string> t1 = obradiIzraz(pom);
						int provera = proveriIzraz(t1);
						if(provera == -1) { greska = 1; break; }
						
						if(provera == 1)
						{
							int rbRelok = relokacija;
							if(relokacija == 0) { rbRelok = globalniSimbol; }
							
							Relokacija *r = new Relokacija(lc+4, "A", rbRelok);
    						tabelaRelokacija.dodajRelokaciju(r);
						}
						
						vector<string> t2 = infix2postfix(t1);
						int vr = izracunajKonstantu(t2);
						int prvi = vr >> 24;
						int drugi = (vr & 0x00ff0000) >> 16;
						int treci = (vr & 0x0000ff00) >> 8;
						int cetvrti = vr & 0x000000ff;
						sekcija.push_back(cetvrti);
						sekcija.push_back(treci);
						sekcija.push_back(drugi);
						sekcija.push_back(prvi); 
						
						lc+=8;
					}
						
					break;	
				} //kraj skokova
				
				
				//LOAD
				if((brojTokena > 11) && (brojTokena < 17))
				{
					string pom = "";
					int instrukcija = LOAD;
					
					int dataT;
					if(brojTokena == 12) { dataT = DOUBLE; }
					if(brojTokena == 13) { dataT = UB; }
					if(brojTokena == 14) { dataT = SB; }
					if(brojTokena == 15) { dataT = UWORD; }
					if(brojTokena == 16) { dataT = SWORD; }
					
					i++;
					rec = tokeni[i];
					r0 = vratiRegistar(vratiToken(rec));
					if(r0 == -1) { greska = 1; poruka = "Registar ne postoji(LOAD)!"; break; }
					r0 = r0 << 16;
					
					i++;
					rec = tokeni[i];
					adr = vratiAdresiranje(rec);
					
					if(adr == 1)
					{ 
						adr = IMM;
						instr = sloziInstrukciju(instrukcija, IMM, r0, 0, 0, dataT);
						
						int upis[4];
						upis[0] = instr >> 24;
						upis[1] = (instr & 0x00ff0000) >> 16;
						upis[2] = (instr & 0x0000ff00) >> 8;
						upis[3] = instr & 0x000000ff;
						for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
						
						pom = "";
						int j = 1;
						while(j<rec.length()) { pom+=rec[j]; j++;}
						if(i<tokeni.size()-1)
						{
							i++;
							
							while(i<tokeni.size())
							{
								rec = tokeni[i];
								if(rec[0] == ';') { break; }
								pom+=rec;
								i++;
							}
						}
						
						vector<string> t1 = obradiIzraz(pom);
						int provera = proveriIzraz(t1);
						if(provera == -1) { greska = 1; break; }
						
						if(provera == 1)
						{
							int rbRelok = relokacija;
							if(relokacija == 0) { rbRelok = globalniSimbol; }
							
							Relokacija *r = new Relokacija(lc+4, "A", rbRelok);
    						tabelaRelokacija.dodajRelokaciju(r);
						}
						
						vector<string> t2 = infix2postfix(t1);
						int vr = izracunajKonstantu(t2);
						int prvi = vr >> 24;
						int drugi = (vr & 0x00ff0000) >> 16;
						int treci = (vr & 0x0000ff00) >> 8;
						int cetvrti = vr & 0x000000ff;
						sekcija.push_back(cetvrti);
						sekcija.push_back(treci);
						sekcija.push_back(drugi);
						sekcija.push_back(prvi);
						
						lc += 8;
						
					}
					else if(adr == 2)
					{
						lc+=4;
						adr = REG_DIR;
						r1 = vratiRegistar(vratiToken(rec));
						if(r1 == -1) { greska = 1; poruka = "Registar ne postoji(LOAD)!"; break; }
						r1 = r1 << 11;
						
						instr = sloziInstrukciju(instrukcija, REG_DIR, r0, r1, 0, dataT);
						
						int upis[4];
						upis[0] = instr >> 24;
						upis[1] = (instr & 0x00ff0000) >> 16;
						upis[2] = (instr & 0x0000ff00) >> 8;
						upis[3] = instr & 0x000000ff;
						for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); } 
					}
					else if(adr == 3)
					{ 
						adr = MEM_DIR;
						instr = sloziInstrukciju(instrukcija, MEM_DIR, r0, 0, 0, dataT);
						
						int upis[4];
						upis[0] = instr >> 24;
						upis[1] = (instr & 0x00ff0000) >> 16;
						upis[2] = (instr & 0x0000ff00) >> 8;
						upis[3] = instr & 0x000000ff;
						for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
						
						while(i<tokeni.size())
						{
							rec = tokeni[i];
							if(rec[0] == ';') { break; }
							pom+=rec;
							i++;
						}
						
						vector<string> t1 = obradiIzraz(pom);
						int provera = proveriIzraz(t1);
						if(provera == -1) { greska = 1; break; }
						
						if(provera == 1)
						{
							int rbRelok = relokacija;
							if(relokacija == 0) { rbRelok = globalniSimbol; }
							
							Relokacija *r = new Relokacija(lc+4, "A", rbRelok);
    						tabelaRelokacija.dodajRelokaciju(r);
						}
						
						vector<string> t2 = infix2postfix(t1);
						int vr = izracunajKonstantu(t2);
						int prvi = vr >> 24;
						int drugi = (vr & 0x00ff0000) >> 16;
						int treci = (vr & 0x0000ff00) >> 8;
						int cetvrti = vr & 0x000000ff;
						sekcija.push_back(cetvrti);
						sekcija.push_back(treci);
						sekcija.push_back(drugi);
						sekcija.push_back(prvi);
						
						lc += 8;
					}
					else if(adr == 4)
					{
						lc+=4;  
						adr = REG_IND;
						int j = 1;
						while((rec[j] != ']') && (j<rec.length())) { pom += rec[j]; j++; }
						r1 = vratiRegistar(vratiToken(pom));
						if(r1 == -1) { greska = 1; poruka = "Registar ne postoji(LOAD)!"; break; }
						r1 = r1 << 11;
						
						instr = sloziInstrukciju(instrukcija, REG_IND, r0, r1, 0, dataT);
						
						int upis[4];
						upis[0] = instr >> 24;
						upis[1] = (instr & 0x00ff0000) >> 16;
						upis[2] = (instr & 0x0000ff00) >> 8;
						upis[3] = instr & 0x000000ff;
						for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
					}
					else if(adr == 5)
					{
						adr = REG_IND_POM;
						
						if(rec[0] == '$')
						{
							r1 = 0x11 << 11;
							instr = sloziInstrukciju(instrukcija, REG_IND_POM, r0, r1, 0, dataT);
						
							int upis[4];
							upis[0] = instr >> 24;
							upis[1] = (instr & 0x00ff0000) >> 16;
							upis[2] = (instr & 0x0000ff00) >> 8;
							upis[3] = instr & 0x000000ff;
							for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
							
							int u = 0;
							pom = "";
							
							while((u<linija.length()) && (linija[u] != '$')) { u++; }
							u++;
							pom = "";
							while((u<linija.length()) && (linija[u] != ';')) 
							{ 
								pom+=linija[u]; u++; 
							}
							
							vector<string> t1 = obradiIzraz(pom);
							int provera = proveriIzraz(t1);
							if(provera == -1) { greska = 1; break; }
							vector<string> t2 = infix2postfix(t1);
							int vr = izracunajKonstantu(t2);
							
							if((provera == 1) && (relokacija != trenutnaSekcija))
							{
								int rbRelok = relokacija;
								if(relokacija == 0) { rbRelok = globalniSimbol; }
								Relokacija *r = new Relokacija(lc+4, "R", rbRelok);
    							tabelaRelokacija.dodajRelokaciju(r);
    							vr = vr - 4;
							}
							else if((provera == 1) && (relokacija == trenutnaSekcija))
							{
								vr = vr - (lc + 8);
							}
							else if(provera == 0)
							{
								Relokacija *r = new Relokacija(lc+4, "R", 0);
    							tabelaRelokacija.dodajRelokaciju(r);
    							vr = vr - 4;
							}
							
							int prvi = vr >> 24;
							int drugi = (vr & 0x00ff0000) >> 16;
							int treci = (vr & 0x0000ff00) >> 8;
							int cetvrti = vr & 0x000000ff;
							sekcija.push_back(cetvrti);
							sekcija.push_back(treci);
							sekcija.push_back(drugi);
							sekcija.push_back(prvi);
						
							lc+=8;
							break;
						}
						
						int j = 1;
						while((rec[j] != ' ') && (rec[j] != '+') && (rec[j] != '-') && (j<rec.length())) { pom += rec[j]; j++; }
						r1 = vratiRegistar(vratiToken(pom));
						if(r1 == -1) { greska = 1; poruka = "Registar ne postoji(LOAD)!"; break; }
						r1 = r1 << 11;
						
						instr = sloziInstrukciju(instrukcija, REG_IND_POM, r0, r1, 0, dataT);
						
						int upis[4];
						upis[0] = instr >> 24;
						upis[1] = (instr & 0x00ff0000) >> 16;
						upis[2] = (instr & 0x0000ff00) >> 8;
						upis[3] = instr & 0x000000ff;
						for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
						
						pom = "";
						while((rec[j] != ']') && (j<rec.length())) 
						{ 
							pom+=rec[j]; 
							j++; 
						}
						
						vector<string> t1 = obradiIzraz(pom);
						int provera = proveriIzraz(t1);
						if(provera == -1) { greska = 1; break; }
						
						if(provera == 1)
						{
							int rbRelok = relokacija;
							if(relokacija == 0) { rbRelok = globalniSimbol; }
							
							Relokacija *r = new Relokacija(lc+4, "A", rbRelok);
    						tabelaRelokacija.dodajRelokaciju(r);
						}
						
						vector<string> t2 = infix2postfix(t1);
						int vr = izracunajKonstantu(t2);
						int prvi = vr >> 24;
						int drugi = (vr & 0x00ff0000) >> 16;
						int treci = (vr & 0x0000ff00) >> 8;
						int cetvrti = vr & 0x000000ff;
						sekcija.push_back(cetvrti);
						sekcija.push_back(treci);
						sekcija.push_back(drugi);
						sekcija.push_back(prvi);
						
						lc+=8; 
					}
						
					break;	
				} //kraj LOAD
				
				
				//STORE
				if((brojTokena > 16) && (brojTokena < 20))
				{
					string pom = "";
					int instrukcija = STORE;
					
					int dataT;
					if(brojTokena == 17) { dataT = DOUBLE; }
					if(brojTokena == 18) { dataT = SWORD; }
					if(brojTokena == 19) { dataT = SB; }
					
					i++;
					rec = tokeni[i];
					r0 = vratiRegistar(vratiToken(rec));
					if(r0 == -1) { greska = 1; poruka = "Registar ne postoji(STORE)!"; break; }
					r0 = r0 << 16;
					
					i++;
					rec = tokeni[i];
					adr = vratiAdresiranje(rec);
					
					if(adr == 2)
					{
						lc+=4;
						adr = REG_DIR;
						r1 = vratiRegistar(vratiToken(rec));
						if(r1 == -1) { greska = 1; poruka = "Registar ne postoji(STORE)!"; break; }
						r1 = r1 << 11;
						
						instr = sloziInstrukciju(instrukcija, REG_DIR, r0, r1, 0, dataT);
						
						int upis[4];
						upis[0] = instr >> 24;
						upis[1] = (instr & 0x00ff0000) >> 16;
						upis[2] = (instr & 0x0000ff00) >> 8;
						upis[3] = instr & 0x000000ff;
						for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); } 
					}
					else if(adr == 3)
					{ 
						adr = MEM_DIR;
						instr = sloziInstrukciju(instrukcija, MEM_DIR, r0, 0, 0, dataT);
						
						int upis[4];
						upis[0] = instr >> 24;
						upis[1] = (instr & 0x00ff0000) >> 16;
						upis[2] = (instr & 0x0000ff00) >> 8;
						upis[3] = instr & 0x000000ff;
						for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
						
						while(i<tokeni.size())
						{
							rec = tokeni[i];
							if(rec[0] == ';') { break; }
							pom+=rec;
							i++;
						}
						
						vector<string> t1 = obradiIzraz(pom);
						int provera = proveriIzraz(t1);
						if(provera == -1) { greska = 1; break; }
						
						if(provera == 1)
						{
							int rbRelok = relokacija;
							if(relokacija == 0) { rbRelok = globalniSimbol; }
							
							Relokacija *r = new Relokacija(lc+4, "A", rbRelok);
    						tabelaRelokacija.dodajRelokaciju(r);
						}
						
						vector<string> t2 = infix2postfix(t1);
						int vr = izracunajKonstantu(t2);
						int prvi = vr >> 24;
						int drugi = (vr & 0x00ff0000) >> 16;
						int treci = (vr & 0x0000ff00) >> 8;
						int cetvrti = vr & 0x000000ff;
						sekcija.push_back(cetvrti);
						sekcija.push_back(treci);
						sekcija.push_back(drugi);
						sekcija.push_back(prvi);
						
						lc += 8;	
					}
					else if(adr == 4)
					{
						lc+=4;  
						adr = REG_IND;
						int j = 1;
						while((rec[j] != ']') && (j<rec.length())) { pom += rec[j]; j++; }
						r1 = vratiRegistar(vratiToken(pom));
						if(r1 == -1) { greska = 1; poruka = "Registar ne postoji(STORE)!"; break; }
						r1 = r1 << 11;
						
						instr = sloziInstrukciju(instrukcija, REG_IND, r0, r1, 0, dataT);
						
						int upis[4];
						upis[0] = instr >> 24;
						upis[1] = (instr & 0x00ff0000) >> 16;
						upis[2] = (instr & 0x0000ff00) >> 8;
						upis[3] = instr & 0x000000ff;
						for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
					}
					else if(adr == 5)
					{
						adr = REG_IND_POM;
						
						if(rec[0] == '$')
						{
							r1 = 0x11 << 11;
							instr = sloziInstrukciju(instrukcija, REG_IND_POM, r0, r1, 0, dataT);
						
							int upis[4];
							upis[0] = instr >> 24;
							upis[1] = (instr & 0x00ff0000) >> 16;
							upis[2] = (instr & 0x0000ff00) >> 8;
							upis[3] = instr & 0x000000ff;
							for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
							
							int u = 0;
							pom = "";
							
							while((u<linija.length()) && (linija[u] != '$')) { u++; }
							u++;
							pom = "";
							while((u<linija.length()) && (linija[u] != ';')) 
							{ 
								pom+=linija[u]; u++; 
							}
							
							vector<string> t1 = obradiIzraz(pom);
							int provera = proveriIzraz(t1);
							if(provera == -1) { greska = 1; break; }
							vector<string> t2 = infix2postfix(t1);
							int vr = izracunajKonstantu(t2);
							
							if((provera == 1) && (relokacija != trenutnaSekcija))
							{
								int rbRelok = relokacija;
								if(relokacija == 0) { rbRelok = globalniSimbol; }
								Relokacija *r = new Relokacija(lc+4, "R", rbRelok);
    							tabelaRelokacija.dodajRelokaciju(r);
    							vr = vr - 4;
							}
							else if((provera == 1) && (relokacija == trenutnaSekcija))
							{
								vr = vr - (lc + 8);
							}
							else if(provera == 0)
							{
								Relokacija *r = new Relokacija(lc+4, "R", 0);
    							tabelaRelokacija.dodajRelokaciju(r);
    							vr = vr - 4;
							}
							
							int prvi = vr >> 24;
							int drugi = (vr & 0x00ff0000) >> 16;
							int treci = (vr & 0x0000ff00) >> 8;
							int cetvrti = vr & 0x000000ff;
							sekcija.push_back(cetvrti);
							sekcija.push_back(treci);
							sekcija.push_back(drugi);
							sekcija.push_back(prvi);
						
							lc+=8; 
							break;
						}
						
						int j = 1;
						while((rec[j] != ' ') && (rec[j] != '+') && (rec[j] != '-') && (j<rec.length())) { pom += rec[j]; j++; }
						r1 = vratiRegistar(vratiToken(pom));
						if(r1 == -1) { greska = 1; break; }
						r1 = r1 << 11;
						
						instr = sloziInstrukciju(instrukcija, REG_IND_POM, r0, r1, 0, dataT);
						
						int upis[4];
						upis[0] = instr >> 24;
						upis[1] = (instr & 0x00ff0000) >> 16;
						upis[2] = (instr & 0x0000ff00) >> 8;
						upis[3] = instr & 0x000000ff;
						for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
						
						pom = "";
						while((rec[j] != ']') && (j<rec.length())) 
						{ 
							pom+=rec[j]; 
							j++; 
						}
						
						vector<string> t1 = obradiIzraz(pom);
						int provera = proveriIzraz(t1);
						if(provera == -1) { greska = 1; break; }
						
						if(provera == 1)
						{
							int rbRelok = relokacija;
							if(relokacija == 0) { rbRelok = globalniSimbol; }
							
							Relokacija *r = new Relokacija(lc+4, "A", rbRelok);
    						tabelaRelokacija.dodajRelokaciju(r);
						}
						
						vector<string> t2 = infix2postfix(t1);
						int vr = izracunajKonstantu(t2);
						int prvi = vr >> 24;
						int drugi = (vr & 0x00ff0000) >> 16;
						int treci = (vr & 0x0000ff00) >> 8;
						int cetvrti = vr & 0x000000ff;
						sekcija.push_back(cetvrti);
						sekcija.push_back(treci);
						sekcija.push_back(drugi);
						sekcija.push_back(prvi);
						
						lc+=8;
					}
						
					break;	
				} //kraj STORE
				
				
				//PUSH i POP
				if((brojTokena == 22) || (brojTokena == 23))
				{
					int instrukcija;
					if(brojTokena == 22) { instrukcija = PUSH; }
					if(brojTokena == 23) { instrukcija = POP; }
					
					lc+=4;
					
					i++;
					rec = tokeni[i];
					r0 = vratiRegistar(vratiToken(rec));
					if(r0 == -1) { greska = 1; poruka = "Registar ne postoji(PUSH/POP)!"; break; }
					r0 = r0 << 16;
					
					instr = sloziInstrukciju(instrukcija, REG_DIR, r0, 0, 0, 0);
					
					int upis[4];
					upis[0] = instr >> 24;
					upis[1] = (instr & 0x00ff0000) >> 16;
					upis[2] = (instr & 0x0000ff00) >> 8;
					upis[3] = instr & 0x000000ff;
					for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
					 
					break;
				} //kraj PUSH i POP
				
				
				//Aritmeticke instrukcije
				if((brojTokena > 23) && (brojTokena < 34))
				{
					int instrukcija;
					if(brojTokena == 24) { instrukcija = ADD; }
					if(brojTokena == 25) { instrukcija = SUB; }
					if(brojTokena == 26) { instrukcija = MUL; }
					if(brojTokena == 27) { instrukcija = DIV; }
					if(brojTokena == 28) { instrukcija = MOD; }
					if(brojTokena == 29) { instrukcija = AND; }
					if(brojTokena == 30) { instrukcija = OR; }
					if(brojTokena == 31) { instrukcija = XOR; }
					if(brojTokena == 32) { instrukcija = ASL; }
					if(brojTokena == 33) { instrukcija = ASR; }
					
					lc+=4;
					
					i++;
					rec = tokeni[i];
					r0 = vratiRegistar(vratiToken(rec));
					if(r0 == -1) { greska = 1; poruka = "Registar ne postoji(aritmeticke)!"; break; }
					r0 = r0 << 16;
					
					i++;
					rec = tokeni[i];
					r1 = vratiRegistar(vratiToken(rec));
					if(r1 == -1) { greska = 1; poruka = "Registar ne postoji(aritmeticke)!"; break; }
					r1 = r1 << 11;
					
					i++;
					rec = tokeni[i];
					r2 = vratiRegistar(vratiToken(rec));
					if(r2 == -1) { greska = 1; poruka = "Registar ne postoji(aritmeticke)!"; break; }
					r2 = r2 << 6;
					
					instr = sloziInstrukciju(instrukcija, REG_DIR, r0, r1, r2, 0);
					
					int upis[4];
					upis[0] = instr >> 24;
					upis[1] = (instr & 0x00ff0000) >> 16;
					upis[2] = (instr & 0x0000ff00) >> 8;
					upis[3] = instr & 0x000000ff;
					for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
					 
					break;
				} //kraj aritmeticke instrukcije
				
				
				//NOT
				if(brojTokena == 34)
				{
					int instrukcija = NOT;
					lc+=4;
					
					i++;
					rec = tokeni[i];
					r0 = vratiRegistar(vratiToken(rec));
					if(r0 == -1) { greska = 1; poruka = "Registar ne postoji(NOT)!"; break; }
					r0 = r0 << 16;
					
					i++;
					rec = tokeni[i];
					r1 = vratiRegistar(vratiToken(rec));
					if(r1 == -1) { greska = 1; poruka = "Registar ne postoji(NOT)!"; break; }
					r1 = r1 << 11;
					
					instr = sloziInstrukciju(instrukcija, REG_DIR, r0, r1, 0, 0);
					
					int upis[4];
					upis[0] = instr >> 24;
					upis[1] = (instr & 0x00ff0000) >> 16;
					upis[2] = (instr & 0x0000ff00) >> 8;
					upis[3] = instr & 0x000000ff;
					for(int cnt=0; cnt<4; cnt++) { sekcija.push_back(upis[cnt]); }
					 
					break;
				} //kraj NOT
				
			} //if za instrukcije
			
    	} //for za jednu liniju
    	
    } //while za fajl
    
 	if(greska == 1) 
	{
		izlaz << poruka << endl;
	 	return; 
	}   	
} //kraj drugog prolaza

