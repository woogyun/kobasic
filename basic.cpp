#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <cmath>
#include <stack>
#include <tuple>
#include <limits>
#include <windows.h>


#define MAXLEN 100
#define NUM_FUNCTIONS 32

#define NUMBER 130
#define REM 131
#define QUOTEREM 132
#define BYE 133
#define CALL 134
#define CHANGE 135
#define CLS 136
#define CMD 137
#define DATA 138
#define DEF 139
#define END 140
#define EXIT 141
#define FOR 142
#define TO 143
#define GOSUB 144
#define OF 145
#define INPUT 146
#define THEN 147
#define IF 148
#define GOTO 149
#define LET 150
#define NEXT 151
#define NEW 152
#define ON 153
#define OPTION 154
#define BASE 155
#define POKE 156
#define POP 157
#define PRINT 158
#define USING 159
#define RANDOMIZE 160
#define READ 161
#define RESTORE 162
#define	RETURN 163
#define STOP 164
#define STRING 165
#define SYS 166
#define TIME_STR 167
#define VARLIST 168
#define WAIT 169
#define AND 170
#define OR 171
#define CMP_LE 172
#define CMP_GE 173
#define CMP_NE 174
#define CMP_HASH 175
#define NOT 176
#define FRE 177
#define RND 178
#define TIME 179
#define ABS 180
#define ATN 181
#define CHR 182
#define CLOG 183
#define COS 184
#define EXP 185
#define FIX 186
#define INT 187
#define LCASE 188
#define LEN 189
#define LIN 190
#define STR 191
#define LOG 192
#define PEEK 193
#define SGN 194
#define LEFT 195
#define RIGHT 196
#define STRNG 197
#define MID 198
#define SEG 199
#define SUBSTR 200
#define VARIABLE_NAME 201
#define FUNCTION_NAME 202
#define DOUBLE 203
#define SINGLE 204
#define INTEGER 205
#define BANGREM 206
#define CLEAR 207
#define CLR 208
#define DIM 209
#define GET 210
#define LIST 211
#define STEP 212
#define OPEN 213
#define CLOSE 214
#define STATUS 215
#define PRINT_FILE 216
#define INPUT_FILE 217
#define GET_FILE 218
#define PUT_FILE 219
#define RUN 220
#define SIN 221
#define TAN 222
#define UCASE 223
#define SQR 224
#define CONVERT 225
#define FRAC 226
#define CINT 227
#define CSNG 228
#define CDBL 229
#define ASC 230
#define DEFDBL 231
#define VAL 232
#define XOR 233
#define SPC 234
#define TAB 235
#define POS 236
#define USR 237
#define DEFSTR 238
#define DEFINT 239
#define DEFSNG 240
#define FUN 241
#define UMINUS 242
#define VAR 243
#define CLO 244
#define CMP_EQ 245
#define ARR 246
#define CONT 247

using namespace std;

struct _node;

const string functionNames[NUM_FUNCTIONS] = {
    "FRE", "RND", "TIME", "TIME_STR", "ABS", "ATN", "CHR", "CLOG", "COS", "EXP", "FIX", "INT",
    "LCASE", "LEN", "LIN", "STR", "LOG", "PEEK", "SGN", "SIN", "SPC", "SQR", "TAB", "VAL",
    "UCASE", "USR", "LEFT", "RIGHT", "STRNG", "MID", "SEG", "SUBSTR"
};

enum class ExceptionCode {
    NextWithoutFor,
    SyntaxError,
    typeMisMatch,
    NOT_DECLARED
};

typedef struct _AttrVal{
	int tag;
	union {
		int ival;
		char* sval;
		struct _node* pval;
	};
	vector<_AttrVal> arr;
} AttrVal;

typedef struct _node {
	int nID;
	AttrVal val;
	struct _node *son, *bro; 
} Node;

map<string, AttrVal> tab;
int getToken();
void printToken(int);
FILE* fp;
void match(int);
void Prog();
Node* Line();
Node* Stmts();
Node* Stmt();
Node* Expr();
Node* Expr0();
Node* Expr1();
Node* Expr2();
Node* Expr3();
Node* Expr4();
Node* Func();
Node* Factor();
Node* UserFunc();
Node* Var();
Node* PrintList();
void PrintSep();
Node* ExprList();
void SliceList();
Node* VarList();
void NumList();
int lookahead;
AttrVal attrVal;
Node* newNode(int);
void printTree(Node*, int);
void append(Node*, Node*);
Node* traverse(Node*);
void printEnv();
map<string, AttrVal> funList;
AttrVal mkInt(int);
AttrVal mkStr(char*);
AttrVal mkClo(Node*);
AttrVal mkVar(char*);
AttrVal mkFun(char*);
bool isFN(char*);
string getFunctionName(int);
void setFunList();
void printFunList();
map<string, AttrVal> merge(map<string, AttrVal>, map<string, AttrVal>);
void addToEnv(Node*);
Node* exec(Node*); 
AttrVal getValueOf(Node*);
bool isComparisonOperator(char);
vector<Node*> dataQueue;
int dataQueueIndex;
stack<tuple<long int, int, int>> forStack;
stack<long int> gosubStack;
map<int, Node*> linedStmt;
map<string, Node*> forTab;
stack<Node*> callStack;
int getIndex(Node*);
bool isAllDigits(const string&);
bool continueExecution = true;
bool isBuiltinFunction(string);
AttrVal getBuiltinFunctionResult(string fname, Node* arg);
bool ishangul(char);
const char* toUtf8(const char*);
pair<Node*, Node*> stopPos;

int main(void) {

	
	
	
	fp = fopen("fin.txt", "r");
	if (!fp) fp = stdin;
	cout << "" << endl;
	
	lookahead = getToken();
	Prog();
	return 0;
}

