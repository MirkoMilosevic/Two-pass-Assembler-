#include "Mapiranje.h"
#include <stdio.h>
#include <ctype.h>
#include <sstream>
#include <iostream>

using namespace std;

vector<string> obradiLiniju(string s)
{
	vector<string> tokeni;
	
	for(int i = 0; i < s.length(); i++)
	{
		string temp = "";
		
		if((s[i] == ' ') || (s[i] == '	')) { continue; }
		
		if(s[i] == '[')
		{
			while((s[i] != ']') && (i<s.length()))
			{
				if(s[i] != ' ') { temp += s[i]; }
				i++;
			}
			temp += s[i];
			tokeni.push_back(temp);
			continue;
		}
		
		if(s[i] == ';')
		{
			while(i<s.length())
			{
				temp += s[i];
				i++;
			}
			tokeni.push_back(temp);
			break;
		}
		
		while((s[i] != ' ') && (s[i] != ',') && (s[i] != '	') && (i<s.length()))
		{
			if(s[i] == ';') { tokeni.push_back(temp); return tokeni; }
			temp += s[i];
			i++;
		}
		
		tokeni.push_back(temp);
	}
	return tokeni;
}


vector<string> obradiIzrazSaZarezima(string s)
{
	vector<string> tokeni;
	
	for(int i = 0; i < s.length(); i++)
	{
		string temp = "";
		
		if((s[i] == ' ') || (s[i] == '	')) { continue; }
		
		if(s[i] == ';') { break; }
		
		while((s[i] != ',') && (i<s.length()) && (s[i] != ';'))
		{
			if((s[i] != ' ') && (s[i] != '	'))
			{
				temp += s[i];
			}
			i++;
		}
		
		tokeni.push_back(temp);
	}
	return tokeni;
}


