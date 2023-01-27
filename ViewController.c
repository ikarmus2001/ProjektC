#include "ViewController.h"

enum colNames {
  COL_NAZWA,
  COL_ILOSC,
  COL_WARTOSC,
  COL_COUNT
};

GtkWidget *inputWindow, *nameEntry, *amountEntry, *valueEntry;

/*
Populates model with read data
*/
static GtkTreeModel* fill_model(JakasStruktura* js, size_t rowsRead) {
    GtkListStore* store = gtk_list_store_new(
        COL_COUNT, G_TYPE_STRING, G_TYPE_INT, G_TYPE_FLOAT);
    printf("rowsRead = %zd", rowsRead);
    
    if (js != NULL) {
        GtkTreeIter iter;
        for (int i = 0; i < rowsRead; i++) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 
                COL_NAZWA, js[i].nazwa, 
                COL_ILOSC, js[i].ilosc, 
                COL_WARTOSC, js[i].wartosc, 
                -1);
            printf("js[%d] = %s %d %f\n", i, js[i].nazwa, js[i].ilosc, js[i].wartosc);
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
GtkTreeModel* getNewData(unsigned char source) {
    JakasStruktura* js;
    size_t* rowsRead = malloc(sizeof(size_t));
    unsigned char result;
    js = readDatabase(source, rowsRead);
    
    GtkWidget* newTreeView = createNewTreeView(js, *rowsRead);
    
    return gtk_tree_view_get_model(GTK_TREE_VIEW(newTreeView));
}

gboolean dataFromModel(GtkTreeModel* model, GtkTreePath* path, GtkTreeIter* iter, gpointer fs) {
    char* nazwa;
    int ilosc;
    float wartosc;
    gtk_tree_model_get(model, iter,
                      COL_NAZWA, &nazwa,
                      COL_ILOSC, &ilosc,
                      COL_WARTOSC, &wartosc,
                      -1);

    printf("nazwa = %s, ilosc = %d, wartosc = %f\n", nazwa, ilosc, wartosc);
    saveToFileManuallyLink(nazwa, ilosc, wartosc, fs);

    return FALSE;
}


/*
Just for fast testing
*/
char* selectF(GtkFileChooserAction action) {
    GtkWidget* dialog;
    char *filename = NULL;

    dialog = gtk_file_chooser_dialog_new ("Open File",
        NULL /*parentWindow*/, action, "Cancel", GTK_RESPONSE_CANCEL,
        "Open", GTK_RESPONSE_ACCEPT, NULL);

    printf("Selecting file: Setup file chooser\n");

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);    
        filename = (char*)gtk_file_chooser_get_filename(chooser);
        printf("Selecting file: Chosen file: %s\n", filename);
    }

    gtk_widget_destroy(dialog);
    printf("Selecting file ended, file_chooser destroyed\n");
    return filename;
}

unsigned char getDataFromTreeView(GtkWidget* treeView) {
    char* filename = selectF(GTK_FILE_CHOOSER_ACTION_SAVE);
    if (filename == NULL)
        return -1;

    FILE* filestream = fopen(filename, "a");

    gtk_tree_model_foreach(gtk_tree_view_get_model(GTK_TREE_VIEW(treeView)), dataFromModel, filestream);
    
    fclose(filestream);
    return 0;
}

/*
Retrieves data from existing TreeView and saves it to chosen destination
*/
unsigned char saveData(unsigned char destination, GtkWidget* existingTreeView) {
    // printf("existingTreeView = %p", existingTreeView);
    getDataFromTreeView(existingTreeView);
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

void deleteRow(GtkWidget* mainTreeView) {
    GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(mainTreeView));
    GtkTreeModel* model;
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
    }
}

void addRow(GtkWidget* mainTreeView) {
    
    if (inputWindow != NULL) {
        gtk_widget_destroy(inputWindow);
    }
    
    inputWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

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