int getToken() {
	char c, buffer[MAXLEN];
	if (lookahead == REM) {
		while (c = fgetc(fp)) {
			if (c == '\n' || c == EOF) {
				break;
			}
		}
	}
	int i = 0;
	while (c = fgetc(fp)) {
		
		if (c == '\n') return '\n';
		if (isspace(c)) continue;
		
		if (isComparisonOperator(c)) {
			buffer[i++] = c;
			while (c = fgetc(fp)) {
				if (isComparisonOperator(c)) {
					buffer[i++] = c;
				}
				else {
					ungetc(c, fp);
					buffer[i] = '\0';
					break;
				}
			}
			if (i == 1) {
				return buffer[0];
			}
			else if (i == 2) {
				if (!strcmp(buffer, ">=")) return CMP_GE;
				else if (!strcmp(buffer, "<=")) return CMP_LE;
				else if (!strcmp(buffer, "<>")) return CMP_NE;
			}
			else return -1;
		}
		
		if (isdigit(c)) {
			buffer[i++] = c;
			while (c = fgetc(fp)) {
				if (isdigit(c)) {
					buffer[i++] = c;
				}
				else {
					ungetc(c, fp);
					buffer[i] = '\0';
					break;
				}
			}
			attrVal = mkInt(atoi(buffer));
			return NUMBER;
		}
		
		if (c == '"') {
			while (c = fgetc(fp)) {
				if (c != '"') {
					buffer[i++] = c;					
				}
				else {
					buffer[i] = '\0';
					break;
				}
			}
			attrVal = mkStr(buffer);
			return STRING;
		}
		
		if (c == '.' || c == '&' || c == '|') {
			buffer[i++] = c;
			while (c = fgetc(fp)) {
				if (c == '.' || c == '&' || c == '|') {
					buffer[i++] = c;
				}
				else {
					ungetc(c, fp);
					buffer[i] = '\0';
					if (!strcmp(buffer, "..")) return TO;
					else if (!strcmp(buffer, "&&")) return AND;
					else if (!strcmp(buffer, "||")) return OR;
					return -1;
				}
			}
		}

		if (isalpha(c) || c == '$' || ishangul(c)) {
			buffer[i++] = c;
			while (c = fgetc(fp)) {
				if (isalpha(c) || c == '_' || isdigit(c) || ishangul(c)) {
					buffer[i++] = c;
				}
				else {
					ungetc(c, fp);
					buffer[i] = '\0';
					for(char* c = buffer; *c = toupper(*c); c++);
					if (!strcmp(buffer, "REM") || !strcmp(buffer, toUtf8("주석"))) return REM;
					else if (!strcmp(buffer, "DEF") || !strcmp(buffer, toUtf8("함수"))) return DEF;
					else if (!strcmp(buffer, "QUOTEREM")) return QUOTEREM;
					else if (!strcmp(buffer, "BYE")) return BYE;
					else if (!strcmp(buffer, "CONT") || !strcmp(buffer, toUtf8("재개"))) return CONT;
					else if (!strcmp(buffer, "CMD")) return CMD;
					else if (!strcmp(buffer, "CALL")) return CALL;
					else if (!strcmp(buffer, "CHANGE")) return CHANGE;
					else if (!strcmp(buffer, "CLS")) return CLS;
					else if (!strcmp(buffer, "DATA")|| !strcmp(buffer, toUtf8("자료"))) return DATA;
					else if (!strcmp(buffer, "END")|| !strcmp(buffer, toUtf8("끝"))) return END;
					else if (!strcmp(buffer, "EXIT")) return EXIT;
					else if (!strcmp(buffer, "FOR") || !strcmp(buffer, toUtf8("반복"))) return FOR;
					else if (!strcmp(buffer, "TO") || !strcmp(buffer, "..")) return TO;
					else if (!strcmp(buffer, "GOSUB") || !strcmp(buffer, toUtf8("순회"))) return GOSUB;
					else if (!strcmp(buffer, "OF")) return OF;
					else if (!strcmp(buffer, "INPUT") || !strcmp(buffer, toUtf8("입력"))) return INPUT;
					else if (!strcmp(buffer, "THEN") || !strcmp(buffer, toUtf8("참이면"))) return THEN;
					else if (!strcmp(buffer, "IF") || !strcmp(buffer, toUtf8("조건"))) return IF;
					else if (!strcmp(buffer, "GOTO") || !strcmp(buffer, toUtf8("분기"))) return GOTO;
					else if (!strcmp(buffer, "LET") || !strcmp(buffer, toUtf8("변수"))) return LET;
					else if (!strcmp(buffer, "NEXT") || !strcmp(buffer, toUtf8("다음"))) return NEXT;
					else if (!strcmp(buffer, "NEW")|| !strcmp(buffer, toUtf8("초기화"))) return NEW;
					else if (!strcmp(buffer, "ON")|| !strcmp(buffer, toUtf8("검사"))) return ON;
					else if (!strcmp(buffer, "OPTION")) return OPTION;
					else if (!strcmp(buffer, "BASE")) return BASE;
					else if (!strcmp(buffer, "POKE")) return POKE;
					else if (!strcmp(buffer, "POP")) return POP;
					else if (!strcmp(buffer, "PRINT") || !strcmp(buffer, toUtf8("출력"))) return PRINT;
					else if (!strcmp(buffer, "USING")) return USING;
					else if (!strcmp(buffer, "RANDOMIZE")) return RANDOMIZE;
					else if (!strcmp(buffer, "READ")|| !strcmp(buffer, toUtf8("읽기"))) return READ;
					else if (!strcmp(buffer, "RESTORE")|| !strcmp(buffer, toUtf8("첫자료"))) return RESTORE;
					else if (!strcmp(buffer, "RETURN")|| !strcmp(buffer, toUtf8("복귀"))) return RETURN;
					else if (!strcmp(buffer, "STOP")|| !strcmp(buffer, toUtf8("중지"))) return STOP;
					else if (!strcmp(buffer, "STRING")) return STRING;
					else if (!strcmp(buffer, "SYS")) return SYS;
					else if (!strcmp(buffer, "TIME_STR")) return TIME_STR;
					else if (!strcmp(buffer, "VARLIST")) return VARLIST;
					else if (!strcmp(buffer, "WAIT")) return WAIT;
					else if (!strcmp(buffer, "AND")) return AND;
					else if (!strcmp(buffer, "OR")) return OR;
					else if (!strcmp(buffer, "CMP_HASH")) return CMP_HASH;
					else if (!strcmp(buffer, "NOT")) return NOT;
					else if (!strcmp(buffer, "FRE")) return FRE;
					else if (!strcmp(buffer, "RND")) return RND;
					else if (!strcmp(buffer, "TIME")) return TIME;
					else if (!strcmp(buffer, "ABS")) return ABS;
					else if (!strcmp(buffer, "ATN")) return ATN;
					else if (!strcmp(buffer, "CHR")) return CHR;
					else if (!strcmp(buffer, "CLOG")) return CLOG;
					else if (!strcmp(buffer, "COS")) return COS;
					else if (!strcmp(buffer, "EXP")) return EXP;
					else if (!strcmp(buffer, "FIX")) return FIX;
					else if (!strcmp(buffer, "INT")) return INT;
					else if (!strcmp(buffer, "LCASE")) return LCASE;
					else if (!strcmp(buffer, "LEN")) return LEN;
					else if (!strcmp(buffer, "LIN")) return LIN;
					else if (!strcmp(buffer, "STR")) return STR;
					else if (!strcmp(buffer, "LOG")) return LOG;
					else if (!strcmp(buffer, "PEEK")) return PEEK;
					else if (!strcmp(buffer, "SGN")) return SGN;
					else if (!strcmp(buffer, "LEFT")) return LEFT;
					else if (!strcmp(buffer, "RIGHT")) return RIGHT;
					else if (!strcmp(buffer, "STRNG")) return STRNG;
					else if (!strcmp(buffer, "MID")|| !strcmp(buffer, toUtf8("중간"))) return MID;
					else if (!strcmp(buffer, "SEG")) return SEG;
					else if (!strcmp(buffer, "SUBSTR")) return SUBSTR;
					else if (!strcmp(buffer, "DOUBLE")) return DOUBLE;
					else if (!strcmp(buffer, "SINGLE")) return SINGLE;
					else if (!strcmp(buffer, "INTEGER")) return INTEGER;
					else if (!strcmp(buffer, "BANGREM")) return BANGREM;
					else if (!strcmp(buffer, "CLEAR")) return CLEAR;
					else if (!strcmp(buffer, "CLR")) return CLR;
					else if (!strcmp(buffer, "DIM")|| !strcmp(buffer, toUtf8("배열"))) return DIM;
					else if (!strcmp(buffer, "GET")) return GET;
					else if (!strcmp(buffer, "LIST")) return LIST;
					else if (!strcmp(buffer, "STEP")|| !strcmp(buffer, toUtf8("폭"))) return STEP;
					else if (!strcmp(buffer, "OPEN")) return OPEN;
					else if (!strcmp(buffer, "CLOSE")) return CLOSE;
					else if (!strcmp(buffer, "STATUS")) return STATUS;
					else if (!strcmp(buffer, "PRINT_FILE")) return PRINT_FILE;
					else if (!strcmp(buffer, "INPUT_FILE")) return INPUT_FILE;
					else if (!strcmp(buffer, "GET_FILE")) return GET_FILE;
					else if (!strcmp(buffer, "PUT_FILE")) return PUT_FILE;
					else if (!strcmp(buffer, "RUN")) return RUN;
					else if (!strcmp(buffer, "SIN")) return SIN;
					else if (!strcmp(buffer, "TAN")) return TAN;
					else if (!strcmp(buffer, "SQR")) return SQR;
					else if (!strcmp(buffer, "INT")) return INT;
					else if (!strcmp(buffer, "FRAC")) return FRAC;	
					else if (!strcmp(buffer, "CINT")) return CINT;
					else if (!strcmp(buffer, "CSNG")) return CSNG;
					else if (!strcmp(buffer, "CDBL")) return CDBL;
					else if (!strcmp(buffer, "ASC")) return ASC;
					else if (!strcmp(buffer, "VAL")) return VAL;
					else if (!strcmp(buffer, "XOR")) return XOR;
					else if (!strcmp(buffer, "SPC")) return SPC;
					else if (!strcmp(buffer, "TAB")) return TAB;
					else if (!strcmp(buffer, "POS")) return POS;
					else if (!strcmp(buffer, "USR")) return USR;
					else if (!strcmp(buffer, "DEFSTR")) return DEFSTR;
					else if (!strcmp(buffer, "DEFINT")) return DEFINT;
					else if (!strcmp(buffer, "DEFSNG")) return DEFSNG;
					else if (!strcmp(buffer, "DEFDBL")) return DEFDBL;
					else if (!strcmp(buffer, "CONVERT")) return CONVERT;
					else if (!strcmp(buffer, "UCASE")) return UCASE;
					attrVal.sval = (char*)malloc(strlen(buffer) + 1);
					strcpy(attrVal.sval, buffer);
					if (isFN(buffer)) {
						return FUNCTION_NAME;
					}
					else {
						return VARIABLE_NAME;
					}
				}
			}
		}
		return c;
	}
	return -1;
}

