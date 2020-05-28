/*
 * This file basically combines everything into a single compiler file
 */


#include <stdio.h>
#include <stdlib.h>


#include "tokens.h"
#include "lexer.h"
#include "parser.h"
#include "semantic.h"

// Runner combines file management, lexer and the parser.
int main(int argc, char *argv[]){


    // We included a CMakeLists that automatically imports .tpl files so you can use relative paths
    // to call files e.g examples/filename.tpl so it can be tested easily.
    /* FILE LOCATION GOES HERE */
    FILE *f = fopen("examples/goodexample1.tpl", "r");



    /* LEXER */

    printf("*** LEXER ***\n\n");

    // Allocate memory for our array. later to be reallocated.
      tokenList = (Token *) malloc(totalTokens * sizeof(Token));

    do{
        totalTokens++;

        // If array size changes reallocate memory for a bigger one
        // It is a bad practice to dynamically allocate with each token but it is for convenience. Normally it should be avoided.
        tokenList = (Token *) realloc(tokenList, totalTokens * sizeof(Token));
        tokenList[totalTokens-1] = lex(f);

        printToken(tokenList[totalTokens-1]);

        if(tokenList[totalTokens-1].tkType == UNK)
        {
            printf("MALFORMED TOKEN. LEXER WILL EXIT NOW.");
            exit(0);
        }
    }
    while(tokenList[totalTokens-1].tkType != EOFTK);



   /* PARSER */

    printf("\n\n*** PARSER ***\n\n");

   // Parser will automatically draw its' own parse tree.
    NonTerminal *t = parse(f);

    /* Semantic Analyzer */

    printf("\n\n*** SEMANTIC ANALYZER ***\n\n");
    // Analyze the parse tree semantically
    semanticAnalyze(t);

    return 0;

}