int vratiToken(string s)
{	
	
	//INT
	if((s == "int")||(s == "INT")) return 2;
	
	//JMP
	if((s == "jmp")||(s == "JMP")) return 3;
	
	//CALL
	if((s == "call")||(s == "CALL")) return 4;
	
	//RET
	if((s == "ret")||(s == "RET")) return 5;
	
	//JZ
	if((s == "jz")||(s == "JZ")) return 6;
	
	//JNZ
	if((s == "jnz")||(s == "JNZ")) return 7;
	
	//JGZ
	if((s == "jgz")||(s == "JGZ")) return 8;
	
	//JGEZ
	if((s == "jgez")||(s == "JGEZ")) return 9;
	
	//JLZ
	if((s == "jlz")||(s == "JLZ")) return 10;
	
	//JLEZ
	if((s == "jlez")||(s == "JLEZ")) return 11;
	
	//LOAD
	if((s == "load")||(s == "LOAD")) return 12;
	
	//LOADUB
	if((s == "loadub")||(s == "LOADUB")) return 13;
	
	//LOADSB
	if((s == "loadsb")||(s == "LOADSB")) return 14;
	
	//LOADUW
	if((s == "loaduw")||(s == "LOADUW")) return 15;
	
	//LOADSW
	if((s == "loadsw")||(s == "LOADSW")) return 16;
	
	//STORE
	if((s == "store")||(s == "STORE")) return 17;
	
	//STOREW
	if((s == "storew")||(s == "STOREW")) return 18;
	
	//STOREB
	if((s == "storeb")||(s == "STOREB")) return 19;
	
	/*
	//STOREUW
	if((s == "storeuw")||(s == "STOREUW")) return 20;
	
	//STORESW
	if((s == "storesw")||(s == "STORESW")) return 21;
	*/
	
	//PUSH
	if((s == "push")||(s == "PUSH")) return 22;
	
	//POP
	if((s == "pop")||(s == "POP")) return 23;
	
	//ADD
	if((s == "add")||(s == "ADD")) return 24;
	
	//SUB
	if((s == "sub")||(s == "SUB")) return 25;
	
	//MUL
	if((s == "mul")||(s == "MUL")) return 26;
	
	//DIV
	if((s == "div")||(s == "DIV")) return 27;
	
	//MOD
	if((s == "mod")||(s == "MOD")) return 28;
	
	//AND
	if((s == "and")||(s == "AND")) return 29;
	
	//OR
	if((s == "or")||(s == "OR")) return 30;
	
	//XOR
	if((s == "xor")||(s == "XOR")) return 31;
	
	//ASL
	if((s == "asl")||(s == "ASL")) return 32;
	
	//ASR
	if((s == "asr")||(s == "ASR")) return 33;
	
	//NOT
	if((s == "not")||(s == "NOT")) return 34;
	
	
	//DB
	if((s == "db")||(s == "DB")) return 127;
	
	//DW
	if((s == "dw")||(s == "DW")) return 128;
	
	//DD
	if((s == "dd")||(s == "DD")) return 129;
	
	//DUP
	if((s == "dup")||(s == "DUP")) return 130;
	
	
	//DEF
	if((s == "def")||(s == "DEF")) return 131;
	
	//ORG
	if((s == "org")||(s == "ORG")) return 132;
	
	
	//TEXT_SEKCIJA
	if(((s[0] == '.') && (s[1] == 't') && (s[2] == 'e') && (s[3] == 'x') && (s[4] == 't'))
	   ||
	  ((s[0] == '.') && (s[1] == 'T') && (s[2] == 'E') && (s[3] == 'X') && (s[4] == 'T'))) 
	  return 133;
	
	//DATA_SEKCIJA
	if(((s[0] == '.') && (s[1] == 'd') && (s[2] == 'a') && (s[3] == 't') && (s[4] == 'a'))
	   ||
	  ((s[0] == '.') && (s[1] == 'D') && (s[2] == 'A') && (s[3] == 'T') && (s[4] == 'A'))) 
	  return 134;
	  
	//RODATA_SEKCIJA
	if(((s[0] == '.') && (s[1] == 'r') && (s[2] == 'o') && (s[3] == 'd') && (s[4] == 'a') && (s[5] == 't') && (s[6] == 'a'))
	   ||
	  ((s[0] == '.') && (s[1] == 'R') && (s[2] == 'O') && (s[3] == 'D') && (s[4] == 'A') && (s[5] == 'T') && (s[6] == 'A')))
	  return 135;
	  
	//BSS_SEKCIJA
	if(((s[0] == '.') && (s[1] == 'b') && (s[2] == 's') && (s[3] == 's'))
	   ||
	  ((s[0] == '.') && (s[1] == 'B') && (s[2] == 'S') && (s[3] == 'S'))) 
	  return 136;
	  
	  
	//PC
	if((s == "pc")||(s == "PC")) return 237;
	
	//SP
	if((s == "sp")||(s == "SP")) return 238;
	
	//R0
	if((s == "r0")||(s == "R0")) return 240;
	
	//R1
	if((s == "r1")||(s == "R1")) return 241;
	
	//R2
	if((s == "r2")||(s == "R2")) return 242;
	
	//R3
	if((s == "r3")||(s == "R3")) return 243;
	
	//R4
	if((s == "r4")||(s == "R4")) return 244;
	
	//R5
	if((s == "r5")||(s == "R5")) return 245;
	
	//R6
	if((s == "r6")||(s == "R6")) return 246;
	
	//R7
	if((s == "r7")||(s == "R7")) return 247;
	
	//R8
	if((s == "r8")||(s == "R8")) return 248;
	
	//R9
	if((s == "r9")||(s == "R9")) return 249;
	
	//R10
	if((s == "r10")||(s == "R10")) return 250;
	
	//R11
	if((s == "r11")||(s == "R11")) return 251;
	
	//R12
	if((s == "r12")||(s == "R12")) return 252;
	
	//R13
	if((s == "r13")||(s == "R13")) return 253;
	
	//R14
	if((s == "r14")||(s == "R14")) return 254;
	
	//R15
	if((s == "r15")||(s == "R15")) return 255;
	
	
	//GLOBAL
	if((s == ".global")||(s == ".GLOBAL")) return 156;
	
	//END
	if((s == ".end")||(s == ".END")) return 157;
	
	
	//BYTE
	if((s == ".byte")||(s == ".BYTE")) return 158;
	
	//WORD
	if((s == ".word")||(s == ".WORD")) return 159;
	
	//LONG
	if((s == ".long")||(s == ".LONG")) return 160;
	
	
	//LABELA ILI NEKI SIMBOL
	if (s[s.length() - 1] == ':') return 1;
	else if(isalpha(s[0])) return 0;
	else return -1;
	 
}


int vratiAdresiranje(string s)
{
	int temp;

	if(s[0] == '#') return 1; //IMM
	
	if((s[0] == '$') && (s.length()>1)) 
	{
		if((isdigit(s[1])) || (isalpha(s[1]))) { return 5; } //REG_IND_POM
		else return 3; //MEM_DIR
	}
	else if(s[0] == '$') { return 3; } //MEM_DIR 
	
	if(s[0] == '[')
	{
		string pom = "";
		int i = 1;
		while(s[i] != ']')
		{
			pom += s[i];
			i++;
		}
		temp = vratiToken(pom);
		if((temp > 236) && (temp < 256)) return 4; //REG_IND
		else return 5; //REG_IND_POM
	}
	
	temp = vratiToken(s);
	
	if((temp > 236) && (temp < 256)) return 2; //REG_DIR
	
	if((temp == 0) || ((temp > 132) && (temp < 137)) || isdigit(s[0]) || (s[0] == '-') || (s[0] == '+') || (s[0] == '(')) return 3; //MEM_DIR

	return 0;		
}


