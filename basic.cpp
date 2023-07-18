#include <iostream>
#include <string.h>
#include <stdlib.h>

#define MAXLEN 100

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
#define	EXIT 141
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
#define FUNCTION_NAME 201
#define VARIABLE_NAME 202
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

struct _node;

typedef union _AttrVal{
	int ival;
	char* sval;
	struct _node* pval;
} AttrVal;

typedef struct _node {
	int nID;
	AttrVal val;
	struct _node *son, *bro; 
} Node;

int getToken();
void printToken(int token);
FILE* fp = stdin;
void match(int expected);
void Prog();
void Line();
void Stmts();
void Stmt();
void Expr();
void Expr0();
void Expr1();
void Expr2();
void Expr3();
void Expr4();
void Func();
void Factor();
void UserFunc();
void Var();
void PrintList();
void PrintSep();
void ExprList();
void SliceList();
void VarList();
void NumList();
int lookahead;
AttrVal attrVal;
Node* newNode(int);

int main(void) {
	lookahead = getToken();
	Prog();
	return 0;
}

int getToken() {
	char c, buffer[MAXLEN];
	int i = 0;
	while (c = getchar()) {
		if (c == '\n') return '\n';
		if (isspace(c)) continue;
		
		// number token
		if (isdigit(c)) {
			buffer[i++] = c;
			while (c = getchar()) {
				if (isdigit(c)) {
					
					buffer[i++] = c;
					
				}
				else {
					ungetc(c, fp);
					buffer[i] = '\0';
					break;
				}
			}
			attrVal.ival = atoi(buffer);
			return NUMBER;
		}
		
		// stirng token
		if (c == '"') {
			while (c = getchar()) {
				if (c != '"') {
					buffer[i++] = c;					
				}
				else {
					buffer[i] = '\0';
					break;
				}
			}
			attrVal.sval = (char*)malloc(strlen(buffer) + 1);
			strcpy(attrVal.sval, buffer);
			return STRING;
		}
		
		// otherwise
		if (isalpha(c)) {
			buffer[i++] = c;
			while (c = getchar()) {
				if (isalpha(c) || c == '_') {
					buffer[i++] = c;
				}
				else {
					ungetc(c, fp);
					buffer[i] = '\0';
					
					// to upper
					for(char* c = buffer; *c = toupper(*c); c++);
					
					if (!strcmp(buffer, "REM")) return REM;
					else if (!strcmp(buffer, "QUOTEREM")) return QUOTEREM;
					else if (!strcmp(buffer, "BYE")) return BYE;
					else if (!strcmp(buffer, "CALL")) return CALL;
					else if (!strcmp(buffer, "CHANGE")) return CHANGE;
					else if (!strcmp(buffer, "CLS")) return CLS;
					else if (!strcmp(buffer, "DATA")) return DATA;
					else if (!strcmp(buffer, "END")) return END;
					else if (!strcmp(buffer, "EXIT")) return EXIT;
					else if (!strcmp(buffer, "FOR")) return FOR;
					else if (!strcmp(buffer, "TO")) return TO;
					else if (!strcmp(buffer, "GOSUB")) return GOSUB;
					else if (!strcmp(buffer, "OF")) return OF;
					else if (!strcmp(buffer, "INPUT")) return INPUT;
					else if (!strcmp(buffer, "THEN")) return THEN;
					else if (!strcmp(buffer, "IF")) return IF;
					else if (!strcmp(buffer, "GOTO")) return GOTO;
					else if (!strcmp(buffer, "LET")) return LET;
					else if (!strcmp(buffer, "NEXT")) return NEXT;
					else if (!strcmp(buffer, "NEW")) return NEW;
					else if (!strcmp(buffer, "ON")) return ON;
					else if (!strcmp(buffer, "OPTION")) return OPTION;
					else if (!strcmp(buffer, "BASE")) return BASE;
					else if (!strcmp(buffer, "POKE")) return POKE;
					else if (!strcmp(buffer, "POP")) return POP;
					else if (!strcmp(buffer, "PRINT")) return PRINT;
					else if (!strcmp(buffer, "USING")) return USING;
					else if (!strcmp(buffer, "RANDOMIZE")) return RANDOMIZE;
					else if (!strcmp(buffer, "READ")) return READ;
					else if (!strcmp(buffer, "RESTORE")) return RESTORE;
					else if (!strcmp(buffer, "RETURN")) return RETURN;
					else if (!strcmp(buffer, "STOP")) return STOP;
					else if (!strcmp(buffer, "STRING")) return STRING;
					else if (!strcmp(buffer, "SYS")) return SYS;
					else if (!strcmp(buffer, "TIME_STR")) return TIME_STR;
					else if (!strcmp(buffer, "VARLIST")) return VARLIST;
					else if (!strcmp(buffer, "WAIT")) return WAIT;
					else if (!strcmp(buffer, "AND")) return AND;
					else if (!strcmp(buffer, "OR")) return OR;
					else if (!strcmp(buffer, "CMP_LE")) return CMP_LE;
					else if (!strcmp(buffer, "CMP_GE")) return CMP_GE ;
					else if (!strcmp(buffer, "CMP_NE")) return CMP_NE;
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
					else if (!strcmp(buffer, "MID")) return MID;
					else if (!strcmp(buffer, "SEG")) return SEG;
					else if (!strcmp(buffer, "SUBSTR")) return SUBSTR;
					else if (!strcmp(buffer, "FUNCTION_NAME")) return FUNCTION_NAME;
					else if (!strcmp(buffer, "VARIABLE_NAME")) return VARIABLE_NAME;
					else if (!strcmp(buffer, "DOUBLE")) return DOUBLE;
					else if (!strcmp(buffer, "SINGLE")) return SINGLE;
					else if (!strcmp(buffer, "INTEGER")) return INTEGER;
					else if (!strcmp(buffer, "BANGREM")) return BANGREM;
					else if (!strcmp(buffer, "CLEAR")) return CLEAR;
					else if (!strcmp(buffer, "CLR")) return CLR;
					else if (!strcmp(buffer, "DIM")) return DIM;
					else if (!strcmp(buffer, "GET")) return GET;
					else if (!strcmp(buffer, "LIST")) return LIST;
					else if (!strcmp(buffer, "STEP")) return STEP;
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
					else return -1;
				}
			}
		}
		// Handling single character token
		return c;
	}
	return -1;
}

