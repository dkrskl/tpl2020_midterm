#ifndef PARSER_H
#define PARSER_H

#define CHILDRENMAX 5 // Max children for NonTerminals

void * declarationPart();
void * programBlock();
void * statementPart();
void * variableDeclaration();
void * identifier();
void * type();
void * statement();
void * simpleStatement();
void * termFactor();
void * assignmentStatement();
void * expression();
void * structuredStatement();
void * relativeExpression();
void * ifStatement();
void * repeatStatement();
void * variable();
void * indexRange();
void * constant();
void syntaxError(char *);

// Enumerator for NonTerminal Types
typedef enum{
    PROGRAMBLOCK, DECLARATIONPART, STATEMENTPART, VARIABLEDECLARATION, IDENTIFIER, TYPE, STATEMENT, SIMPLESTATEMENT, ASSIGNMENTSTATEMENT,
    EXPRESSION, TERMFACTOR, STRUCTUREDSTATEMENT, RELATIVEEXPRESSION, IFSTATEMENT, REPEATSTATEMENT, VARIABLE, INDEXRANGE, NOTSTATE, CONSTANT

} NtType;

// Custom made data type that holds nonterminals
typedef struct NonTerminal {
    NtType ntType;
    Token *token;
    struct NonTerminal *child[CHILDRENMAX];
} NonTerminal;


NonTerminal * parse(FILE *);
char * getNonTerminalName(NonTerminal*, char*);
NonTerminal * getNonTerminal(NtType);
void drawTree(NonTerminal *, int);
void insertTerminal(NonTerminal *, Token);
Token * getTokenPointer(Token);

#endif