vector<string> obradiIzraz(string s)
{
	vector<string> tokeni;
	
	for(int i = 0; i < s.length(); i++)
	{
		string temp = "";
		
		if((i == 0) && ((s[i] == '+') || (s[i] == '-')))
		{
			string str = "0";
			temp+=s[i];
			tokeni.push_back(str);
			tokeni.push_back(temp);
			continue;
		}
		
		if((s[i] == ' ') || (s[i] == '	')) { continue; }
		
		if(s[i] == '+') 
		{ 
			if(tokeni.back() == "(") { string str = "0"; tokeni.push_back(str); }
			temp = ""; 
			temp += s[i]; 
			tokeni.push_back(temp); 
		}
		
		if(s[i] == '-') 
		{ 
			if(tokeni.back() == "(") { string str = "0"; tokeni.push_back(str); }
			temp = ""; 
			temp += s[i]; 
			tokeni.push_back(temp); 
		}
		
		if(s[i] == '*') { temp = ""; temp += s[i]; tokeni.push_back(temp); }
		
		if(s[i] == '/') { temp = ""; temp += s[i]; tokeni.push_back(temp); }
		
		if(s[i] == '(') { temp = ""; temp += s[i]; tokeni.push_back(temp); }
		
		if(s[i] == ')') { temp = ""; temp += s[i]; tokeni.push_back(temp); }
		
		if((s[i] == '0') && (s[i+1] == 'x'))
		{
			temp += s[i];
			i++;
			temp += s[i];
			i++;
			while((s[i] != ' ') && (s[i] != '+') && (s[i] != '-') && (s[i] != '*') && (s[i] != '/') && (s[i] != ')') && (s[i] != '(') && (i<s.length()))
			{
				temp += s[i];
				i++;
			}
			
			tokeni.push_back(temp);
			if((i<s.length()) && (s[i] != ' ')) { temp = ""; temp += s[i]; tokeni.push_back(temp); }
			continue;
		}
		
		if((s[i] == '0') && (s[i+1] == 'b'))
		{
			temp += s[i];
			i++;
			temp += s[i];
			i++;
			while((isdigit(s[i])) && (i<s.length()))
			{
				temp += s[i];
				i++;
			}
			
			tokeni.push_back(temp);
			if((i<s.length()) && (s[i] != ' ')) { temp = ""; temp += s[i]; tokeni.push_back(temp); }
			continue;
		}
		
		if((int) s[i] == 39)
		{
			temp += s[i];
			i++;
			temp += s[i];
			i++;
			temp += s[i];
			tokeni.push_back(temp);
			continue;
		}
		
		if(s[i] == '$')
		{
			while((s[i] != ' ') && (s[i] != '	') && (s[i] != '+') && (s[i] != '-') && (s[i] != '*') && (s[i] != '/') && (s[i] != ')') && (s[i] != '(') && (i<s.length()))
			{
				temp += s[i];
				i++;
			}
			
			tokeni.push_back(temp);
			if((i<s.length()) && (s[i] != ' ') && (s[i] != '	')) { temp = ""; temp += s[i]; tokeni.push_back(temp); }
			continue;
		}
		
		if(isalpha(s[i]) || (s[i] == '.'))
		{
			while((s[i] != ' ') && (s[i] != '	') && (s[i] != '+') && (s[i] != '-') && (s[i] != '*') && (s[i] != '/') && (s[i] != ')') && (s[i] != '(') && (i<s.length()))
			{
				temp += s[i];
				i++;
			}
			
			tokeni.push_back(temp);
			if((i<s.length()) && (s[i] != ' ') && (s[i] != '	')) { temp = ""; temp += s[i]; tokeni.push_back(temp); }
			continue;
		}
		
		if(isdigit(s[i]))
		{
			while((s[i] != ' ') && (s[i] != '	') && (s[i] != '+') && (s[i] != '-') && (s[i] != '*') && (s[i] != '/') && (s[i] != ')') && (s[i] != '(') && (i<s.length()))
			{
				temp += s[i];
				i++;
			}
			
			tokeni.push_back(temp);
			if((i<s.length()) && (s[i] != ' ') && (s[i] != '	')) { temp = ""; temp += s[i]; tokeni.push_back(temp); }
			continue;
		}
		
	}
	return tokeni;	
}

