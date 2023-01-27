#include "ViewController.h"

enum colNames {
    COL_ID,
    COL_NAZWA,
    COL_ILOSC,
    COL_WARTOSC,
    COL_COUNT
};

GtkWidget *inputWindow, *nameEntry, *amountEntry, *valueEntry, *searchEntry, *searchWindow, *searchResultsWindow;
size_t *ids, *idsCount, *structsCount, *matchLength, *highestId;

/*
Populates model with read data
*/
static GtkTreeModel* fill_model(JakasStruktura* js, size_t rowsRead) {
    GtkListStore* store = gtk_list_store_new(
        COL_COUNT, G_TYPE_LONG, G_TYPE_STRING, G_TYPE_INT, G_TYPE_FLOAT);
    // printf("rowsRead = %zd\n", rowsRead);
    
    if (js != NULL) {
        GtkTreeIter iter;
        for (int i = 0; i < rowsRead; i++) {
            // printf("js[%d] = %s %d %f\n", i, js[i].nazwa, js[i].ilosc, js[i].wartosc);
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 
                COL_ID, js[i].id,
                COL_NAZWA, js[i].nazwa, 
                COL_ILOSC, js[i].ilosc, 
                COL_WARTOSC, js[i].wartosc, 
                -1);
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
        GTK_TREE_VIEW(view), -1, "ID", renderer, "text", COL_ID, NULL);

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
    
    if (structsCount == NULL)
        structsCount = malloc(sizeof(size_t));
    *structsCount = rowsRead;
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
    g_object_unref(model);

    return view;
}

/*
Retrieves data from given source and creates new TreeModel
*/
GtkTreeModel* getNewDataFromSource(unsigned char source) {
    JakasStruktura* js;
    size_t* rowsRead = malloc(sizeof(size_t));
    unsigned char result;
    js = readDatabase(source, rowsRead);
    if (js == NULL)
        return NULL;
    
    GtkWidget* newTreeView = createNewTreeView(js, *rowsRead);
    highestId = malloc(sizeof(size_t));
    *highestId = *rowsRead - 1;
    
    return gtk_tree_view_get_model(GTK_TREE_VIEW(newTreeView));  // refactor function to return model
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

    // printf("nazwa = %s, ilosc = %d, wartosc = %f\n", nazwa, ilosc, wartosc);
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

static void addNewRowFromEntryToTreeView(GtkWidget* widget, gpointer mainTreeView) {
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(mainTreeView));
    GtkTreeIter iter;
    if (highestId == NULL) {
        highestId = malloc(sizeof(size_t));
        *highestId = 0;
    }
    else 
        *highestId += 1;
    
    gtk_list_store_append(GTK_LIST_STORE(model), &iter);
    gtk_list_store_set(GTK_LIST_STORE(model), &iter, 
        COL_ID, *highestId,
        COL_NAZWA, gtk_entry_get_text(GTK_ENTRY(nameEntry)), 
        COL_ILOSC, atoi(gtk_entry_get_text(GTK_ENTRY(amountEntry))), 
        COL_WARTOSC, atof(gtk_entry_get_text(GTK_ENTRY(valueEntry))), 
        -1);
}

/*
Recursively destroys all children of entryWindow
*/
static void operationAborted(GtkWidget* widget, gpointer entryWindow) {
    gtk_widget_destroy(entryWindow);
}

void deleteSelectedRowFromTreeView(GtkWidget* mainTreeView) {
    GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(mainTreeView));
    GtkTreeModel* model;
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
        *structsCount--;
    }
}

void createAndShowAddRowEntry(GtkWidget* mainTreeView) {
    
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

    g_signal_connect(addButton, "clicked", G_CALLBACK(addNewRowFromEntryToTreeView), mainTreeView);
    g_signal_connect(abortButton, "clicked", G_CALLBACK(operationAborted), inputWindow);


    GtkWidget* bottomBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_box_pack_end(GTK_BOX(bottomBox), abortButton, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(bottomBox), addButton, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(box), bottomBox, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(inputWindow), box);
    gtk_widget_show_all(inputWindow);
}

gboolean compareNames(GtkTreeModel* model, GtkTreePath* path, GtkTreeIter* iter, gpointer searchedName) {
    SearchedString* target = (SearchedString *)searchedName;
    char* nazwa;
    size_t id;
    gtk_tree_model_get(model, iter, COL_ID, &id, COL_NAZWA, &nazwa, -1);
    size_t nameLen = strlen(nazwa);
    size_t mostMatchingChars = 0;
    size_t matching_chars = 0;
    
    if (target->length > nameLen)
        return FALSE;

    for (int i = 0; i < nameLen; i++) {
        if (nazwa[i] != target->string[0])
                continue;
        else {
            for (int j = i; j < nameLen; j++) {
                if (target->length - j <= nameLen-j && nazwa[j] == target->string[target->length - j]) {
                    matching_chars += 1;
                    continue;
                }
                break;
            }
            if (matching_chars > mostMatchingChars) {
                mostMatchingChars = matching_chars;
            }
            matching_chars = 0;
        }
    }
    
    if (mostMatchingChars > *matchLength) {
        // store only the most matching names
        free(ids);
        ids = malloc(*structsCount * sizeof(size_t));
        *matchLength = mostMatchingChars;
        *idsCount = 0;
    }

    if (mostMatchingChars == *matchLength) {
        ids[*idsCount] = id;
        *idsCount += 1;
    }
        
    return FALSE;
}

gboolean idInArray(size_t id) {
    for (int i = 0; i < *idsCount; i++) {
        if (ids[i] == id)
            return TRUE;
    }
    return FALSE;
}

