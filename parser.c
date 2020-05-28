/*  Built in headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Handcrafted headers */
#include "tokens.h"
#include "parser.h"
#include "lexer.h"

/* Variables */
Token token = {UNK, "UNK", 0};
FILE *f = NULL;


// This is where we start parsing things.
NonTerminal * parse(FILE *fPtr){
    f = fPtr;
    currentLine = 1; // extern from tokens.h

    // Reset reading process of .tpl file
    if (fseek(f, 0L, SEEK_SET) != 0 )
        printf("Error while rewinding the file for parser. Check .tpl file.");

    // Call the lexer first to recognize the tokens.
    token = lex(f);

    // Create a starting point for our parser
    NonTerminal *firstNt = NULL;
    firstNt = programBlock();

    // If end of file is reached we print that the process was successful
    if(token.tkType == EOFTK)
        printf("File parsed completely\n");

    // Print out the tree
    drawTree(firstNt, 0);

    return firstNt;
}

/* Non-terminal Blocks */

void * programBlock(){
    NonTerminal *nt = getNonTerminal(PROGRAMBLOCK);
    // A program block will consist 2 parts declarationPart for variables and statementPart for statements
    nt->child[0] = declarationPart();
    nt->child[1] = statementPart();

    // One final lex to see EOFTK
    token = lex(f);

    return nt;
}

void * declarationPart(){
    NonTerminal *nt = getNonTerminal(DECLARATIONPART);
    // Declaration part can be skipped as a whole
    if(token.tkType == VARS){
        token = lex(f);
        nt->child[0] = variableDeclaration();
    }

    return nt;
}

void * variableDeclaration(){
    NonTerminal *nt = getNonTerminal(VARIABLEDECLARATION);
    // If a type is known by our parser then it will proceed
    if(token.tkType == TYPEINT || token.tkType == TYPEFLT || token.tkType == TYPECHAR || token.tkType == TYPEBOOL ){
       // In case the variable is an array member we might require an extra <index-range> so we keep a counter
        int vd = 0;
        // a variable must have a type
        nt->child[vd] = type();

        vd++;
        token = lex(f);

        if(token.tkType == COLONS){
            token = lex(f);

            // a variable must have an identifier as a name
            nt->child[vd] = identifier();

            vd++;
            token = lex(f);

            // This indicated an array member
            if(token.tkType == LBRACE){

                // We only support char/int arrays
                if(nt->child[0]->token->tkType != TYPEINT)
                {
                    if(nt->child[0]->token->tkType == TYPECHAR)
                        printf("%s degiskeni icin karakter dizisi olustururken alt elemanlara dogrudan atama yapamazsiniz. (Ancak sonradan atama yapabilirsiniz. Satir: %d) ", nt->child[0]->token->str, nt->child[0]->token->line);
                    else
                    printf("%s dizi yapisi desteklenmemektedir. Satir: %d", nt->child[0]->token->str, nt->child[0]->token->line);

                    exit(1);
                }

                token = lex(f);

                // Used to determine member index
                nt->child[vd] = indexRange();

                // Array index can not be null
                if(nt->child[vd] == NULL)
                    syntaxError("expression");

                vd++;

                if(token.tkType == RBRACE)
                {
                    token = lex(f);

                }
            }

            if(token.tkType == COLONS){
                token = lex(f);

                nt->child[vd] = constant();
                vd++;

                // If the semicolon is missing fail the declaration
            if(token.tkType == SEMICOLON) {
                token = lex(f);

                // Predict a new variable declaration even if there are no more declarations in the source code
                nt->child[vd] = variableDeclaration();
                return nt;
            } else syntaxError(";"); // missing semicolon
        } else syntaxError(":"); // : missing between identifier and constant

        }else syntaxError(":"); // : missing between type and identifier

    }
    else{
        // If we are done with declarations return null
        nt = NULL;
        return nt;
    }

    return nt;
}


void * indexRange(){
    NonTerminal *nt = getNonTerminal(INDEXRANGE);
    // Index range can only be an integer
    if(token.tkType == LITINT)
    {
        insertTerminal(nt, token);
    }else syntaxError("sayi"); // input is not an integer

    token = lex(f);
    return nt;
}

void * statementPart(){
    NonTerminal *nt = getNonTerminal(STATEMENTPART);
    if(token.tkType == START){

        token = lex(f);
        // Start calling statements
        nt->child[0] = statement();

        if(token.tkType == END)
            return nt;
        else syntaxError("bitir"); // missing 'bitir'
    } else {
        syntaxError("basla"); // missing 'basla'
    }
    return nt;
}

void * statement(){
    NonTerminal *nt = getNonTerminal(STATEMENT);

    // It is a simplestatement
    if(token.tkType == ASSIGN) {
        nt->child[0] = simpleStatement();

        token = lex(f);
        if(token.tkType == SEMICOLON){
            token = lex(f);
            // Predict that there is another statement even if it is not provided in source code
            nt->child[1] = statement();
            return nt;
        }
        else
            syntaxError(";");
    }
    // If it is a structured statement
    else if(token.tkType == REPEAT || token.tkType == IF) {
        nt->child[0] = structuredStatement();

        token = lex(f);
        // Predict that there is another statement even if it is not provided in source code
        nt->child[1] = statement();

        return nt;
    }
    else {
        // If there are no more statements halt
        nt = NULL;
        return nt;
    }

    return nt;
}