int ipr(string s)
{
	if(s == "+") { return 2; }
	if(s == "-") { return 3; }
	if(s == "*") { return 4; }
	if(s == "/") { return 4; }
	if(s == "(") { return 5; }
	if(s == ")") { return 1; }
}

int spr(string s)
{
	if(s == "+") { return 2; }
	if(s == "-") { return 3; }
	if(s == "*") { return 4; }
	if(s == "/") { return 4; }
	if(s == "(") { return 0; }
}

int isOperand(string s)
{
	if((s != "+") && (s != "-") && (s != "*") && (s != "/") && (s != "(") && (s != ")")) { return 1; }
	else { return 0; }
}

vector<string> infix2postfix(vector<string> input)
{
	vector<string> output;
	int sp = 0;
	string stek[64];
	
	int i = 0;
	string next = input[i];
	
	while(i<input.size())
	{
		if(isOperand(next)) { output.push_back(next); }
		else
		{
			while((sp != 0) && (ipr(next) <= spr(stek[sp])))
			{
				string x = stek[sp];
				sp--;
				output.push_back(x);
			}
			
			if(next != ")") { sp++; stek[sp] = next; }
			else { sp--; }
		}
		
		i++;
		if(i<input.size()) { next = input[i]; }
	}
	
	while( sp != 0)
	{
		string x = stek[sp];
		sp--;
		output.push_back(x);
	}
	
	return output;
}


int vratiVrednostDec(string s)
{
	int x;   
	stringstream ss;
	ss << std::dec << s;
	ss >> x;
	return x;
}

int vratiVrednostHex(string s)
{
	int x;
	stringstream ss;
	
	//za slucaj da je najvisi bit jedinica
	if(s.length() == 10)
	{
		if((s[2] == '8') || (s[2] == '9') || (s[2] == 'a') || (s[2] == 'b') || (s[2] == 'c') || (s[2] == 'd') || (s[2] == 'e') || (s[2] == 'f') ||
		   (s[2] == 'A') || (s[2] == 'B') || (s[2] == 'C') || (s[2] == 'D') || (s[2] == 'E') || (s[2] == 'F'))
		{	
			string replaceS = "0x";
			for(int i=2; i<10; i++)
			{
				stringstream m;
				int pom = 0;
				string temp = "";
				temp += s[i];
				
				pom = vratiVrednostHex(temp);
				
				pom = ~pom;
				pom = pom & 0xf;
				
				m << std::hex << pom;
				replaceS += m.str();
			}
			
			x = vratiVrednostHex(replaceS);
			x = -x-1;
			return x;
		}
	}
	
	ss << std::hex << s;
	ss >> x;
	return x;
}

int vratiVrednostBin(string s)
{
	int x = 0;
	int n = 1;   
	int i = s.length()-1;
	while((s[i] != 'b') && (i>1))
	{
		if(s[i] == '1') { x += n; }
		n *= 2;
		i--;
	}
	return x;
}

int vratiVrednostChar(string s)
{
	int x = (int) s[1];
	return x;
}

int izracunaj(string s, int x, int y)
{
	if(s == "+") { int n = x + y; return n; }
	if(s == "-") { int n = x - y; return n; }
	if(s == "*") { int n = x * y; return n; }
	if(s == "/") { int n = x / y; return n; }
}


int sloziInstrukciju(int oc, int adrm, int reg0, int reg1, int reg2, int t)
{
	int x = oc | adrm | reg0 | reg1 | reg2 | t;
	return x;
}

int vratiRegistar(int reg)
{
	switch(reg)
	{
		case 237: return 0x11;
		case 238: return 0x10;
		case 240: return 0x00;
		case 241: return 0x01;
		case 242: return 0x02;
		case 243: return 0x03;
		case 244: return 0x04;
		case 245: return 0x05;
		case 246: return 0x06;
		case 247: return 0x07;
		case 248: return 0x08;
		case 249: return 0x09;
		case 250: return 0x0A;
		case 251: return 0x0B;
		case 252: return 0x0C;
		case 253: return 0x0D;
		case 254: return 0x0E;
		case 255: return 0x0F;
		default: return -1;
	}
}

int proveriBSS(vector<int> bssSekcija)
{
	int provera = 1;
	for(int i=0; i<bssSekcija.size(); i++)
	{
		if(bssSekcija[i] != 0) { provera = -1; }
	}
	return provera;
}