void printToken(int token) {
	if (!token) {
		;
	}
	else if (token == -1) {
		printf("Not A Token");
	}
	else if (token == STRING) {
	    printf("STRING(%s)", attrVal.sval);
	}
	else if (token == CONT) {
		printf("CONT");
	}
	else if (token == NUMBER) {
		printf("NUM(%d)", attrVal.ival);
	} 
	else if (token == QUOTEREM) {
		printf("QUOTEREM");
	}
	else if (token == REM) {
		printf("REM");
	}
	else if (token == BYE) {
		printf("BYE");
	} 
	else if (token == CMD) {
		printf("CMD");
	}
	else if (token == CALL) {
		printf("CALL");
	}
	else if (token == CHANGE) {
		printf("CHANGE");
	}
	else if (token == CLS) {
		printf("CLS");
	}
	else if (token == DATA) {
		printf("DATA");
	}
	else if (token == END) {
		printf("END");
	}
	else if (token == DEF) {
		printf("DEF");
	}
	else if (token == EXIT) {
		printf("EXIT");
	}
	else if (token == CALL) {
		printf("CALL");
	}
	else if (token == FOR) {
		printf("FOR");
	}
	else if (token == TO) {
		printf("TO");
	}
	else if (token == GOSUB) {
		printf("GOSUB");
	}
	else if (token == OF) {
	    printf("OF");
	}
	else if (token == INPUT) {
	    printf("INPUT");
	}
	else if (token == THEN) {
	    printf("THEN");
	}
	else if (token == IF) {
	    printf("IF");
	}
	else if (token == GOTO) {
	    printf("GOTO");
	}
	else if (token == LET) {
	    printf("LET");
	}
	else if (token == NEXT) {
	    printf("NEXT");
	}
	else if (token == NEW) {
	    printf("NEW");
	}
	else if (token == ON) {
	    printf("ON");
	}
	else if (token == OPTION) {
	    printf("OPTION");
	}
	else if (token == BASE) {
	    printf("BASE");
	}
	else if (token == POKE) {
	    printf("POKE");
	}
	else if (token == POP) {
	    printf("POP");
	}
	else if (token == PRINT) {
	    printf("PRINT");
	}
	else if (token == USING) {
	    printf("USING");
	}
	else if (token == RANDOMIZE) {
	    printf("RANDOMIZE");
	}
	else if (token == READ) {
	    printf("READ");
	}
	else if (token == RESTORE) {
	    printf("RESTORE");
	}
	else if (token == RETURN) {
	    printf("RETURN");
	}
	else if (token == STOP) {
	    printf("STOP");
	}
	else if (token == SYS) {
	    printf("SYS");
	}
	else if (token == TIME_STR) {
	    printf("TIME_STR");
	}
	else if (token == VARLIST) {
	    printf("VARLIST");
	}
	else if (token == WAIT) {
	    printf("WAIT");
	}
	else if (token == AND) {
	    printf("AND");
	}
	else if (token == OR) {
	    printf("OR");
	}
	else if (token == CMP_LE) {
	    printf("CMP_LE");
	}
	else if (token == CMP_GE) {
	    printf("CMP_GE");
	}
	else if (token == CMP_NE) {
	    printf("CMP_NE");
	}
	else if (token == CMP_HASH) {
	    printf("CMP_HASH");
	}
	else if (token == NOT) {
	    printf("NOT");
	}
	else if (token == FRE) {
	    printf("FRE");
	}
	else if (token == RND) {
	    printf("RND");
	}
	else if (token == TIME) {
	    printf("TIME");
	}
	else if (token == ABS) {
	    printf("ABS");
	}
	else if (token == ATN) {
	    printf("ATN");
	}
	else if (token == CHR) {
	    printf("CHR");
	}
	else if (token == CLOG) {
	    printf("CLOG");
	}
	else if (token == COS) {
	    printf("COS");
	}
	else if (token == EXP) {
	    printf("EXP");
	}
	else if (token == FIX) {
	    printf("FIX");
	}
	else if (token == INT) {
	    printf("INT");
	}
	else if (token == LCASE) {
	    printf("LCASE");
	}
	else if (token == LEN) {
	    printf("LEN");
	}
	else if (token == LIN) {
	    printf("LIN");
	}
	else if (token == STR) {
	    printf("STR");
	}
	else if (token == LOG) {
	    printf("LOG");
	}
	else if (token == PEEK) {
	    printf("PEEK");
	}
	else if (token == SGN) {
	    printf("SGN");
	}
	else if (token == LEFT) {
	    printf("LEFT");
	}
	else if (token == RIGHT) {
	    printf("RIGHT");
	}
	else if (token == STRNG) {
	    printf("STRNG");
	}
	else if (token == MID) {
	    printf("MID");
	}
	else if (token == SEG) {
	    printf("SEG");
	}
	else if (token == SUBSTR) {
	    printf("SUBSTR");
	}
	else if (token == VARIABLE_NAME) {
	    printf("VARIABLE_NAME");
	}
	else if (token == DOUBLE) {
	    printf("DOUBLE");
	}
	else if (token == SINGLE) {
	    printf("SINGLE");
	}
	else if (token == INTEGER) {
	    printf("INTEGER");
	}
	else if (token == BANGREM) {
	    printf("BANGREM");
	}
	else if (token == CLEAR) {
	    printf("CLEAR");
	}
	else if (token == CLR) {
	    printf("CLR");
	}
	else if (token == DIM) {
	    printf("DIM");
	}
	else if (token == GET) {
	    printf("GET");
	}
	else if (token == LIST) {
	    printf("LIST");
	}
	else if (token == STEP) {
	    printf("STEP");
	}
	else if (token == OPEN) {
	    printf("OPEN");
	}
	else if (token == CLOSE) {
	    printf("CLOSE");
	}
	else if (token == STATUS) {
	    printf("STATUS");
	}
	else if (token == PRINT_FILE) {
	    printf("PRINT_FILE");
	}
	else if (token == INPUT_FILE) {
	    printf("INPUT_FILE");
	}
	else if (token == GET_FILE) {
	    printf("GET_FILE");
	}
	else if (token == PUT_FILE) {
	    printf("PUT_FILE");
	}
	else if (token == RUN) {
	    printf("RUN");
	}
	else if (token == SIN) {
	    printf("SIN");
	}
	else if (token == TAN) {
	    printf("TAN");
	}
	else if (token == SQR) {
	    printf("SQR");
	}
	else if (token == INT) {
	    printf("INT");
	}
	else if (token == FRAC) {
	    printf("FRAC");
	}
	else if (token == CINT) {
	    printf("CINT");
	}
	else if (token == CSNG) {
	    printf("CSNG");
	}
	else if (token == CDBL) {
	    printf("CDBL");
	}
	else if (token == ASC) {
	    printf("ASC");
	}
	else if (token == VAL) {
	    printf("VAL");
	}
	else if (token == XOR) {
	    printf("XOR");
	}
	else if (token == SPC) {
	    printf("SPC");
	}
	else if (token == TAB) {
	    printf("TAB");
	}
	else if (token == POS) {
	    printf("POS");
	}
	else if (token == USR) {
	    printf("USR");
	}
	else if (token == DEFSTR) {
	    printf("DEFSTR");
	}
	else if (token == DEFINT) {
	    printf("DEFINT");
	}
	else if (token == DEFSNG) {
	    printf("DEFSNG");
	}
	else if (token == DEFDBL) {
	    printf("DEFDBL");
	}
	else if (token == CONVERT) {
	    printf("CONVERT");
	}
	else if (token == UCASE) {
	    printf("UCASE");
	}	
	else if (token == CMP_EQ) {
		printf("CMP_EQ");
	}
	else if (token == FUN) {
	    printf("FUN(%s)", attrVal.sval);
	}	
	else if (token == UMINUS) {
		printf("UMINUS");
	}
	else if (token == VAR) {
		printf("VAR(%s)", attrVal.sval);
	}
	else if (token == '\n') {
		printf("newline");
	}
	else printf("%c", token);
}