// Simple state is currently only a buffer state for assignment. However we keep an extra case
// In case we need more simple-statements
void * simpleStatement(){
    NonTerminal *nt = getNonTerminal(SIMPLESTATEMENT);

    nt->child[0] = assignmentStatement();

    return nt;
}

void * assignmentStatement(){
    NonTerminal *nt = getNonTerminal(ASSIGNMENTSTATEMENT);

    token = lex(f);
    if(token.tkType == LPAR){
        token = lex(f);

        if(token.tkType == IDENT){
            nt->child[0] = variable();

            if(token.tkType == COMMA){

                token = lex(f);
                nt->child[1] = expression();

                if(token.tkType == RPAR)
                    return nt;
                else syntaxError(")"); // missing )
            }syntaxError(","); // missing ,
        }syntaxError("degisken"); // missing identifier
    }syntaxError("("); // missing (

    return nt;
}

void * structuredStatement(){
    NonTerminal *nt = getNonTerminal(STRUCTUREDSTATEMENT);

    // Determine what kind of a structuredStatement we have
    if(token.tkType == IF)
        nt->child[0] = ifStatement();
    else if(token.tkType == REPEAT)
        nt->child[0] = repeatStatement();

    return nt;
}

void * ifStatement(){
    NonTerminal *nt = getNonTerminal(IFSTATEMENT);

    token = lex(f);
    nt->child[0] = relativeExpression();

    token = lex(f);
    if(token.tkType != STATEMENTBEGIN) syntaxError("yap");

    token = lex(f);
    nt->child[1] = statement();

    // if statement is empty
    if(token.tkType == STATEMENTEND)
        return nt;

    if(token.tkType == ELSE){
        token = lex(f);
        nt->child[2] = statement();
    }

    if(token.tkType == STATEMENTEND)
        return nt;
    else syntaxError("cik");


    return nt;
}

void * repeatStatement(){
    NonTerminal *nt = getNonTerminal(REPEATSTATEMENT);

    token = lex(f);
    nt->child[0] = relativeExpression();

    token = lex(f);
    if(token.tkType != STATEMENTBEGIN) syntaxError("yap");

    token = lex(f);
    nt->child[1] = statement();

    if(token.tkType == STATEMENTEND)
        return nt;
    else syntaxError("cik");

    return nt;
}

// These are expression for checking relativity
void * relativeExpression(){
    NonTerminal *nt = getNonTerminal(RELATIVEEXPRESSION);

    insertTerminal(nt, token);

    if(token.tkType == LESSER || token.tkType == GREATER || token.tkType == GREATEREQ || token.tkType == LESSEREQ || token.tkType == EQUAL){
        token = lex(f);

        if(token.tkType != LPAR) syntaxError("(");

        token = lex(f);
        nt->child[0] = expression();

       if(token.tkType != COMMA) syntaxError(",");

        token = lex(f);
        nt->child[1] = expression();

        if(token.tkType != RPAR) syntaxError(")");

    } else syntaxError("karsilastirma operatoru"); // theres no relative operator

    return nt;
}

void * expression(){
    NonTerminal *nt = getNonTerminal(EXPRESSION);

    // if theres is a mathematical expression
     if(token.tkType == SUB ||token.tkType == ADD || token.tkType == MUL || token.tkType == DIV ){
        insertTerminal(nt, token);
        token = lex(f);
        if(token.tkType == LPAR){

            token = lex(f);
            nt->child[0] = expression();

            if(token.tkType == COMMA){
                token = lex(f);
            }else syntaxError(",");

            nt->child[1] = expression();

            if(token.tkType == RPAR) {
               token = lex(f);
                return nt;
            }
            else syntaxError(")"); //missing )
        }
    }
    else if(token.tkType == IDENT || token.tkType == STRING || token.tkType == LITFLT || token.tkType == LITINT || token.tkType == TRUE || token.tkType == FALSE) {
        nt->child[0] = termFactor();
    } else{
        nt = NULL;
        return nt;
    }

    return nt;
}

void * not(){
    NonTerminal *nt = getNonTerminal(NOTSTATE);

    if(token.tkType == NOT){
        insertTerminal(nt, token);

        token = lex(f);
    }else{
        nt=NULL;
    }

    return nt;
}

void * termFactor() {
    NonTerminal *nt = getNonTerminal(TERMFACTOR);

        if(token.tkType == IDENT){
            // If theres an id return early
            nt->child[0] = variable();

            nt->child[1] = not();
            return nt;
        }
        else // If it is not an identifier it is a constant for us. Still push 1 more token because variable() will.
        {
            nt->child[0] = constant();
            if(nt->child[0]->token->tkType == TRUE || nt->child[0]->token->tkType == FALSE)
            nt->child[1] = not();

        }
        return nt;
}

