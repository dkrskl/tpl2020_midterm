#ifndef TOKENS_H //include guards
#define TOKENS_H

#define MAXLEN 100

typedef enum{
    TYPEINT,
    TYPEFLT,
    TYPEBOOL,
    TYPECHAR,

    IDENT,

    // Types
    LITINT, // 6
    LITFLT,
    STRING,
    TRUE, FALSE, // Boolean

    // Keywords for these Types


    // Statements, Blocks
    START, END, IF, ELSE, REPEAT, VARS, STATEMENTEND, STATEMENTBEGIN,

    // Delimiters, line enders etc.
    SEMICOLON, LPAR, RPAR, LBRACE, RBRACE, QUOTE, COMMA, COLONS, DOT,

    // Assignment
    ASSIGN, ADD, MUL, DIV, SUB, NOT,

    // Relational operators
    GREATER, LESSER, EQUAL, GREATEREQ, LESSEREQ,

    UNK, // Unknown
    EOFTK // EOF token

} TokenType;


typedef struct Token{

    TokenType tkType;
    char str[MAXLEN];
    int line;

    struct Token *nextToken;

} Token;


// Global vars. Its better to include them in headers.
extern Token *tokenList;
extern int totalTokens;
extern int currentLine;

#endif