#ifndef VIEW_CONTROLLER_H
#define VIEW_CONTROLLER_H

#include <gtk/gtk.h>
#include "Structures/structures.h"
#include "databaseOperations.h"

GtkWidget* getNewData(unsigned char);
GtkWidget* createNewTreeView(JakasStruktura*, size_t);
void addRow(GtkWidget*);
unsigned char saveData(unsigned char, GtkWidget*);

#endif //VIEW_CONTROLLER_H