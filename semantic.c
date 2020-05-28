/*  Built in headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Handcrafted headers */
#include "tokens.h"
#include "parser.h"
#include "semantic.h"
#include "lexer.h"

/* Variables */
int mapIndex = 0;
int mapMultiplier = 1;


// Just a transition function to pass values to semantic()
void semanticAnalyze(NonTerminal *nt){
    varMap = (Var *) malloc(sizeof(Var) * VARMAP);

    semantic(nt);
}

// This is where the semantic analysis happens
void semantic(NonTerminal *nt) {
    if (nt == NULL) return;

    char tmpStr[100];

    switch (nt->ntType) {

        case VARIABLEDECLARATION: { // We scope cases because the whole switch statement is in the same scope. (Or it will require permanent vars)

            Token *tkRight;
            Token *tkLeft = nt->child[0]->token;
            // Termfactor could be on 2 or 3 depending if the variable is an array or not
            if (nt->child[2]->ntType == CONSTANT) {
                tkRight = nt->child[2]->token;
            } else tkRight = nt->child[3]->token;

            // Integer
            if (tkLeft->tkType == TYPEINT) {
                if (tkRight->tkType != LITINT)
                    semanticError("sayi", getTokenType(*tkRight, tmpStr), tkRight->line);
            }
                // Float
            else if (tkLeft->tkType == TYPEFLT) {
                if (tkRight->tkType != LITFLT)
                    semanticError("ondalik", getTokenType(*tkRight, tmpStr), tkRight->line);
            }
                // Bool
            else if (tkLeft->tkType == TYPEBOOL) {
                if (tkRight->tkType != TRUE && tkRight->tkType != FALSE)
                    semanticError("ikili", getTokenType(*tkRight, tmpStr), tkRight->line);
            }
                // Char
            else if (tkLeft->tkType == TYPECHAR) {
                if (tkRight->tkType != STRING)
                    semanticError("karakter", getTokenType(*tkRight, tmpStr), tkRight->line);
            }


                // Check the varmap to see if the variable is already declared
                for (int i = 0; i < mapIndex; i++) {
                    // Var already declared
                    if (stricmp(varMap[i].name, nt->child[1]->token->str) == 0) {
                        if (nt->child[2]->ntType == INDEXRANGE && tkLeft->tkType != TYPECHAR) {
                            // If it is an array member check the index
                            if (varMap[i].index == atoi(nt->child[2]->token->str)) {
                                printf("' %s ' dizisinin bu elemani daha once tanimlanmis.", varMap[i].name);
                                exit(1);
                            }
                        } else {
                            printf("Degisken ' %s ' daha once tanimlanmis.", varMap[i].name);
                            exit(1);
                        }
                    }
                }



            // Create a temporary variable to be hold in varmap these are for ints/int arrays/floats/bools
            Var tmp;
            tmp.type = tkLeft->tkType;
            strcpy(tmp.name, nt->child[1]->token->str);
            if (nt->child[2]->ntType == INDEXRANGE && tkLeft->tkType != TYPECHAR)
                tmp.index = atoi(nt->child[2]->token->str);
            else
                tmp.index = -1;
            strcpy(tmp.data, tkRight->str);


            varMap[mapIndex++] = tmp;

                // If there are no more spaces in our varmap array increase its size by VARMAP amount
                if (mapIndex % VARMAP == 0) {
                    mapMultiplier++;
                    varMap = (Var*) realloc(varMap, VARMAP * mapMultiplier * sizeof(Var));

                }




            break;
        }

        // If theres an assignment statement
        case ASSIGNMENTSTATEMENT: {
            Var tLeft = getVar(nt->child[0]);
            Var tRight = getExpression(nt->child[1]);


            if (tLeft.type != tRight.type) {
                printf("Uyumsuz atama islemi.");
                exit(1);
            }

            strcpy(tLeft.data, tRight.data);

            if(setVar(tLeft))
                printf("%s degiskeni icerigi %s olarak degistirildi.\n", tLeft.name, tLeft.data);


        }
            break;


        // Relative expressions
        case RELATIVEEXPRESSION: {
            Var tLeft = getExpression(nt->child[0]);
            Var tRight = getExpression(nt->child[1]);

            if (tLeft.type != tRight.type) {
                printf("Uyumsuz kiyaslama islemi.\n");
                exit(1);
            }

            //esitse()
            if (nt->token->tkType == EQUAL) {
            if (strcmp(tLeft.data, tRight.data) == 0)
                printf("%s ve %s esit.\n", tLeft.data, tRight.data);
            else
                printf("%s ve %s esit DEGIL.\n", tLeft.data, tRight.data);
        }
                // kucukse()
            else if(nt->token->tkType == LESSER)
            {
                if(tLeft.type == TYPEINT){
                    if(atoi(tLeft.data) < atoi(tRight.data))
                        printf("%s %s dan kucuk.\n", tLeft.data, tRight.data);
                    else
                        printf("%s %s dan kucuk DEGIL.\n", tLeft.data, tRight.data);
                }
                else if(tLeft.type == TYPEFLT){
                    if(atof(tLeft.data) < atof(tRight.data))
                        printf("%s %s dan kucuk.\n", tLeft.data, tRight.data);
                    else
                        printf("%s %s dan kucuk DEGIL.\n", tLeft.data, tRight.data);
                } else
                { printf("%s ve %s kiyaslanamaz. (Bu operator icin sadece sayi ve ondalik desteklenmektedir.)\n", tLeft.data, tRight.data);
                    exit(1);
                }
            }

            // buyukse()
            else if(nt->token->tkType == GREATER)
            {
                if(tLeft.type == TYPEINT){
                    if(atoi(tLeft.data) > atoi(tRight.data))
                        printf("%s %s dan buyuk.\n", tLeft.data, tRight.data);
                    else
                        printf("%s %s dan buyuk DEGIL.\n", tLeft.data, tRight.data);
                }
                else if(tLeft.type == TYPEFLT){
                    if(atof(tLeft.data) > atof(tRight.data))
                        printf("%s %s dan buyuk.\n", tLeft.data, tRight.data);
                    else
                        printf("%s %s dan buyuk DEGIL.\n", tLeft.data, tRight.data);
                } else{
                    printf("%s ve %s kiyaslanamaz. (Bu operator icin sadece sayi ve ondalik desteklenmektedir.)\n", tLeft.data, tRight.data);
                exit(1);
                }
            }

            else if(nt->token->tkType == GREATEREQ)
            {
                if(tLeft.type == TYPEINT){
                    if(atoi(tLeft.data) >= atoi(tRight.data))
                        printf("%s %s dan buyuk veya esit .\n", tLeft.data, tRight.data);
                    else
                        printf("%s %s dan buyuk veya esit DEGIL.\n", tLeft.data, tRight.data);
                }
                else if(tLeft.type == TYPEFLT){
                    if(atof(tLeft.data) >= atof(tRight.data))
                        printf("%s %s dan buyuk veya esit.\n", tLeft.data, tRight.data);
                    else
                        printf("%s %s dan buyuk veya esit DEGIL.\n", tLeft.data, tRight.data);
                } else{
                    printf("%s ve %s kiyaslanamaz. (Bu operator icin sadece sayi ve ondalik desteklenmektedir.)\n", tLeft.data, tRight.data);
                    exit(1);
                }
            }

            else if(nt->token->tkType == LESSEREQ)
            {
                if(tLeft.type == TYPEINT){
                    if(atoi(tLeft.data) <= atoi(tRight.data))
                        printf("%s %s dan kucuk veya esit .\n", tLeft.data, tRight.data);
                    else
                        printf("%s %s dan kucuk veya esit DEGIL.\n", tLeft.data, tRight.data);
                }
                else if(tLeft.type == TYPEFLT){
                    if(atof(tLeft.data) <= atof(tRight.data))
                        printf("%s %s dan kucuk veya esit.\n", tLeft.data, tRight.data);
                    else
                        printf("%s %s dan kucuk veya esit DEGIL.\n", tLeft.data, tRight.data);
                } else{
                    printf("%s ve %s kiyaslanamaz. (Bu operator icin sadece sayi ve ondalik desteklenmektedir.)\n", tLeft.data, tRight.data);
                    exit(1);
                }
            }


        }
            break;


    }

    // Semantically analyze the children and their children and so on
    for (int i = 0; i < CHILDRENMAX; i++)
        semantic(nt->child[i]);
}

