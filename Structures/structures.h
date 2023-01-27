#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>

typedef struct cokolwiek {
    size_t id;
    char* nazwa;
    int ilosc;
    float wartosc;
    signed char stan;
} JakasStruktura;

typedef struct searchedString {
    char* string;
    size_t length;
    size_t* ids;
    size_t maxIdsLength;
} SearchedString;

#endif