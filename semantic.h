#ifndef SEMANTIC_H
#define SEMANTIC_H

#define VARMAP 5 // Variable map will increment its' size if needed

void semantic(NonTerminal *nt);
void semanticAnalyze(NonTerminal *nt);
void semanticError(char *, char *, int);

// Custom made data type for holding variables
typedef struct{
    TokenType type;
    char name[MAXLEN];
    char data[MAXLEN];
    int index;
} Var;

// Variable map later to be allocated
Var *varMap;

Var getVar(NonTerminal *);
Var getExpression(NonTerminal *nt);
int setVar(Var);

#endif