void match(int expected) {
    if (lookahead == expected) {
        lookahead = getToken();
    }
    else {
    	printf("Syntax error: Expected token ");
		printToken(expected);
		printf(", found token ");
		printToken(lookahead);
		printf("\n");
    	exit(1);
    }
}

void Prog() {
	Node* lines = newNode(0);
	setFunList();
	// tab = merge(tab, funList);
    while (fgetc(fp) != EOF) {
    	Node* line = Line();
    	append(lines, line);
        if (lookahead = '\n')
        	match('\n');
    }
    printTree(lines, 0);
    append(lines, newNode(END));
    // printTree(lines, 0);
    exec(lines);
}

Node* Line() {
	Node* node;
	if (lookahead == '\n') {
		match('\n');
	}
	int lineNumber;
    if (lookahead == NUMBER) {
        match(NUMBER);
    	lineNumber = attrVal.ival;
    	node = Stmts();
    	linedStmt[lineNumber] = node;
    }
    
    else {
		node = Stmts();
	}
    return node;
}

Node* Stmts() {
	Node* node;
    node = Stmt();
    if (lookahead == ':') {
        match(':');
        append(node, Stmts());
    }
    return node;
}

Node* Stmt() {
	Node* node = newNode(lookahead);
    if (lookahead == REM) {
        match(REM);
    } else if (lookahead == QUOTEREM) {
        match(QUOTEREM);
    } else if (lookahead == BYE) {
        match(BYE);
    } else if (lookahead == CLS) {
        match(CLS);
	} else if (lookahead == CMD) {
        match(CMD);
    } else if (lookahead == POP) {
        match(POP);
    } else if (lookahead == CALL) {
        match(CALL);
        node->son = Expr();
    } else if (lookahead == CHANGE) {
        match(CHANGE);
        node->son = Var();
		match(TO);
        append(node->son, Var());
    } else if (lookahead == DATA) {
        match(DATA);
        node->son = ExprList();
    } else if (lookahead == DEF) {
		match(DEF);
        node->son = UserFunc();
        string functionName = string(node->son->val.sval);
        AttrVal val;
        val.tag = FUN;
        val.pval = node->son;
        funList[functionName] = val;
		tab[functionName] = val; 
        match('=');
        append(node->son->son, Expr());
    } else if (lookahead == DEFDBL) {
        match(DEFDBL);
        node->son = VarList();
    } else if (lookahead == DEFINT) {
        match(DEFINT);
        node->son = VarList();
    } else if (lookahead == DEFSNG) {
        match(DEFSNG);
        node->son =VarList();
    } else if (lookahead == DEFSTR) {
        match(DEFSTR);
        node->son = VarList();
    } else if (lookahead == DIM) {
        match(DIM);
        node->son = VarList();
    } else if (lookahead == END) {
        match(END);
    } else if (lookahead == EXIT) {
        match(EXIT);
    } else if (lookahead == FOR) {  	
        match(FOR);
        node->son = Var();
        match('=');
        append(node->son, Expr());
        match(TO);
        append(node->son, Expr());
        if (lookahead == STEP) {
            match(STEP);
            append(node->son, Expr());
		}
    } else if (lookahead == GOSUB) {
        match(GOSUB);
        if (lookahead == NUMBER) {
        	node->son = newNode(NUMBER);
			node->son->val.ival = attrVal.ival;
            match(NUMBER);
        } else {
            node->son = Expr();
            if (lookahead == OF) {
                append(node->son, newNode(OF));
				match(OF);
                append(node->son, ExprList());
            }
        }
    } else if (lookahead == GOTO) {
        match(GOTO);
        if (lookahead == NUMBER) {
        	node->son = newNode(NUMBER);
        	node->son->val.ival = attrVal.ival;
            match(NUMBER);
        } else {
            node->son = Expr();
            if (lookahead == OF) {
                append(node->son, newNode(OF));
				match(OF);
                append(node->son, ExprList());
            }
        }
    } else if (lookahead == INPUT) {
        match(INPUT);
        node->son = VarList();
		append(node->son, PrintList());
    } else if (lookahead == IF) {
        match(IF);
        node->son = Expr();
        if (lookahead == THEN) {
        	append(node->son, newNode(THEN));
            match(THEN);
            if (lookahead == NUMBER) {
            	append(node->son, newNode(NUMBER));
                match(NUMBER);
            } else {
                append(node->son, Stmts());
            }
        } else if (lookahead == GOTO) {
    		append(node->son, newNode(GOTO));
    		Node* tmp = node->son;
            match(GOTO);
            append(node->son, newNode(NUMBER));
            for(; tmp->bro; tmp = tmp->bro);
        	tmp->val.ival = attrVal.ival;
            match(NUMBER);
        }
    } else if (lookahead == LET) {
        match(LET);
        if (lookahead == TIME_STR) {
        	match(TIME_STR);
	        match('=');
	        node->son = newNode('=');
	        node->son->son = newNode(TIME_STR);
	        append(node->son->son, Expr());
		} else {
        node->son = newNode('=');
        node->son->son = Var();
        match('=');
        append(node->son->son, Expr());
    	}
    	addToEnv(node->son);
    } else if (lookahead == NEXT) {
        match(NEXT);
        if (lookahead == VARIABLE_NAME) {
            node->son = VarList();
        }
    } else if (lookahead == NEW) {
        match(NEW);
    } else if (lookahead == ON) {
        match(ON);
        node->son = Expr();
        if (lookahead == GOTO) {
            match(GOTO);
            append(node->son, newNode(GOTO));
            node->son->bro->son = ExprList();
        } else if (lookahead == GOSUB) {
            match(GOSUB);
            append(node->son, newNode(GOSUB));
            node->son->bro->son = ExprList();
        }
    } else if (lookahead == OPTION) {
        match(OPTION);
        match(BASE);
        node->son = newNode(BASE);
        append(node->son, Expr());
    } else if (lookahead == POKE) {
        match(POKE);
        node->son = Expr();
        match(',');
        append(node->son, Expr());
    } else if (lookahead == PRINT) {
        match(PRINT);
        if (lookahead == USING) {
        	node->son = newNode(USING);
            match(USING);
            append(node->son, Expr());
            match(';');
            append(node->son, PrintList());
        } else {
            node->son = PrintList();
        }
    } else if (lookahead == RANDOMIZE) {
        match(RANDOMIZE);
        if (lookahead != '\n') {
            node->son = Expr();
        }
    } else if (lookahead == READ) {
        match(READ);
        node->son = VarList();
    } else if (lookahead == RESTORE) {
        match(RESTORE);
        if (lookahead != '\n') {
            node->son = Expr();
        }
    } else if (lookahead == RETURN) {
        match(RETURN);
    } else if (lookahead == STOP) {
        match(STOP);
    } else if (lookahead == CONT) {
    	match(CONT);
	} else if (lookahead == STRING) { 
    	node->val.sval = attrVal.sval;
    	node->val.tag = STRING;
        match(STRING);
        node->son = Expr();
    } else if (lookahead == SYS) {
        match(SYS);
        node->son = Expr();
    } else if (lookahead == TIME_STR) {
        match(TIME_STR);
        match('=');
        node = newNode('=');
	    node->son = newNode(TIME_STR);
        append(node->son, Expr());
        addToEnv(node);
    } else if (lookahead == VARLIST) {
        match(VARLIST);
    } else if (lookahead == WAIT) {
        match(WAIT);
        node->son = Expr();
    } else {
        node = newNode('=');
        node->son = Var();
        match('=');
        append(node->son, Expr());
        // addToEnv(node);
    }
    return node;
}

