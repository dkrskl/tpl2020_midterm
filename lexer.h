#ifndef LEXER_H //include guards
#define LEXER_H

Token lex(FILE *);
int isKeyword(char *);
TokenType recKeyword(char *);
void printToken(Token);
char *getTokenType(Token, char *);
TokenType recOperator(char);


#endif