void printToken(int token) {
	if (token == -1) {
		printf("Not A Token");
	}
	else if (token == STRING) {
	    printf("STRING(%s)", attrVal.sval);
	}
	else if (token == NUMBER) {
		printf("NUM(%d)", attrVal.ival);
	} 
	else if (token == QUOTEREM) {
		printf("QUOTEREM");
	}
	else if (token == BYE) {
		printf("BYE");
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
	else if (token == FUNCTION_NAME) {
	    printf("FUNCTION_NAME");
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
	else if (token == '\n') {
		printf("newline");
	}
	else printf("%c", token);
}

void match(int expected) {
	printToken(expected);
	printf("\n");
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
    while (getchar() != EOF) {
        Line();
        match('\n');
    }
}

void Line() {
    if (lookahead == NUMBER) {
        match(NUMBER);
    }
    Stmts();
}
void Stmts() {
    Stmt();
    if (lookahead == ':') {
        match(':');
        Stmts();
    }
}

void Stmt() {
    if (lookahead == REM) {
        match(REM);
    } else if (lookahead == QUOTEREM) {
        match(QUOTEREM);
    } else if (lookahead == BYE) {
        match(BYE);
    } else if (lookahead == CALL) {
        match(CALL);
        Expr();
    } else if (lookahead == CHANGE) {
        match(CHANGE);
        Var();
        match(TO);
        Var();
    } else if (lookahead == CLS) {
        match(CLS);
    } else if (lookahead == CMD) {
        match(CMD);
    } else if (lookahead == DATA) {
        match(DATA);
        ExprList();
    } else if (lookahead == DEF) {
        match(DEF);
        UserFunc();
        match('=');
        Expr();
    } else if (lookahead == DEFDBL) {
        match(DEFDBL);
        VarList();
    } else if (lookahead == DEFINT) {
        match(DEFINT);
        VarList();
    } else if (lookahead == DEFSNG) {
        match(DEFSNG);
        VarList();
    } else if (lookahead == DEFSTR) {
        match(DEFSTR);
        VarList();
    } else if (lookahead == DIM) {
        match(DIM);
        VarList();
    } else if (lookahead == END) {
        match(END);
    } else if (lookahead == EXIT) {
        match(EXIT);
    } else if (lookahead == FOR) {
        match(FOR);
        Var();
        match('=');
        Expr();
        match(TO);
        Expr();
        if (lookahead == STEP) {
            match(STEP);
            Expr();
        }
    } else if (lookahead == GOSUB) {
        match(GOSUB);
        if (lookahead == NUMBER) {
            match(NUMBER);
        } else {
            Expr();
            if (lookahead == OF) {
                match(OF);
                ExprList();
            }
        }
    } else if (lookahead == GOTO) {
        match(GOTO);
        if (lookahead == NUMBER) {
            match(NUMBER);
        } else {
            Expr();
            if (lookahead == OF) {
                match(OF);
                ExprList();
            }
        }
    } else if (lookahead == INPUT) {
        match(INPUT);
        PrintList();
    } else if (lookahead == IF) {
        match(IF);
        Expr();
        if (lookahead == THEN) {
            match(THEN);
            if (lookahead == NUMBER) {
                match(NUMBER);
            } else {
                Stmts();
            }
        } else if (lookahead == GOTO) {
            match(GOTO);
            match(NUMBER);
        }
    } else if (lookahead == LET) {
        match(LET);
        Var();
        match('=');
        Expr();
    } else if (lookahead == NEXT) {
        match(NEXT);
        if (lookahead != ',') {
            VarList();
        }
    } else if (lookahead == NEW) {
        match(NEW);
    } else if (lookahead == ON) {
        match(ON);
        Expr();
        if (lookahead == GOTO) {
            match(GOTO);
            ExprList();
        } else if (lookahead == GOSUB) {
            match(GOSUB);
            ExprList();
        }
    } else if (lookahead == OPTION) {
        match(OPTION);
        match(BASE);
        Expr();
    } else if (lookahead == POKE) {
        match(POKE);
        Expr();
        match(',');
        Expr();
    } else if (lookahead == POP) {
        match(POP);
    } else if (lookahead == PRINT) {
        match(PRINT);
        if (lookahead == USING) {
            match(USING);
            Expr();
            match(';');
            PrintList();
        } else {
            PrintList();
        }
    } else if (lookahead == RANDOMIZE) {
        match(RANDOMIZE);
        if (lookahead != ',') {
            Expr();
        }
    } else if (lookahead == READ) {
        match(READ);
        VarList();
    } else if (lookahead == RESTORE) {
        match(RESTORE);
        if (lookahead != ',') {
            Expr();
        }
    } else if (lookahead == RETURN) {
        match(RETURN);
    } else if (lookahead == STOP) {
        match(STOP);
        if (lookahead != ',') {
            Expr();
        }
    } else if (lookahead == STRING) {
        match(STRING);
        Expr();
    } else if (lookahead == SYS) {
        match(SYS);
        Expr();
    } else if (lookahead == TIME_STR) {
        match(TIME_STR);
        match('=');
        Expr();
    } else if (lookahead == LET) {
        match(LET);
        match(TIME_STR);
        match('=');
        Expr();
    } else if (lookahead == VARLIST) {
        match(VARLIST);
    } else if (lookahead == WAIT) {
        match(WAIT);
        Expr();
    } else {
        Var();
        match('=');
        Expr();
    }
}

void Expr() {
    Expr0();
}

void Expr0() {
    Expr1();
    while (lookahead == AND || lookahead == OR) {
        match(lookahead);
        Expr1();
    }
}

void Expr1() {
    Expr2();
    while (lookahead == '=' || lookahead == '<' || lookahead == '>' ||
        lookahead == CMP_LE || lookahead == CMP_GE || lookahead == CMP_NE || lookahead == CMP_HASH) {
        match(lookahead);
        Expr2();
    }
}

void Expr2() {
    Expr3();
    while (lookahead == '+' || lookahead == '-' || lookahead == '&') {
        match(lookahead);
        Node* e2opNode = newNode(lookahead);
        Expr3();
    }
}

void Expr3() {
    Expr4();
    while (lookahead == '*' || lookahead == '/' || lookahead == '^') {
        match(lookahead);
        Expr4();
    }
}

void Expr4() {
    if (lookahead == '-' || lookahead == NOT) {
        match(lookahead);
    }
    Func();
}

void Func() {
	/*
    <func> ::= <factor>
    <factor> ::= NUMBER | STRING | <var> | <userfunc> | '(' <expr> ')'
    */
    if (lookahead == NUMBER || lookahead == STRING || lookahead == VARIABLE_NAME || lookahead == FUNCTION_NAME || lookahead == '(') {
    	Factor();
	}
	
	/* 
	<fn0> ::= FRE | RND | TIME | TIME_STR
	<func> ::= <fn0> '(' ')'
			| <fn0> '(' <expr> ')'
	*/
	else if (lookahead == FRE || lookahead == RND || lookahead == TIME || lookahead == TIME_STR) {
		match(lookahead);
		match('(');
		if(lookahead == ')') {
			match(')');
		}
		else {
			Expr();
			match(')');
		}
	}
	
	/*
	<fn1> ::= ABS | ATN | CHR | CLOG | COS | EXP | FIX 
	| INT | LCASE | LEN | LIN | STR | LOG | PEEK | SGN
	<func> ::= <fn1> '(' <expr> ')'
	
	*/
	else if (lookahead == ABS || lookahead == ATN || lookahead == CHR || lookahead == CLOG || lookahead == COS || lookahead == EXP || lookahead == FIX 
	|| lookahead == INT || lookahead == LCASE || lookahead == LEN || lookahead == LIN || lookahead == STR || lookahead == LOG || lookahead == PEEK || lookahead == SGN) {
    	match(lookahead);
    	match('(');
    	Expr();
    	match(')');
	}
	
	/*
	<fn2> ::= LEFT | RIGHT | STRNG
	<func> ::= <fn2> '(' <expr> ',' <expr> ')'
	*/
	else if (lookahead == LEFT || lookahead == RIGHT || lookahead == STRNG) {
		match(lookahead);
		match('(');
		Expr();
		match(',');
		Expr();
		match(')');
	}
	
	/*
	<fnx> ::= MID | SEG | SUBSTR
	<func> ::= <fnx> '(' <expr> ',' <expr> ',' <expr> ')'
	*/
	else if (lookahead == MID || lookahead == SEG || lookahead == SUBSTR) {
		match(lookahead);
		match('(');
		Expr();
		match(',');
		Expr();
		match(',');
		Expr();
		match(')');
	}
}

void Factor() {
    if (lookahead == NUMBER) {
        match(NUMBER);
    } else if (lookahead == STRING) {
        match(STRING);
    } else if (lookahead == VARIABLE_NAME) {
        Var();
    } else if (lookahead == FUNCTION_NAME) {
        UserFunc();
    } else if (lookahead == '(') {
        match('(');
        Expr();
        match(')');
    }
}

void UserFunc() {
    match(FUNCTION_NAME);
    match('(');
    ExprList();
    match(')');
}

void Var() {
    if (lookahead == VARIABLE_NAME) {
        match(VARIABLE_NAME);
        if (lookahead == '(') {
            match('(');
            ExprList();
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
}

void PrintList() {
    Expr();
    while (lookahead == ',' || lookahead == ';') {
    	match(lookahead);
    	Expr();
	}
}

void PrintSep() {
    if (lookahead == ',' || lookahead == ';') {
        match(lookahead);
    }
}

void ExprList() {
    Expr();
    while (lookahead == ',') {
        match(',');
        Expr();
    }
}

void SliceList() {
    Expr();
    match(':');
    Expr();
}

void VarList() {
    Var();
    while (lookahead == ',') {
        match(',');
        Var();
    }
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
	node->val.ival = -1;
	node->son = node->bro = NULL;
	return node;
}