Node* Expr() {
	return Expr0();
}

Node* Expr0() {
	Node* left;
	Node* node;
	
    left = Expr1();
    while (lookahead == AND || lookahead == OR) {
        node = newNode(lookahead);
        match(lookahead);
        node->son = left;
        append(left, Expr1());
        left = node;
    }
    return left;
}

Node* Expr1() {
	Node* left;
	Node* node;
	
    left = Expr2();
    while (lookahead == '=' || lookahead == '<' || lookahead == '>' ||
        lookahead == CMP_LE || lookahead == CMP_GE || lookahead == CMP_NE || lookahead == CMP_HASH) {
        node = newNode(lookahead);
        if (lookahead == '=') {
			node = newNode(CMP_EQ);
		}
        match(lookahead);
        node->son = left;
        append(left, Expr2());
        left = node;
    }
    return left;
}

Node* Expr2() {
    Node* left;
	Node* node;
	
    left = Expr3();
    while (lookahead == '+' || lookahead == '-' || lookahead == '&') {
        node = newNode(lookahead);
        match(lookahead);
        node->son = left;
        append(left, Expr3());
        left = node;
    }
    return left;
}

Node* Expr3() {
    Node* left;
    Node* node;
	
    left = Expr4();

    while (lookahead == '*' || lookahead == '/' || lookahead == '^') {
        node = newNode(lookahead);
        match(lookahead);
        node->son = left;
        append(left, Expr4());
        left = node;
    }
    return left;
}

Node* Expr4() {
    if (lookahead == '-' || lookahead == NOT) {
    	Node* node;
    	if (lookahead == '-')
    		node = newNode(UMINUS);
    	else node = newNode(NOT);
    	node->val.ival = lookahead;
        match(lookahead);
        node->son = Func();
        return node;
    }
    return Func();
}

Node* Func() {
	/*
    <func> ::= <factor>
    <factor> ::= NUMBER | STRING | <var> | <userfunc> | '(' <expr> ')'
    */
    if (lookahead == NUMBER || lookahead == STRING || lookahead == VARIABLE_NAME || lookahead == FUNCTION_NAME || lookahead == '(') {
    	return Factor();
	}
	
	/* 
	<fn0> ::= FRE | RND | TIME | TIME_STR
	<func> ::= <fn0> '(' ')'
			| <fn0> '(' <expr> ')'
	*/
	else if (lookahead == FRE || lookahead == RND || lookahead == TIME || lookahead == TIME_STR) {
		Node* node = newNode(FUN);
		node->val.ival = lookahead;
		match(lookahead);
		match('(');
		if(lookahead != ')') {
			node->son = Expr();
		}
		match(')');
		return node;
	}
	
	/*
	<fn1> ::= ABS | ATN | CHR | CLOG | COS | EXP | FIX | INT | LCASE | LEN | LIN | STR | LOG | PEEK | SGN | SIN | SPC | SQR | TAB | VAL | UCASE | USR 
	<func> ::= <fn1> '(' <expr> ')'
	
	*/
	else if (lookahead == ABS || lookahead == ATN || lookahead == CHR || lookahead == CLOG || lookahead == COS || lookahead == EXP || lookahead == FIX 
	|| lookahead == INT || lookahead == LCASE || lookahead == LEN || lookahead == LIN || lookahead == STR || lookahead == LOG || lookahead == PEEK || lookahead == SGN
	|| lookahead == SIN || lookahead == SPC || lookahead == SQR || lookahead == TAB || lookahead == VAL || lookahead == UCASE || lookahead == USR) {
		Node* node = newNode(FUN);
		char *fname = const_cast<char*>(getFunctionName(lookahead).c_str());
		node->val.sval = (char *) malloc(strlen(fname) + 1);
		strcpy(node->val.sval, fname);
    	match(lookahead);
    	match('(');
		node->son = Expr();
    	match(')');
    	return node;
	}
	
	/*
	<fn2> ::= LEFT | RIGHT | STRNG
	<func> ::= <fn2> '(' <expr> ',' <expr> ')'
	*/
	else if (lookahead == LEFT || lookahead == RIGHT || lookahead == STRNG) {
		Node* node = newNode(FUN);
		char *fname = const_cast<char*>(getFunctionName(lookahead).c_str());
		node->val.sval = (char *) malloc(strlen(fname) + 1);
		strcpy(node->val.sval, fname);
		match(lookahead);
		match('(');
		node->son = Expr();
		match(',');
		append(node->son, Expr());
		match(')');
		return node;
	}
	
	/*
	<fnx> ::= MID | SEG | SUBSTR
	<func> ::= <fnx> '(' <expr> ',' <expr> ',' <expr> ')'
	*/
	else if (lookahead == MID || lookahead == SEG || lookahead == SUBSTR) {
		Node* node = newNode(FUN);
		char *fname = const_cast<char*>(getFunctionName(lookahead).c_str());
		node->val.sval = (char *) malloc(strlen(fname) + 1);
		strcpy(node->val.sval, fname);
		match(lookahead);
		match('(');
		node->son = Expr();
		match(',');
		append(node->son, Expr());
		match(',');
		append(node->son, Expr());
		match(')');
		return node;
	}
}

Node* Factor() {
	Node* node;
    if (lookahead == NUMBER) {
    	node = newNode(lookahead);
    	node->val.ival = attrVal.ival;
    	node->val.tag = INT;
        match(NUMBER);
    } else if (lookahead == STRING) {
    	node = newNode(lookahead);
    	node->val.sval = attrVal.sval;
    	node->val.tag = STRING;
        match(STRING);
    } else if (lookahead == VARIABLE_NAME) {
        node = Var();
    } else if (lookahead == FUNCTION_NAME) {
        node = UserFunc();
    } else if (lookahead == '(') {
        match('(');
        node = Expr();
		match(')');
    }
    return node;
}

Node* UserFunc() {
	Node* node = newNode(FUN);
	node->val.sval = attrVal.sval;
	node->val.tag = FUN;
    match(FUNCTION_NAME);
    match('(');
	node->son = ExprList();
    match(')');
    return node;
}

