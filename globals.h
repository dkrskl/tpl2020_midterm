#ifndef GLOBALS_H
#define GLOBALS_H

#define KEYWORDLEN 25
#define STRINGLEN 100

// Keywords cant be for other means. They are reserved
char *keywords[KEYWORDLEN] = {"basla", "bitir", "eger", "yap" , "cik", "yoksa", "tekrarla", "degiskenler",
                              "sayi", "ondalik", "ikili", "karakter", "dogru", "yanlis",
                              "topla", "esitle", "cikar", "bol","carp", "tersi",
                              "buyukse", "kucukse", "buyukesitse", "kucukesitse", "esitse"};

// It will be challenging to determine whether a "word" is an ID or KEYW. So we store them as words for now. Later to be figured out what they are.
char words[MAXLEN];
int w = 0; // index of words array

// Same as words just for numbers this time
char nums[MAXLEN];
int n = 0;

// An array for strings recognition only.
char strs[STRINGLEN];
int s = 0;

#endif
