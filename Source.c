#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include<string.h>

void error(char *m);
void emit(int t,int tval);
void parse();
void expr();
void term();
void factor();
void match();
int lookup(char s[]);
int insert(char s[],  int tok);
int lexan();
void init();


/* load i/o routines */
/* load character test routines */
#define BSIZE 128 /* buffer size */
#define NONE -1
#define EOS '\0'

#define NUM 256
#define DIV 257
#define MOD 258
#define ID 259
#define DONE ']'
#define PROGRAM 261
#define INF 262
#define POS 263

int tokenval; /* value of token attribute */
int lineno;











struct entry { /* form of symbol table entry */
    char *lexptr;
    int token;
};

struct entry symtable[];  /*  symbol table */


///////////////////////////////////////////

#define STRMAX 999 /* size of lexemes array */
#define SYMMAX 100 /* size of symtable */
char lexemes [STRMAX];
int lastchar = -1; /*last used position in lexemes */
struct entry symtable[SYMMAX];
int lastentry = 0; /* last used position in symtable */

int lookup(char s[])  /* returns position of entry for s */    
{
    int p;
    for (p = lastentry; p > 0; p = p - 1)
		if (strcmp( symtable[p].lexptr, s) == 0)
            return p;
    return 0;
}
int insert(char s[],  int tok) /* returns position of entry for s */
{
    int len;
    len = strlen(s); /* strlen computes length of s */
    if (lastentry + 1 >= SYMMAX)
        error("symbol table full");
    if (lastchar + len + 1 >= STRMAX)
        error("lexemes array full");
    lastentry = lastentry + 1;
    symtable[lastentry].token = tok;
    symtable[lastentry].lexptr = &lexemes[lastchar + 1];
    lastchar = lastchar + len + 1;
    strcpy(symtable[lastentry].lexptr, s);
    return lastentry;
}
////////////////////////////////////////
int lookahead;

void parse() /* parses and translates expression list */
{   
    lookahead = lexan();
	if (lookahead==PROGRAM)
		emit(lookahead,NONE);
	   else error("syntax error");

	 lookahead = lexan();
	if (lookahead=='(')
		printf("(");
	else error("syntax error");

	 lookahead = lexan();
	if (lookahead==INF)
		emit(lookahead,NONE);
	else error("syntax error");

	 lookahead = lexan();
	if (lookahead==',')
		printf(",");
	else error("syntax error");

	 lookahead = lexan();
	if (lookahead==POS)
		emit(lookahead,NONE);
	else error("syntax error");

	 lookahead = lexan();
	if (lookahead==')')
		printf(")");
	else error("syntax error");

	 lookahead = lexan();
	if (lookahead=='[')
		printf("[");
	else error("syntax error");

	 lookahead = lexan();
	while (lookahead != DONE) {
        expr(); match(';');
	} 
	printf("]");
	
}


void expr()
{
    int t;
    term();
    while (1)
        switch (lookahead) {
        case '+': case '-':
            t = lookahead;
            match(lookahead); term(); emit(t, NONE);
            continue;
        default:
            return;
        }
}
void term()
{
    int t;
    factor();
    while (1)
        switch (lookahead) {
        case '*': case '/': case DIV: case MOD:
            t = lookahead;
            match(lookahead); factor(); emit(t, NONE);
            continue;

        default:
            return;
        }
}

void factor()
{
    switch (lookahead) {

    case '(':
        match('('); expr(); match(')'); break;
    case NUM:
        emit(NUM, tokenval); match(NUM); break;
    case ID:
        emit(ID, tokenval); match(ID); break;
    default:
        error("syntax error");
    }

}

void match(int t)
{
    if (lookahead == t)
        lookahead = lexan();
    else error("syntax error");
}


///////////////////////////////
char lexbuf[BSIZE];
int lineno = 1;
int tokenval = NONE;
int lexan() /* lexical analyzer */
{
    int t;
    while (1) {
        t = getchar();
        if (t == ' ' || t == '\t')
            ; /* strip out white space */
        else if (t == '\n')
            lineno = lineno + 1;
        else if (isdigit(t)) { /* t is a digit */
            ungetc(t, stdin);
            scanf_s("%d", &tokenval);
            return NUM;
        }
        else if (isalpha(t)) { /* t is a letter */
            int p, b = 0;
            while (isalnum(t)) { /* t is alphanumeric */
                lexbuf[b] = t;
                t = getchar();
                b = b + 1;
                if (b >= BSIZE)
                    error("compiler error");
            }
            lexbuf[b] = EOS;
            if (t != EOF)
                ungetc(t, stdin);
            p = lookup(lexbuf);
            if (p == 0)
                p = insert(lexbuf, ID);
            tokenval = p;
            return symtable[p].token;
        }
        else if (t == EOF)
            return DONE;

        else {
            tokenval = NONE;
            return t;
        }

    }
}


//////////////////////////////
struct entry keywords[] = {
    "div", DIV,
    "mod", MOD,
	"program", PROGRAM,
	"inf",INF,
	"pos",POS,
     0, 0
	
};

void init() /* loads keywords into symtable */
{
    struct entry *p;
    for (p = keywords; p->token; p++)
        insert(p->lexptr, p->token);
}
////////////////////////////

void error(char *m) /* generates all error */
{
fprintf(stderr, "line %d: %s\n", lineno, m);
exit(1); /* unsuccessful termination */
}
//////////////////////////////////
void emit(int t,int tval) /* generates output */
{
    switch(t) {
    case '+': case '-': case '*': case '/':
        printf("%c\n", t); break;
    case DIV:
        printf("DIV\n"); break;
	case PROGRAM:
        printf("program\n"); break;
	case INF:
        printf("inf\n"); break;
	case POS:
        printf("pos\n"); break;
    case MOD:
        printf ("MOD\n"); break;
    case NUM:
        printf("%d\n", tval); break;
    case ID:
        printf("%s\n", symtable[tval].lexptr); break;
    default:
        printf("token %d,tokenval %d\n", t, tval);
    }
}

////////////////////////////////
main()
{
    init();
    parse();
    exit(0); /* successful termination */
}

