#include "ViewController.h"
#include "FileOperations/fileOperations.h"
#include <gtk/gtk.h>

// Delegates
unsigned char readDatabase(unsigned char source, GtkWidget* newTreeView) {
    JakasStruktura* js;
    size_t* rowsRead;
    unsigned char result = retrieveData(&js, &rowsRead);
    if (result != 0)
        return result;
    
    newTreeView = createNewTreeView(js, rowsRead);

    return 0;
}


GtkWidget* createNewTreeView(JakasStruktura js[], size_t rowsRead) {
    GtkTreeModel* store = GTK_TREE_MODEL(gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_INT, G_TYPE_FLOAT));
    GtkWidget* newTreeView = gtk_tree_view_new();
    gtk_tree_view_set_model(GTK_TREE_VIEW(newTreeView), store);
    
    

    for (int i = 0; i < rowsRead; i++) {

    }

    return newTreeView;
}