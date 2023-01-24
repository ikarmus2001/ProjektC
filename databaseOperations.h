#ifndef DATABASE_OPERATIONS_H
#define DATABASE_OPERATIONS_H

#include <gtk/gtk.h>
#include <stdio.h>
#include "Structures/structures.h"

unsigned char readDatabase(unsigned char, JakasStruktura**, size_t*);
// unsigned char readDatabase(unsigned char source, JakasStruktura** warehouseArray, size_t* warehouseLength);

#endif