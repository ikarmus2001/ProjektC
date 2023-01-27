#ifndef DATABASE_OPERATIONS_H
#define DATABASE_OPERATIONS_H

#include <gtk/gtk.h>
#include <stdio.h>
#include "Structures/structures.h"

JakasStruktura* readDatabase(unsigned char, size_t*);
unsigned char saveDatabase(unsigned char, JakasStruktura*, size_t);
void saveToFileManuallyLink(char*, int, float, FILE*);
void deleteRow(GtkWidget*);

#endif //DATABASE_OPERATIONS_H