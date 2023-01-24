#ifndef VIEW_CONTROLLER_H
#define VIEW_CONTROLLER_H

#include <gtk/gtk.h>
#include "Structures/structures.h"
#include "FileOperations/fileOperations.h"

unsigned char readDatabase(unsigned char source, GtkWidget* newTreeView);

#endif //VIEW_CONTROLLER_H