gboolean getResultByIdAndStore(GtkTreeModel* model, GtkTreePath* path, GtkTreeIter* iter, gpointer searchResultsListStore) {
    GtkListStore *listStore = (GtkListStore *)searchResultsListStore;
    size_t id;
    gtk_tree_model_get(model, iter, COL_ID, id, -1);
    if (idInArray(id)) {
        char* nazwa;
        int ilosc;
        float wartosc;
        GtkTreeIter* newIter;
        gtk_tree_model_get(model, iter, COL_NAZWA, &nazwa, COL_ILOSC, ilosc, COL_WARTOSC, wartosc, -1);
        gtk_list_store_append(listStore, newIter);
        gtk_list_store_set(listStore, newIter, 
            COL_ID, id,
            COL_NAZWA, gtk_entry_get_text(GTK_ENTRY(nameEntry)), 
            COL_ILOSC, atoi(gtk_entry_get_text(GTK_ENTRY(amountEntry))), 
            COL_WARTOSC, atof(gtk_entry_get_text(GTK_ENTRY(valueEntry))), 
            -1);
    }
}

GtkListStore* findItemsByIds(GtkWidget* mainTreeView) {
    GtkListStore* searchResultsListStore = gtk_list_store_new(COL_COUNT, G_TYPE_LONG, G_TYPE_STRING, G_TYPE_INT, G_TYPE_FLOAT);
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(mainTreeView));
    
    gtk_tree_model_foreach(model, getResultByIdAndStore, searchResultsListStore);
    
    return searchResultsListStore;
}

void openSearchResultsWindow(GtkWidget* mainTreeView) {
    if (searchResultsWindow != NULL) {
        gtk_widget_destroy(searchResultsWindow);
    }
    searchResultsWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    GtkWidget* searchResultsTreeView = gtk_tree_view_new();
    GtkCellRenderer* renderer;
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(searchResultsTreeView), -1, "ID", renderer, "text", COL_ID, NULL);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(searchResultsTreeView), -1, "Nazwa", renderer, "text", COL_NAZWA, NULL);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(searchResultsTreeView), -1, "Ilość", renderer, "text", COL_ILOSC, NULL);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(
        GTK_TREE_VIEW(searchResultsTreeView), -1, "Wartość", renderer, "text", COL_WARTOSC, NULL);
    
    
    GtkListStore* searchResultsListStore = findItemsByIds(mainTreeView);

    gtk_tree_view_set_model(GTK_TREE_VIEW(searchResultsTreeView), GTK_TREE_MODEL(searchResultsListStore));

    

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
    gtk_box_pack_start(GTK_BOX(box), searchResultsTreeView, FALSE, FALSE, 0);

    GtkWidget* okButton = gtk_button_new_with_label("Ok");
    g_signal_connect(okButton, "clicked", G_CALLBACK(operationAborted), searchResultsWindow);
    GtkWidget* bottomBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_box_pack_start(GTK_BOX(bottomBox), okButton, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(box), bottomBox, FALSE, FALSE, 0);

    
    gtk_container_add(GTK_CONTAINER(searchResultsWindow), box);
    gtk_widget_show_all(searchResultsWindow);
}

void searchTreeView(GtkWidget* widget, gpointer mainTreeView) {
    SearchedString* searchedName = malloc(sizeof(SearchedString));
    const char* name = gtk_entry_get_text(GTK_ENTRY(searchEntry));
    searchedName->string = malloc(strlen(name) + 1);
    if (searchedName->string == NULL) {
        printf("Memory allocation failed\n\n");
        return;
    }
    strcpy(searchedName->string, name);
    ids = malloc(sizeof(size_t) * *structsCount);
    searchedName->string[strlen(name)] = '\0';
    searchedName->length = strlen(name);
    searchedName->maxIdsLength = *structsCount;

    matchLength = malloc(sizeof(size_t));
    *matchLength = 0;
    idsCount = malloc(sizeof(size_t));
    *idsCount = 0;
    
    gtk_tree_model_foreach(gtk_tree_view_get_model(GTK_TREE_VIEW(mainTreeView)), compareNames, searchedName);

    free(searchedName->string);
    free(searchedName);

    printf("*idsCount = %zd, *matchLength = %zd\n", *idsCount, *matchLength);

    if (*matchLength > 0) {
        printf("Found %zd matches\n", *matchLength);
        openSearchResultsWindow((GtkWidget *)mainTreeView);
    }   
}

void searchItem(GtkWidget* mainTreeView) {

    if (searchWindow != NULL) {
        gtk_widget_destroy(searchWindow);
    }

    searchWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    searchEntry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(searchEntry), "Nazwa szukanego produktu");

    GtkWidget* searchButton = gtk_button_new_with_label("Szukaj");
    GtkWidget* abortButton = gtk_button_new_with_label("Anuluj");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
    gtk_box_pack_start(GTK_BOX(box), searchEntry, FALSE, FALSE, 0);

    GtkWidget* bottomBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_box_pack_end(GTK_BOX(bottomBox), abortButton, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(bottomBox), searchButton, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(box), bottomBox, FALSE, FALSE, 0);

    // gtk_tree_model_foreach(gtk_tree_view_get_model(GTK_TREE_VIEW(mainTreeView)), countRows, maxIdsLength);

    g_signal_connect(searchButton, "clicked", G_CALLBACK(searchTreeView), mainTreeView);
    g_signal_connect(abortButton, "clicked", G_CALLBACK(operationAborted), searchWindow);

    gtk_container_add(GTK_CONTAINER(searchWindow), box);
    gtk_widget_show_all(searchWindow);
}