// Delve deep into the expression NonTerminal and bring their types back
Var getExpression(NonTerminal *nt){
    // If the expression is a math operation
    if (nt->token != NULL) {
        Var tLeft = getExpression(nt->child[0]);
        Var tRight = getExpression(nt->child[1]);
        Var tmp;

        if(tLeft.type == TYPEBOOL)
        {
            printf("ikili degiskenler uzerinde matematik atamalari desteklenmiyor. Sadece kiyaslama yapabilirsiniz.");
            exit(1);
        }

        if(tRight.type == STRING)
            tRight.type = TYPECHAR;

        if (tLeft.type != tRight.type){
            printf("Uyumsuz matematik islemi.\n");
            exit(1);
        }

        if(nt->token->tkType == ADD){
            // Add integers
            if(tLeft.type == TYPEINT){
                tmp.type = TYPEINT;

                char buffer[MAXLEN];
                // first decompile strings into ints then compile back into strings
                itoa(atoi(tLeft.data) + atoi(tRight.data), buffer, 10);
                strcpy(tmp.data, buffer);

            }
            // Add floats
            else if(tLeft.type == TYPEFLT){
                tmp.type = TYPEFLT;

                char buffer[MAXLEN];
                // float to str
                gcvt(atof(tLeft.data) + atof(tRight.data), 10, buffer);
                strcpy(tmp.data, buffer);


            }
            // String catenation
            if(tLeft.type == TYPECHAR)
            {
                strcpy(tmp.data, strcat(tLeft.data, tRight.data));
                tmp.type = TYPECHAR;
            }
        }

        else if(nt->token->tkType == SUB){
            if(tLeft.type == TYPECHAR)
            {
                printf("karakter degiskenleri uzerinde sadece toplama (katenasyon) islemi yapilabilir.");
                exit(1);
            }
            // sub integers
            if(tLeft.type == TYPEINT){
                tmp.type = TYPEINT;

                char buffer[MAXLEN];
                // first decompile strings into ints then compile back into strings
                itoa(atoi(tLeft.data) - atoi(tRight.data), buffer, 10);
                strcpy(tmp.data, buffer);

            }
            // sub floats
            else if(tLeft.type == TYPEFLT){
                tmp.type = TYPEFLT;

                char buffer[MAXLEN];
                // float to str
                gcvt(atof(tLeft.data) - atof(tRight.data), 10, buffer);
                strcpy(tmp.data, buffer);


            }

        }

        else if(nt->token->tkType == MUL){
            if(tLeft.type == TYPECHAR)
            {
                printf("karakter degiskenleri uzerinde sadece toplama (katenasyon) islemi yapilabilir.");
                exit(1);
            }
            // multiply integers
            if(tLeft.type == TYPEINT){
                tmp.type = TYPEINT;

                char buffer[MAXLEN];
                // first decompile strings into ints then compile back into strings
                itoa(atoi(tLeft.data) * atoi(tRight.data), buffer, 10);
                strcpy(tmp.data, buffer);

            }
                // multiply floats
            else if(tLeft.type == TYPEFLT){
                tmp.type = TYPEFLT;

                char buffer[MAXLEN];
                // float to str
                gcvt(atof(tLeft.data) * atof(tRight.data), 10, buffer);
                strcpy(tmp.data, buffer);


            }

        }

        else if(nt->token->tkType == DIV){
            if(tLeft.type == TYPECHAR)
            {
                printf("karakter degiskenleri uzerinde sadece toplama (katenasyon) islemi yapilabilir.");
                exit(1);
            }
            // divide integers
            if(tLeft.type == TYPEINT){
                tmp.type = TYPEINT;

                char buffer[MAXLEN];
                // first decompile strings into ints then compile back into strings
                itoa(atoi(tLeft.data) / atoi(tRight.data), buffer, 10);
                strcpy(tmp.data, buffer);

            }
                // divide floats
            else if(tLeft.type == TYPEFLT){
                tmp.type = TYPEFLT;

                char buffer[MAXLEN];
                // float to str
                gcvt(atof(tLeft.data) / atof(tRight.data), 10, buffer);
                strcpy(tmp.data, buffer);


            }

        }


        return tmp;
    } // math operation related END


    else if(nt->child[0]->ntType == TERMFACTOR){
        if(nt->child[0]->child[0]->ntType == VARIABLE){
            // Check if it is a boolean and has tersi after it
            Var tmp;
            tmp = getVar(nt->child[0]->child[0]);

            if(nt->child[0]->child[1] != NULL)
            {
                if(tmp.type != TYPEBOOL) {
                    printf("%s bir ikili degiskeni degil. tersi yapisini desteklemiyor.", tmp.name);
                    exit(1);
                }

                if(strcmp(tmp.data, "dogru") == 0 )
                    strcpy(tmp.data, "yanlis");
                    else
                    strcpy(tmp.data, "dogru");
            }

            return tmp;
        }
        else if(nt->child[0]->child[0]->ntType == CONSTANT){
            Var tmp; //Create a temporary var for comparison at expression level
            strcpy(tmp.data, nt->child[0]->child[0]->token->str);



            switch(nt->child[0]->child[0]->token->tkType)
            {
                case LITINT: tmp.type = TYPEINT; break;
                case LITFLT: tmp.type = TYPEFLT; break;
                case STRING: tmp.type = TYPECHAR; break;

                case TRUE: tmp.type = TYPEBOOL;
                if(nt->child[0]->child[1] != NULL)
                    strcpy(tmp.data, "yanlis");
                    break;
                case FALSE: tmp.type = TYPEBOOL;
                    if(nt->child[0]->child[1] != NULL)
                        strcpy(tmp.data, "dogru");
                    break;
            }


            return tmp;
        }
    }
    // Only math related expressions have terminals inside them
    else if(nt->child[0]->ntType == EXPRESSION && nt->child[1]->ntType == EXPRESSION ){
            Var tLeft = getExpression(nt->child[0]);
            Var tRight= getExpression(nt->child[1]);
            if(tLeft.type == tRight.type)
            {
                if(nt->token == NULL) {
                    printf("Matematik operatoru bulunamadi.");
                    exit(1);
                }
            }
    }

}

