#ifndef VIEW_CONTROLLER_H
#define VIEW_CONTROLLER_H

#include <gtk/gtk.h>
#include "Structures/structures.h"
#include "databaseOperations.h"

GtkTreeModel* getNewDataFromSource(unsigned char);
GtkWidget* createNewTreeView(JakasStruktura*, size_t);
void createAndShowAddRowEntry(GtkWidget*);
unsigned char saveData(unsigned char, GtkWidget*);
void searchItem(GtkWidget*);

#endif //VIEW_CONTROLLER_H