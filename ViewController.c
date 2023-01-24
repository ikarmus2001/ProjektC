#include "ViewController.h"

enum {
  COL_NAZWA = 0,
  COL_ILOSC,
  COL_WARTOSC
};

GtkWidget* createNewTreeView(JakasStruktura js[], size_t rowsRead) {
    GtkWidget* store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_INT, G_TYPE_FLOAT);
    GtkWidget* newTreeView = gtk_tree_view_new();
    
    GtkTreeIter iter;
    for (int i = 0; i < rowsRead; i++) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 
            COL_NAZWA, (&js[i])->nazwa,
            COL_ILOSC, (&js[i])->ilosc,
            COL_WARTOSC, (&js[i])->wartosc);
    }

    store = GTK_TREE_MODEL(store);

    gtk_tree_view_set_model(GTK_TREE_VIEW(newTreeView), store);
    return newTreeView;
}

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



