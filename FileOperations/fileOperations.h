#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "../Structures/structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

JakasStruktura* getDataFromFile(size_t*);

char saveToFile(char*, JakasStruktura*, size_t);

#endif //FILE_OPERATIONS_H