Node* Var() {
	Node* node;

	node = newNode(VAR);
	node->val.sval = attrVal.sval;
	node->val.tag = VAR;
	
    if (lookahead == VARIABLE_NAME) {
        match(VARIABLE_NAME);
        if (lookahead == '(') {
            match('(');
            node->son = ExprList();
            match(')');
        } else if (lookahead == '[') {
            match('['); 
            ExprList();
            match(']');
        } else if (lookahead == '(') {
            match('(');
            SliceList();
            match(')');
        } else if (lookahead == '(') {
            match('(');
            ExprList();
            match(')');
            match('(');
            SliceList();
            match(')');
        }
    }
    return node;
}

Node* PrintList() {
	Node* node = newNode(0);
	while (true) {
		if (lookahead == EOF || lookahead == '\n') {
			break;
		}
	    if (lookahead == ',' || lookahead == ';') {
	    
	    	match(lookahead);
		}
		else append(node, Expr());
	}
	return node;
}

Node* ExprList() {
    Node* node;
	node = Expr();
    while (lookahead == ',') {
        match(',');
        append(node, Expr());
    }
    return node;
}

void SliceList() {
    Expr();
    match(':');
    Expr();
}

Node* VarList() {
	Node* node;
    node = Var();
    while (lookahead == ',') {
        match(',');
        append(node, Var());
    }
    return node;
}

void NumList() {
    match(NUMBER);
    while (lookahead == ',') {
        match(',');
        match(NUMBER);
    }
}

Node* newNode(int nID) {
	Node* node = (Node*)malloc(sizeof(Node));
	node->nID = nID;
	node->val.pval = node;
	node->val.tag = CLO;
	node->son = NULL;
	node->bro = NULL;
	return node;
}

void printTree(Node *node, int depth) {

	if (node == NULL) {
		return;
	}
	for (int i = 0; i < depth; i++) {
		if(node->nID == 0) break;
		printf("  ");
	}
	int token = node->nID; 
	attrVal = node->val;
	printToken(token);
	if(node->nID) printf("\n");
	printTree(node->son, depth + 1);
	printTree(node->bro, depth);
}

void append(Node *head, Node *elmt) {
	Node* tmp = head;
	for(; head->bro; head = head->bro);
	head->bro = elmt;
	head = tmp;
} 

void printEnv() {
	cout << endl << endl;
	cout << "---------env-----------" << endl;
	map<string, AttrVal> m = tab;
	map<string, AttrVal>::iterator iter;
	for (iter = m.begin(); iter != m.end(); iter++) {
		AttrVal v = iter->second;
		if (v.tag == INT) {
			cout << iter->first << ": (INT, " << v.ival << ")" <<endl;
		}
		else if (v.tag == STRING) {
			cout << iter->first << ": (STR, " << v.sval << ")" <<endl;
		}
		else if (v.tag == CLO) {
			cout << iter->first << ": (CLO, ";
			printToken(v.pval->nID); 
			cout << ")" << endl;
			printTree(v.pval, 0);
		}
		else if (v.tag == FUN) {
			cout << iter->first << ": (FUN, ";
			if(!v.pval) cout << "NULL)\n";
			else {
				Node* node = v.pval;
				printf("%s)\n", node->val.sval);
				printTree(v.pval, 1);
			}
		}
		else if (v.tag == ARR) {
			cout << iter->first << ": (ARR, ";
			Node* sizeNode = v.pval;
			string s;
			while (true) {
				int size = sizeNode->val.ival;
				s += to_string(size);
				if (!sizeNode->bro) {
					break;
				}
				sizeNode = sizeNode->bro;
				s += "x";
			}
			cout << s << ")" << endl;
			cout << "[ ";
			for (int i = 0; i < v.arr.size(); i++) {
				cout << v.arr[i].ival << " ";
			}
			cout << "]" << endl;
		}
	}
	cout << "-----------------------" << endl;
}

AttrVal mkInt(int i) {
	AttrVal val;
	val.ival = i;
	val.tag = INT;
	return val;
}

AttrVal mkStr(char* s) {
	AttrVal val;
	val.sval = (char*)malloc(strlen(s) + 1);
	strcpy(val.sval, s);
	val.tag = STRING;
	return val;
}

AttrVal mkClo(Node* n) {
	AttrVal val;
	val.pval = n;
	val.tag = CLO;
	return val;
}

bool isFN(char* name) {
	string fn = string(name);
	if (funList.find(fn) != funList.end() || lookahead == DEF) return true;
	return false;
}

string getFunctionName(int fn) {
	if (fn == FRE) return "FRE";
	if (fn == RND) return "RND";
	if (fn == TIME) return "TIME";
	if (fn == TIME_STR) return "TIME_STR";
	if (fn == ABS) return "ABS";
	if (fn == ATN) return "ATN";
	if (fn == CHR) return "CHR";
	if (fn == CLOG) return "CLOG";
	if (fn == COS) return "COS";
	if (fn == EXP) return "EXP";
	if (fn == FIX) return "FIX";
	if (fn == INT) return "INT";
	if (fn == LCASE) return "LCASE";
	if (fn == LEN) return "LEN";
	if (fn == LIN) return "LIN";
	if (fn == STR) return "STR";
	if (fn == LOG) return "LOG";
	if (fn == PEEK) return "PEEK";
	if (fn == SGN) return "SGN";
	if (fn == SIN) return "SIN";
	if (fn == SPC) return "SPC";
	if (fn == SQR) return "SQR";
	if (fn == TAB) return "TAB";
	if (fn == VAL) return "VAL";
	if (fn == UCASE) return "UCASE";
	if (fn == USR) return "USR";
	if (fn == LEFT) return "LEFT";
	if (fn == RIGHT) return "RIGHT";
	if (fn == STRNG) return "STRNG";
	if (fn == MID) return "MID";
	if (fn == SEG) return "SEG";
	if (fn == SUBSTR) return "SUBSTR";
}

void setFunList() {
	AttrVal tmp;
	tmp.tag = FUN;
	tmp.pval = NULL;
	for (int i = 0; i < NUM_FUNCTIONS; i++) {
        funList[functionNames[i]] = tmp;
    }
}

void printFunList() {
	cout << endl << endl;
	cout << "------functionList-----" << endl;
	map<string, AttrVal> m = funList;
	map<string, AttrVal>::iterator iter;
	for (iter = m.begin(); iter != m.end(); iter++) {
		cout << iter->first << endl;
		if (iter->second.pval) {
			printTree(iter->second.pval, 1);
		}
		else cout << "  NULL" << endl;
	}
	cout << "-----------------------" << endl;
}

map<string, AttrVal> merge(map<string, AttrVal> m1, map<string, AttrVal> m2) {
	for (auto &it : m2) {
		m1[it.first] = it.second;
	}
	return m1;
}

void addToEnv(Node* node) {
	char* varName = node->son->val.sval;
	string name(varName);
	if (tab.find(name) == tab.end()) {
		tab[name] = node->son->bro->val;
	}
} 

