//#include <string.h>
//#include <stdio.h>
//using namespace std;

#include <vector>
#include <string>

const int INT = 0x00000000;
const int JMP = 0x02000000;
const int CALL = 0x03000000;
const int RET = 0x01000000;
const int JZ = 0x04000000;
const int JNZ = 0x05000000;
const int JGZ = 0x06000000;
const int JGEZ = 0x07000000;
const int JLZ = 0x08000000;
const int JLEZ = 0x09000000;
const int LOAD = 0x10000000;
const int STORE = 0x11000000;
const int PUSH = 0x20000000;
const int POP = 0x21000000;
const int ADD = 0x30000000;
const int SUB = 0x31000000;
const int MUL = 0x32000000;
const int DIV = 0x33000000;
const int MOD = 0x34000000;
const int AND = 0x35000000;
const int OR = 0x36000000;
const int XOR = 0x37000000;
const int NOT = 0x38000000;
const int ASL = 0x39000000;
const int ASR = 0x3A000000;

const int IMM = 0x00800000;
const int REG_DIR = 0x00000000;
const int MEM_DIR = 0x00C00000;
const int REG_IND = 0x00400000;
const int REG_IND_POM = 0x00E00000;

const int DOUBLE = 0x00000000;
const int UWORD = 0x00000008;
const int SWORD = 0x00000028;
const int UB = 0x00000018;
const int SB = 0x00000038;



int vratiToken(std::string);

std::vector<std::string> obradiLiniju(std::string);

int vratiAdresiranje(std::string);

std::vector<std::string> obradiIzraz(std::string);


int ipr(std::string);
int spr(std::string);
int isOperand(std::string);

std::vector<std::string> infix2postfix(std::vector<std::string>);

int vratiVrednostDec(std::string);
int vratiVrednostHex(std::string);
int vratiVrednostBin(std::string);
int vratiVrednostChar(std::string);

int izracunaj(std::string, int, int);

std::vector<std::string> obradiIzrazSaZarezima(std::string);

int sloziInstrukciju(int, int, int, int, int, int);
int vratiRegistar(int);

int proveriBSS(std::vector<int>);
