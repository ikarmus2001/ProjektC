#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "../Structures/structures.h"
#include <stdio.h>
#include <gtk/gtk.h>

unsigned char getDataFromFile(JakasStruktura**, size_t*);
// unsigned char getDataFromFile(JakasStruktura* structuresArray, size_t* rowsRead);
char saveToFile(char*, JakasStruktura*, size_t);
// char saveToFile(char* path, JakasStruktura* structsArray, size_t arrayLength);

#endif //FILE_OPERATIONS_H