void * constant(){
    NonTerminal *nt = getNonTerminal(CONSTANT);
    // Insert constant terminal

    insertTerminal(nt, token);
    token = lex(f);

    return nt;
}

void * variable(){
    NonTerminal *nt = getNonTerminal(VARIABLE);


    nt->child[0] = identifier();

    // If it is not an array it returns
    token = lex(f);

    if(token.tkType == LBRACE){
        token = lex(f);

        nt->child[1] = indexRange();

        if(nt->child[1] == NULL)
            syntaxError("sayi"); // array index cant be empty

        if(token.tkType != RBRACE)syntaxError("]");

        token = lex(f);
    }


    return nt;
}

void * identifier(){
    NonTerminal *nt = getNonTerminal(IDENTIFIER);

    // insert variable name as a terminal
    insertTerminal(nt, token);

    return nt;
}

void * type(){
    NonTerminal *nt = getNonTerminal(TYPE);
    //insert type as a terminal
    insertTerminal(nt, token);

    return nt;
}

/* These are helper functions to create our linked list styled tree*/

NonTerminal * getNonTerminal(NtType ntt){
    NonTerminal *tempNt;

    tempNt = (NonTerminal *) malloc(sizeof(NonTerminal));

    tempNt->ntType = ntt;
    tempNt->token = NULL;

    for(int i=0; i < CHILDRENMAX; i++)
        tempNt->child[i] = NULL;

    return tempNt;
}

Token * getTokenPointer(Token tk){
    Token *tempToken = (Token *) malloc(sizeof(Token));
    strcpy(tempToken->str, tk.str);
    tempToken->line = tk.line;
    tempToken->tkType = tk.tkType;

    return tempToken;
}

void insertTerminal(NonTerminal *nt, Token tk){
    Token *tkPointer = getTokenPointer(tk);

    if(nt->token == NULL)
        nt->token = tkPointer;
    else{
        Token *t = nt->token;
        while(t->nextToken != NULL)
            t = t->nextToken;

        t->nextToken = tkPointer;
    }

}

/* Tree */

void drawTree(NonTerminal *nt, int lv)
{
    if(nt == NULL) return;

    char strTree[100];

    printf("%*s" "%d %s", lv * 4, " ", lv, getNonTerminalName(nt, strTree));

    if(nt->child[0] == NULL || nt->ntType == EXPRESSION){ // Terminals have no children
    Token *tk = nt->token;

    // If theres a terminal inside then token can not be null
        if(nt->token != NULL){
        printf("\n      %*s" "%s (%s)", lv*4, " ", tk->str, getTokenType(*tk, strTree) );

         }
    }

    printf("\n");

   for(int i=0; i < CHILDRENMAX; i++)
      drawTree(nt->child[i], lv+1);
}

void syntaxError(char * expected){
        if(token.tkType == EOFTK) {
            printf("%d SATIRINDA SYNTAX HATASI:  bitir tokeni girilmemis. Dosyanin sonu beklenenden erken geldi.  (End of File Token)", currentLine);
            exit(0);
        }

        printf("%d SATIRINDA SYNTAX HATASI: Beklenen token ' %s ' yerine gelen ' %s '", currentLine, expected, token.str);
        exit(0);
}

char * getNonTerminalName(NonTerminal *nt, char *str){
    switch(nt->ntType){
        // Values themselves
        case PROGRAMBLOCK: strcpy(str, "<program-block>"); break;
        case DECLARATIONPART: strcpy(str, "<declaration-part>"); break;
        case VARIABLEDECLARATION: strcpy(str, "<variable-declaration>"); break;
        case IDENTIFIER: strcpy(str, "<identifier>"); break;
        case TYPE: strcpy(str, "<type>"); break;
        case STATEMENTPART: strcpy(str, "<statement-part>"); break;
        case STATEMENT: strcpy(str, "<statement>"); break;
        case SIMPLESTATEMENT: strcpy(str, "<simple-statement>"); break;
        case ASSIGNMENTSTATEMENT: strcpy(str, "<assignment-statement>"); break;
        case EXPRESSION: strcpy(str, "<expression>"); break;
        case TERMFACTOR: strcpy(str, "<term-factor>"); break;
        case STRUCTUREDSTATEMENT: strcpy(str, "<structured-statement>"); break;
        case RELATIVEEXPRESSION: strcpy(str, "<relative-expression>"); break;
        case IFSTATEMENT: strcpy(str, "<if-statement>"); break;
        case REPEATSTATEMENT: strcpy(str, "<repeat-statement>"); break;
        case VARIABLE: strcpy(str, "<variable>"); break;
        case INDEXRANGE: strcpy(str, "<index-range>"); break;
        case CONSTANT: strcpy(str, "<constant>"); break;
        case NOTSTATE: strcpy(str, "<not>"); break;

        default: strcpy(str, "ERROR: UNKNOWN NON-TERMINAL");
    }
    return str;
}
