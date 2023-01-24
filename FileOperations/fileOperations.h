#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "../Structures/structures.h"
#include <stdio.h>
#include <gtk/gtk.h>

unsigned char readFromFile(JakasStruktura* structuresArray, size_t* rowsRead);

#endif //FILE_OPERATIONS_H