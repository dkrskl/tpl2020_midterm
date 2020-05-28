/*  Built in headers */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* Handcrafted headers */
#include "tokens.h"
#include "lexer.h"
#include "globals.h"

/* Variables */

// These are extern vars. from "tokens.h"
Token *tokenList = NULL;
int totalTokens = 0;
int currentLine = 1;

/* Main function to lex everything. Returns a "TokenType" and we can determine what it is. */

Token lex(FILE *fPtr){

    Token token;
    int c;


    // Loop through .tpl file character by character.
    while((c = fgetc(fPtr)) != EOF) {
        // New lines dont mean much to our code but they help us determine which line the token lies. Therefore we increase line number if we see a new line.
        if (c == '\n') currentLine++;

        // Comments are stylized as !!comment
        // Comments are there for the end user only so lexer will skip them entirely.
        if (c == '!') {
             if ((c = fgetc(fPtr)) == '!') {
                 while ((c = fgetc(fPtr)) != '\n');
                  currentLine++;
             } else {
                 printf("Lex Hatasi: Dogru yorum stili !!yorum seklindedir. Satir: %d", currentLine);
                 exit(0);
             }
        }
        // An alphabet character is being streamed from input. Then it is definitely an identifier.
        if(isalpha(c)){
            words[w++] = c;

            // Accept numbers and _ after the first letter
            while(isalnum(c = fgetc(fPtr)) || c == '_' )
             words[w++] = c;


            words[w] = '\0';
            w = 0;

            strcpy(token.str, words);


            // Once it is completed read determine if it is a keyword or an identifier.
            if(isKeyword(token.str))
                token.tkType = recKeyword(token.str);
            else
                token.tkType = IDENT;



            token.line = currentLine;

            // Go back 1 character to read \n s properly.
            fseek(fPtr, -1, SEEK_CUR);

            return token;

        } //end isalpha

        // If a digit comes in first then it is some form of a literal.
        else if(isdigit(c)){
            int dotCounter = 0;

            nums[n++] = c;

            while(isdigit(c = fgetc(fPtr)) || (c == '.')){
                nums[n++] = c;

                if(c == '.')
                    dotCounter++;

            }



            nums[n] = '\0';
            n=0;


            strcpy(token.str, nums);
            if(dotCounter == 1)
                token.tkType = LITFLT;
            else if(dotCounter == 0)
                token.tkType = LITINT;
            else
                token.tkType = UNK;

            token.line = currentLine;

            fseek(fPtr, -1, SEEK_CUR);

            return token;

        } // end isdigit

        // Mainly a check for delimiters.
        else if(ispunct(c)){
            // String check. It will skip quotation marks.
                    if(c == '"'){
                        while(isprint(c = fgetc(fPtr)) && c != '"')
                        {
                            if(c != '"')
                            strs[s++] = c;
                        }

                        strs[s] = '\0';
                        s = 0;
                        token.tkType = STRING;
                        token.line = currentLine;
                        strcpy(token.str, strs);


                        return token;

                    } // if quote
                    // Check for other delimiters.
                    else {

                        token.str[0] = c;
                        token.str[1] = '\0';
                        token.tkType = recOperator(c);
                        token.line = currentLine;

                        return token;
                    } // if other delimiters


        }



    } //end while

    // If it doesnt fit to any criteria. Return EOFTK and end lexing process.
    token.tkType = EOFTK;
    return token;

}



// Checking for operators.
TokenType recOperator(char c){
    switch(c)
    {
        case ';': return SEMICOLON; break;
        case ':': return COLONS; break;
        case '(': return LPAR; break;
        case ')': return RPAR; break;
        case '[': return LBRACE; break;
        case ']': return RBRACE; break;
        case '"': return QUOTE; break;
        case ',': return COMMA; break;
        case '.': return DOT; break;

        default:
            return UNK;
    }
}


