#include "ViewController.h"

enum colNames {
  COL_NAZWA = 0,
  COL_ILOSC,
  COL_WARTOSC
};

/*
Creates and populates new GtkTreeView with provided data
*/
GtkWidget* createNewTreeView(JakasStruktura* js, size_t rowsRead) {
    GtkListStore* store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_INT, G_TYPE_FLOAT);
    GtkWidget* newTreeView = gtk_tree_view_new();

    printf("Element 0: %s", js[0].nazwa);
    
    GtkTreeIter iter;
    for (int i = 0; i < rowsRead; i++) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 
            COL_NAZWA, js[i].nazwa,
            COL_ILOSC, js[i].ilosc,
            COL_WARTOSC, js[i].wartosc);
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(newTreeView), GTK_TREE_MODEL(store));
    return newTreeView;
}

/*
Retrieves data and creates new TreeView widget
*/
GtkWidget* getNewData(unsigned char source) {
    JakasStruktura* js;
    size_t rowsRead;
    unsigned char result;
    js = readDatabase(source, &rowsRead);
    
    GtkWidget* newTreeView = createNewTreeView(js, rowsRead);
    return newTreeView;
}

unsigned char saveData(unsigned char destination, GtkWidget* existingTreeView) {
    
}