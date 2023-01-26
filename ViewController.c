#include "ViewController.h"

enum colNames {
  COL_NAZWA,
  COL_ILOSC,
  COL_WARTOSC,
  COL_COUNT
};

/*
Populates model with read data
*/
static GtkTreeModel* fill_model(JakasStruktura* js, size_t rowsRead) {
    GtkListStore* store = gtk_list_store_new(
        COL_COUNT, G_TYPE_STRING, G_TYPE_INT, G_TYPE_FLOAT);
    
    GtkTreeIter iter;
    printf("js = %p\n", js);
    if (js == NULL) {
        printf("js IS NULL!\n");
        gtk_list_store_append(store, &iter);
    }
    else {
        for (int i = 0; i < rowsRead; i++) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 
                COL_NAZWA, js[i].nazwa, 
                COL_ILOSC, js[i].ilosc, 
                COL_WARTOSC, js[i].wartosc, 
                -1);
            printf("js[%d] = %s %d %f", i, js[i].nazwa, js[i].ilosc, js[i].wartosc);
        }
    }
    
    return GTK_TREE_MODEL(store);
}

/*
Creates and populates new GtkTreeView with provided data,
empty if first argument is NULL
*/
GtkWidget* createNewTreeView(JakasStruktura* js, size_t rowsRead) {
    GtkWidget* view = gtk_tree_view_new();
    GtkCellRenderer* renderer;


    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(view), -1, "Nazwa", renderer, "text", COL_NAZWA, NULL);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(view), -1, "Ilość", renderer, "text", COL_ILOSC, NULL);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(view), -1, "Wartość", renderer, "text", COL_WARTOSC, NULL);

    GtkTreeModel* model = fill_model(js, rowsRead);
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
    g_object_unref(model);

    return view;
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

gboolean dataFromModel(GtkTreeModel* model, GtkTreePath* path, GtkTreeIter* iter, gpointer data) {
    char* nazwa;
    int* ilosc;
    float* wartosc;
    gtk_tree_model_get (model, iter,
                      COL_NAZWA, &nazwa,
                      COL_ILOSC, &ilosc,
                      COL_WARTOSC, &wartosc,
                      -1);    

    saveToFileManuallyLink(nazwa, ilosc, wartosc);

    return FALSE;
}

unsigned char getDataFromTreeView(GtkWidget* treeView, size_t* rowsRead) {
    // *rowsRead = gtk_tree_model_iter_n_children(gtk_tree_view_get_model(GTK_TREE_VIEW(treeView)), NULL);
    // JakasStruktura* js = malloc(rowsRead * sizeof(JakasStruktura));
    gtk_tree_model_foreach(gtk_tree_view_get_model(GTK_TREE_VIEW(treeView)), dataFromModel, NULL);

    return 0;
}

/*
Retrieves data from existing TreeView and saves it to chosen destination
*/
unsigned char saveData(unsigned char destination, GtkWidget* existingTreeView) {
    size_t* rowsRead;
    getDataFromTreeView(existingTreeView, rowsRead);
    // return saveDatabase(destination, dataToSave, rowsRead);
    return 0;
}