#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "../Structures/structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

JakasStruktura* getDataFromFile(size_t*);
char saveToFile(JakasStruktura*, size_t);
void saveToFileManually(char*, int*, float*, FILE*);

char* selectFile(GtkFileChooserAction);

#endif //FILE_OPERATIONS_H