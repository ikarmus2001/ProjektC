#include "ViewController.h"

enum colNames {
  COL_NAZWA,
  COL_ILOSC,
  COL_WARTOSC,
  COL_COUNT
};

GtkWidget *nameEntry, *amountEntry, *valueEntry;

/*
Populates model with read data
*/
static GtkTreeModel* fill_model(JakasStruktura* js, size_t rowsRead) {
    GtkListStore* store = gtk_list_store_new(
        COL_COUNT, G_TYPE_STRING, G_TYPE_INT, G_TYPE_FLOAT);
    
    GtkTreeIter iter;
    if (js == NULL) {
        // gtk_list_store_append(store, &iter);
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

static void addRowToTreeView(GtkWidget* widget, gpointer mainTreeView) {
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(mainTreeView));
    GtkTreeIter iter;
    gtk_list_store_append(GTK_LIST_STORE(model), &iter);
    gtk_list_store_set(GTK_LIST_STORE(model), &iter, 
        COL_NAZWA, gtk_entry_get_text(GTK_ENTRY(nameEntry)), 
        COL_ILOSC, atoi(gtk_entry_get_text(GTK_ENTRY(amountEntry))), 
        COL_WARTOSC, atof(gtk_entry_get_text(GTK_ENTRY(valueEntry))), 
        -1);
}

/*
Recursively destroys all children of entryWindow
*/
static void addingAborted(GtkWidget* widget, gpointer entryWindow) {
    gtk_widget_destroy(entryWindow);
}

void addRow(GtkWidget* mainTreeView) {
    
    GtkWidget* inputWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    nameEntry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(nameEntry), "Nazwa");
    
    amountEntry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(amountEntry), "Ilość");

    valueEntry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(valueEntry), "Wartość");

    GtkWidget* addButton = gtk_button_new_with_label("Dodaj");
    GtkWidget* abortButton = gtk_button_new_with_label("Anuluj");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
    gtk_box_pack_start(GTK_BOX(box), nameEntry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), amountEntry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), valueEntry, FALSE, FALSE, 0);

    g_signal_connect(addButton, "clicked", G_CALLBACK(addRowToTreeView), mainTreeView);
    g_signal_connect(abortButton, "clicked", G_CALLBACK(addingAborted), inputWindow);


    GtkWidget* bottomBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_box_pack_end(GTK_BOX(bottomBox), abortButton, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(bottomBox), addButton, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(box), bottomBox, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(inputWindow), box);

    gtk_widget_show_all(inputWindow);
}