Node* exec(Node* node) {
	if (node == NULL) return NULL;
	
	int token = node->nID;
	bool ifStmtExecution = true;
	switch (token) {
		case IF: {
			Node* conditionNode = node->son;
			Node* stmtNode = conditionNode->bro;
			conditionNode->bro = NULL;
			ifStmtExecution = getValueOf(exec(conditionNode)).ival;
			conditionNode->bro = stmtNode;
			if (stmtNode->bro->nID == GOSUB) {
				if (ifStmtExecution) {
					Node* gosubNode = stmtNode->bro;
					callStack.push(node);
					int target = gosubNode->son->val.ival;
					Node* dst = linedStmt[target];
					exec(dst);
				}
				ifStmtExecution = false;
			} 
			else if (stmtNode->bro->nID == STOP) {
				Node* cont = traverse(node);
				if (!cont) {
					exit(1);
				}
			}
			break;
		}
		
		case ON: {
			Node* index = node->son;
			Node* branchNode = index->bro;
			Node* dst = branchNode->son;
			string varName = string(index->val.sval);
			int val = tab[varName].ival;
			val--;
			while (val--) {
				if (dst->bro) {
					dst = dst->bro;
					attrVal.ival = dst->val.ival;
				}
				else {
					cout << "error" << endl;
					exit(1);
				}
			}
			node->son->bro->son = dst;
			break;
		}
		
		case DEF: {
			ifStmtExecution = false;
			break;
		}
	}
	if (ifStmtExecution) {
		exec(node->son);
	}

	switch (token) {
		
		case PRINT: {
			string output;
			bool newline = true;
			if (node->son) {
				Node* left = node->son;
				while (left->bro) {
					newline = true;
					left = left->bro;
					if (left->nID == ',') {
						output += "\t";
					}
					else if (left->nID == ';') {
						newline = false;
						continue;
					}
					else if (left->val.tag == STRING) {
						output += getValueOf(left).sval;
					}
					else if (left->nID == VAR) {
						string varName = string(left->val.sval);
						if (tab[varName].tag == STRING) {
							output += getValueOf(left).sval;
						}
						else {
							string value = to_string(getValueOf(left).ival);
							output += value;
						}
					}
					else if (left->nID == FUN) {
						AttrVal val = getValueOf(left);
						if (val.tag == STRING) {
							output += val.sval;
						}
						else {
							string value = to_string(val.ival);
							output += value;
						}
					}
					else if (left->nID == NUMBER || left->val.tag == INT) {
						string value = to_string(getValueOf(left).ival);
						output += value;
					}
				}	
			}
			if (newline) output.push_back('\n');
			cout << output;
			break;
		} 
		
		case INPUT: {
			Node* varNode = node->son;
			vector<string> varNames;
			vector<AttrVal> vals;
			string varName = string(varNode->val.sval);
			while (true) {
				string varName = string(varNode->val.sval);
				varNames.push_back(varName);
				if (varNode->bro) {
					if (varNode->bro->val.tag != VAR) {
						break;
					}
				}
				varNode = varNode->bro;
			}
			Node* valNode = varNode->bro->bro;
		
			while (true) {
				AttrVal val = valNode->val;
				vals.push_back(val);
				if (!valNode->bro) {
					break;
				}
				valNode = valNode->bro;
			}
			
			
			for (int i = 0; i < varNames.size(); i++) {
				tab[varNames[i]] = vals[i];
			}
			break;
		}
		
		case REM: {
			break;
		}
		
		case DATA: {
			Node* elmt = node->son;
			while (true) {
				dataQueue.push_back(elmt);
				if (!elmt->bro) break;
				elmt = elmt->bro;
			}
			break;
		}
		
		
		case DIM: {
			Node* arrNode = node->son;
			Node* sizeNode = arrNode->son;
			int size;
			string arrName;
			
			size = sizeNode->val.ival;
			
			Node* tmp = sizeNode;
			while (sizeNode->bro) {
				sizeNode = sizeNode->bro;
				size *= sizeNode->val.ival;
			}
			sizeNode = tmp;
			
			AttrVal arrayVal;
			arrayVal.pval = sizeNode;
			arrayVal.tag = ARR;
			arrayVal.arr = vector<AttrVal>(size);
		
			arrName = string(arrNode->val.sval);
			
			tab[arrName] = arrayVal;
			
			break;
		}
		
		case END: {
			printEnv();
			exit(0);
			break;
		}
		
		case FOR: {
			int init, term, step;
			Node* var = node->son;
			Node* begin = var->bro;
			Node* end = begin->bro;
					
			string varName = string(var->val.sval);
			init = begin->val.ival;
			term = end->val.ival;
			tab[varName] = mkInt(init);
			if (end->bro) {
				Node* stepNode = end->bro;
				step = stepNode->val.ival;
			} 
			else {
				step = 1;
			}
			forTab[varName] = node;
			break;
		}
		
		case NEXT: {
			int init, term, step;
			Node* var = node->son;
			string varName;
			varName = string(var->val.sval);
			Node* forNode = forTab[varName];
			if (!forNode) {
				throw ExceptionCode::NextWithoutFor;
			}
			init = tab[varName].ival;
			Node* termNode = forNode->son->bro->bro;
			term = termNode->val.ival;
			if (termNode->bro) {
				step = termNode->bro->val.ival;	
			}
			else {
				step = 1;
			}
			init += step;
			tab[varName] = mkInt(init);
			if (init >= term) {
				tab.erase(varName);
				forTab.erase(varName);
				break;
			}
			exec(forTab[varName]->bro);
			break;
		}
		
		case GOSUB: {
			callStack.push(node);
			int target = node->son->val.ival;
			Node* dst = linedStmt[target];
			node = dst;
			exec(node);
			break;
		}
		
		case GOTO: {
			int target = node->son->val.ival;
			Node* dst = linedStmt[target];
			node = dst;
			exec(node);
			break;
		}
		
		case NEW: {
			map<string, AttrVal> emptyMap;
			tab = emptyMap;
			setFunList();
			merge(tab, funList);
			break;
		}
		
		case READ: {
			Node *elmt, *var;
			var = node->son;
			while (true) {
				string varName = string(var->val.sval);
				tab[varName] = dataQueue[dataQueueIndex++]->val;
				if (!var->bro) break;
				var = var->bro;
			}
			break;
		}
		
		case RESTORE: {
			dataQueueIndex = 0;
			break;
		}
		
		case STOP: {
			Node* cont = traverse(node);
			if (!cont) {
				exit(1);
			}
			break;
		}
		
		case RETURN: {
			Node* target = callStack.top();
			callStack.pop();
			exec(target->bro);
			break;
		}
	
		
		case '=': {
			
			if (node->son && node->son->bro) {
				Node* left = node->son;
				Node* right = node->son->bro;
				string varName = string(left->val.sval);
				if ((tab.find(varName) != tab.end()) && (tab[varName].tag == ARR)) {
					if (!left->son) {
						throw ExceptionCode::typeMisMatch;
					}
					int index = getIndex(left);
					tab[varName].arr[index] = getValueOf(exec(right));
				}
				else {
					tab[varName] = getValueOf(exec(right));
				}
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			
			break;
		}
		
		case AND: {
			if (node->son && node->son->bro) {
				Node* left = node->son;
				Node* right = node->son->bro;
				node->nID = NUMBER;
				node->val.ival = getValueOf(left).ival && getValueOf(right).ival;
				node->val.tag = INT;
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}
		
		case OR: {
			if (node->son && node->son->bro) {
				Node* left = node->son;
				Node* right = node->son->bro;
				node->nID = NUMBER;
				node->val.ival = getValueOf(left).ival || getValueOf(right).ival;
				node->val.tag = INT;
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}
		  
		case '+': {
			if (node->son && node->son->bro) {
				Node* left = node->son;
				Node* right = node->son->bro;
				bool isStringVarAdd = false;
				if ((left->val.tag == VAR) && (right->val.tag == VAR)) {
					string lname = string(left->val.sval);
					string rname = string(right->val.sval);
					if (tab[lname].tag == STRING && tab[rname].tag == STRING) {
						isStringVarAdd = true;
					}
				}
				else if ((left->val.tag == VAR) && right->val.tag == STRING) {
					string lname = string(left->val.sval);
					if (tab[lname].tag == STRING) {
						isStringVarAdd = true;
					}
				}
				else if ((left->val.tag == STRING) && (right->val.tag == VAR)) {
					string rname = string(right->val.sval);
					if (tab[rname].tag == STRING) {
						isStringVarAdd = true;
					}
				}
				if ((node->son->val.tag == STRING && node->son->bro->val.tag == STRING) || isStringVarAdd) {
					string res = string(getValueOf(left).sval) + string(getValueOf(right).sval);
					char *tmp = const_cast<char*>(res.c_str());
					node->val.sval = (char*) malloc(strlen(tmp) + 1);
					strcpy(node->val.sval, tmp);
					node->val.tag = STRING;
				}
				else {
					node->val.ival = getValueOf(left).ival + getValueOf(right).ival;
					node->val.tag = INT;	
				}
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}
		
		case '-': {
			if (node->son && node->son->bro) {
				Node* left = node->son;
				Node* right = node->son->bro;
				node->val.ival = getValueOf(left).ival - getValueOf(right).ival;
				node->val.tag = INT;
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}
		
		case '*': {
			if (node->son && node->son->bro) {
				Node* left = node->son;
				Node* right = node->son->bro;
				node->val.ival = getValueOf(left).ival * getValueOf(right).ival;
				node->val.tag = INT;
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}
		
		case '/': {
			if (node->son && node->son->bro) {
				Node* left = node->son;
				Node* right = node->son->bro;
				if (getValueOf(right).ival == 0) {
					cout << "error: div by zero" << endl;
					exit(1);
				}
				node->val.ival = getValueOf(left).ival / getValueOf(right).ival;
				node->val.tag = INT;
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}
		
		case '&': {
			if (node->son && node->son->bro) {
				Node* left = node->son;
				Node* right = node->son->bro;
				node->val.ival = getValueOf(left).ival & getValueOf(right).ival;
				node->val.tag = INT;
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}
		
		case '^': {
			if (node->son && node->son->bro) {
				Node* left = node->son;
				Node* right = node->son->bro;
				node->val.ival = pow(getValueOf(left).ival, getValueOf(right).ival);
				node->val.tag = INT;
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}

		case CMP_EQ: {
			if (node->son && node->son->bro) {
				Node* left = node->son;
				Node* right = node->son->bro;
				node->val.ival = getValueOf(left).ival == getValueOf(right).ival;
				node->val.tag = INT;
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}
		
		case '<': {
			if (node->son && node->son->bro) {
				Node* left = node->son;
				Node* right = node->son->bro;
				node->val.ival = getValueOf(left).ival < getValueOf(right).ival;
				node->val.tag = INT;
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}
		
		case '>': {
			if (node->son && node->son->bro) {
				Node* left = node->son;
				Node* right = node->son->bro;
				node->val.ival = getValueOf(left).ival > getValueOf(right).ival;
				node->val.tag = INT;
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}
		
		case CMP_LE: {
			if (node->son && node->son->bro) {
				Node* left = node->son;
				Node* right = node->son->bro;
				node->val.ival = getValueOf(left).ival <= getValueOf(right).ival;
				node->val.tag = INT;
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}
		
		case CMP_GE: {
			if (node->son && node->son->bro) {
				Node* left = node->son;
				Node* right = node->son->bro;
				node->val.ival = getValueOf(left).ival >= getValueOf(right).ival;
				node->val.tag = INT;
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}
		
		case CMP_NE: {
			if (node->son && node->son->bro) {
				Node* left = node->son;
				Node* right = node->son->bro;
				node->val.ival = getValueOf(left).ival != getValueOf(right).ival;
				node->val.tag = INT;
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}
		
		case UMINUS: {
			if (node->son) {
				Node* left = node->son;
				node->val.ival = -getValueOf(left).ival;
				node->val.tag = INT;
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}
		
		case NOT: {
			if (node->son) {
				Node* left = node->son;
				node->val.ival = !getValueOf(left).ival;
				node->val.tag = INT;
			}
			else {
				cout << "error" << endl;
				exit(1);
			}
			break;
		}
	}

	exec(node->bro);
	
	return node;
}

AttrVal getValueOf(Node* node) {
	if (node->val.tag == INT) {
		return node->val;
	}
	if (node->nID == NUMBER) {
		return node->val;
	}
	else if (node->nID == UMINUS) {
		AttrVal v;
		v.ival = -node->son->val.ival;
		return v;
	}
	else if (node->nID == VAR) {
		string varName = string(node->val.sval);
		if (tab.find(varName) != tab.end()) {
			if (tab[varName].tag == ARR) {
				int index = getIndex(node);
				return tab[varName].arr[index];
			}
			return tab[varName];
		}
		else {
			throw ExceptionCode::NOT_DECLARED;
		}
	}
	else if (node->nID == FUN) {
		map<string, AttrVal> declaredVars;
		vector<string> used;
		AttrVal result;
		string funName = string(node->val.sval);
		
		if (tab.find(funName) == tab.end()) {
			throw ExceptionCode::NOT_DECLARED;
		}
		
		if (isBuiltinFunction(funName)) {
			return getBuiltinFunctionResult(funName, node);
		}
		
		Node* param = tab[funName].pval->son;

		Node* arg = node->son;
		

		while (true) {
			if (!(param->bro)) {
				break;
			}
			string varName = string(param->val.sval);
			if (tab.find(varName) != tab.end()) {
				declaredVars[varName] = tab[varName];
			}
			tab[varName] = getValueOf(arg);
			used.push_back(varName);
			param = param->bro;
			arg = arg->bro;
		}

		Node* resultNode = exec(param);

		result = getValueOf(resultNode);
		for (string varName : used) {
			if (declaredVars.find(varName) == declaredVars.end()) {
				tab.erase(varName);
			}
			else {
				tab[varName] = declaredVars[varName];
			}
		}
		return result;
		
	}
	else if (node->val.tag == STRING) {
		return node->val;
	} 
}

bool isComparisonOperator(char c) {
	return (c == '>' || c == '<' || c == '=');
}

int getIndex(Node* varNode) {
	string varName = string(varNode->val.sval);
	int index = 0, size;
	Node* sizeNode = tab[varName].pval;
	Node* indexNode = varNode->son;
	size = tab[varName].arr.size();
	
	while (true) {
		size /= sizeNode->val.ival;
		index += getValueOf(indexNode).ival * size;
		if (sizeNode->bro == NULL) {
			break;
		}
		sizeNode = sizeNode->bro;
		indexNode = indexNode->bro;
	}
	return index;
}

bool isAllDigits(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool isBuiltinFunction(string fname) {
	for (int i = 0; i < NUM_FUNCTIONS; i++) {
		if (fname == functionNames[i]) {
			return true;
		}
	}
	return false;
}

AttrVal getBuiltinFunctionResult(string fname, Node* node) {
	AttrVal result;
	if (fname == "ABS") {
		int v = getValueOf(node->son).ival;
		result.ival = abs(v);
		result.tag = INT;
	}
	else if (fname == "MID") {
		string str;
		int start, len;
		Node* strNode = node->son;
		Node* startNode = strNode->bro;
		Node* lenNode = startNode->bro;
	
		str = string(getValueOf(strNode).sval);
		start = startNode->val.ival;
		len = lenNode->val.ival;
		
		str = str.substr(start, len);
		
		result.tag = STRING;
		char *s = const_cast<char*>(str.c_str());
		result.sval = (char *) malloc(strlen(s) + 1);
		strcpy(result.sval, s);
	}

	return result;
}

bool ishangul(char c) {
	if (c == EOF) return false;
	return (c & 0x80);
}

const char* toUtf8(const char* ansi) {
    WCHAR unicode[MAXLEN];
    char utf8[MAXLEN * 3];
    MultiByteToWideChar(CP_ACP, 0, ansi, -1, unicode, sizeof(unicode) / sizeof(WCHAR));
    WideCharToMultiByte(CP_UTF8, 0, unicode, -1, utf8, sizeof(utf8), NULL, NULL);
    return _strdup(utf8);
}

Node* traverse(Node* node) {
	if (!node) return NULL;
	
	if (node->nID == CONT) {
		return node;
	}
	
	traverse(node->son);
	traverse(node->bro);
}