// Locate the var in varMap and return its type as TokenType
Var getVar(NonTerminal * nt) {
    for(int i = 0; i < mapIndex; i++)
    {
        // Var already declared
        if(stricmp(varMap[i].name,nt->child[0]->token->str) == 0)
        {
            if(nt->child[1] != NULL)
            {   // If it is an array member check the index
                if(varMap[i].type == TYPECHAR)
                {
                    Var tmp;
                    tmp = varMap[i];
                    char tmpChar[2];
                    tmpChar[0] = varMap[i].data[atoi(nt->child[1]->token->str)];
                    tmpChar[1] = '\0';
                    strcpy(tmp.data, tmpChar);

                    return tmp;
                }
                else if(varMap[i].index == atoi(nt->child[1]->token->str)){

                    return varMap[i];
                }
            }else{
                return varMap[i];
            }
        }
    }

    if(nt->child[1] != NULL)
        printf("Bu dizi elemani %s dizisinde tanimli degil. Satir: %d", nt->child[0]->token->str, nt->child[0]->token->line);
    else
        printf("Degisken %s tanimli degil. Satir: %d", nt->child[0]->token->str, nt->child[0]->token->line);

    exit(1);
}

int setVar(Var var){
    for(int i = 0; i < mapIndex; i++)
    {
        if(strcmp(var.name, varMap[i].name) == 0 ){
            varMap[i] = var;
            return 1;
        }
    }

    return 0;
}


// Print out errors
void semanticError(char * expected, char * entered, int line){
    printf("Semantic Analiz Hatasi: %s ile %s tipi uyumlu degil. Satir: %d\n", expected, entered, line);
    exit(1);
}