// More end user friendly token type names
char *getTokenType(Token tk, char *str){
    switch(tk.tkType){
        // Values themselves
        case IDENT: strcpy(str, "IDENTIFIER"); break;
        case LITINT: strcpy(str, "LITERAL (SAYI)"); break;
        case LITFLT: strcpy(str, "LITERAL (ONDALIK)"); break;
        case STRING: strcpy(str, "METIN");break;
        case TRUE: strcpy(str, "DOGRU"); break;
        case FALSE: strcpy(str, "YANLIS");break;

        // Types
        case TYPEINT: strcpy(str, "SAYI (TYPE)"); break;
        case TYPEFLT: strcpy(str, "ONDALIK (TYPE)"); break;
        case TYPEBOOL: strcpy(str, "IKILI (TYPE)");break;
        case TYPECHAR: strcpy(str, "KARAKTER (TYPE)");break;

        //Keywords
        case START: strcpy(str, "START (KEYWORD)"); break;
        case END: strcpy(str, "END (KEYWORD)");break;
        case IF: strcpy(str, "IF (KEYWORD)");break;
        case ELSE: strcpy(str, "ELSE (KEYWORD)");break;
        case REPEAT: strcpy(str, "REPEAT(KEYWORD)");break;
        case ADD: strcpy(str, "ADDITION (KEYWORD)");break;
        case SUB: strcpy(str, "SUBTRACTION (KEYWORD)");break;
        case MUL: strcpy(str, "MULTIPLICATION (KEYWORD)");break;
        case DIV: strcpy(str, "DIVISION (KEYWORD)");break;
        case ASSIGN: strcpy(str, "ASSIGNMENT (KEYWORD)");break;
        case NOT: strcpy(str, "NOT (KEYWORD)");break;
        case VARS: strcpy(str, "VARIABLES BLOCK (KEYWORD)");break;
        case STATEMENTBEGIN: strcpy(str, "STATEMENTBEGIN (KEYWORD)");break;
        case STATEMENTEND: strcpy(str, "STATEMENTEND (KEYWORD)");break;

        //Delimiters
        case SEMICOLON: strcpy(str, "SEMICOLON (LINE ENDING)");break;
        case COLONS: strcpy(str, "COLONS (TYPE IDENTIFIER OP)");break;
        case LPAR: strcpy(str, "LEFT PAR ");break;
        case RPAR: strcpy(str, "RIGHT PAR ");break;
        case LBRACE: strcpy(str, "LEFT BRACE ");break;
        case RBRACE: strcpy(str, "RIGHT BRACE ");break;
        case COMMA: strcpy(str, "COMMA ");break;
        case DOT: strcpy(str, "DOT ");break;

        //Relational
        case GREATER: strcpy(str, "GREATER (RELATIONAL OP) ");break;
        case LESSER: strcpy(str, "LESSER (RELATIONAL OP) ");break;
        case GREATEREQ: strcpy(str, "GREATER EQUAL (RELATIONAL OP) ");break;
        case LESSEREQ: strcpy(str, "LESSER EQUAL (RELATIONAL OP) ");break;
        case EQUAL: strcpy(str, "EQUALS (RELATIONAL OP) ");break;

        default: strcpy(str, "UNKNOWN TYPE");
    }
    return str;
}

int isKeyword(char *str){
    // We dont cares about case sensitivity. We directly compare our strings.
    // KEYWORDLEN is defined at globals.h
    for(int i = 0; i < KEYWORDLEN; i++)
        if(stricmp(str, keywords[i]) == 0)
            return 1;

        return 0;
}


// Determines the token type from the enum list
TokenType recKeyword(char *str){
    if(stricmp(str, "basla") == 0) return START;
    else if(stricmp(str, "bitir") == 0) return END;
    else if(stricmp(str, "eger") == 0) return IF;
    else if(stricmp(str, "yoksa") == 0) return ELSE;
    else if(stricmp(str, "eger") == 0) return IF;
    else if(stricmp(str, "yap") == 0) return STATEMENTBEGIN;
    else if(stricmp(str, "cik") == 0) return STATEMENTEND;
    else if(stricmp(str, "tekrarla") == 0) return REPEAT;
    else if(stricmp(str, "degiskenler") == 0) return VARS;
    else if(stricmp(str, "tersi") == 0) return NOT;

    else if(stricmp(str, "esitle") == 0) return ASSIGN;
    else if(stricmp(str, "cikar") == 0) return SUB;
    else if(stricmp(str, "carp") == 0) return MUL;
    else if(stricmp(str, "bol") == 0) return DIV;
    else if(stricmp(str, "topla") == 0) return ADD;

    else if(stricmp(str, "esitse") == 0) return EQUAL;
    else if(stricmp(str, "kucukse") == 0) return LESSER;
    else if(stricmp(str, "buyukse") == 0) return GREATER;
    else if(stricmp(str, "buyukesitse") == 0) return GREATEREQ;
    else if(stricmp(str, "kucukesitse") == 0) return LESSEREQ;


    else if(stricmp(str, "sayi") == 0) return TYPEINT;
    else if(stricmp(str, "ondalik") == 0) return TYPEFLT;
    else if(stricmp(str, "karakter") == 0) return TYPECHAR;
    else if(stricmp(str, "ikili") == 0) return TYPEBOOL;


    else if(stricmp(str, "dogru") == 0) return TRUE;
    else if(stricmp(str, "yanlis") == 0) return FALSE;

    return UNK;

}


// Print your token to be readable
void printToken(Token tk){
    if(tk.tkType == EOFTK) {
        printf("End Of File Token");
        return;
    }

    char tokenStr[100];

    printf("%s tokeni %d satirinda turu: %s\n", tk.str, tk.line, getTokenType(tk, tokenStr));

}
