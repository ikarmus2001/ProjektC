#include "windowPrepare.h"
#include "ViewController.h"

/*
Setups basic window properties
*/
static GtkWidget* setupWindow(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Current stash");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    GError* err;
    if(gtk_window_set_icon_from_file(GTK_WINDOW(window), "./jupijej.gif", &err) != TRUE) {
        handleError(&err);
    }
    return window;
}

GtkWidget* find_child(GtkWidget *parent, const gchar *name) {
    // https://stackoverflow.com/a/23497087/14345698
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    if (g_strcasecmp(gtk_widget_get_name((GtkWidget *)parent), name) == 0) {
        return parent;
    }
    #pragma GCC diagnostic pop
    GList *children = gtk_container_get_children(GTK_CONTAINER(parent));

    do {
        GtkWidget *widget = find_child(children->data, name);
        if (widget != NULL)
            return widget;
    } while ((children = g_list_next(children)) != NULL);

    return NULL;
}

/*
Reads data from file and swaps TreeView with new one
*/
static void readFromFile_OnClick(GtkWidget *widget, gpointer menuHorizontalBox) {
    GtkTreeModel* model = getNewDataFromSource(0);
    if (model == NULL)
        return;
    
    GtkWidget* foundTreeView = find_child(menuHorizontalBox, "mainTreeView");
    gtk_tree_view_set_model(GTK_TREE_VIEW(foundTreeView), model);
}

static void addItem_OnClick(GtkWidget *widget, gpointer menuHorizontalBox) {

    GtkWidget* mainTreeView = find_child(menuHorizontalBox, "mainTreeView");
    createAndShowAddRowEntry(mainTreeView);
}

static void removeItem_OnClick(GtkWidget *widget, gpointer menuHorizontalBox) {

    GtkWidget* mainTreeView = find_child(menuHorizontalBox, "mainTreeView");
    deleteSelectedRowFromTreeView(mainTreeView);
}

/*
Gets data from TreeView and saves it to file
*/
static void saveToFile_OnClick(GtkWidget *widget, gpointer menuHorizontalBox) {
    GtkWidget* mainTreeView = find_child(menuHorizontalBox, "mainTreeView");
    saveData(0, mainTreeView);
}

static void searchItem_OnClick(GtkWidget *widget, gpointer menuHorizontalBox) {
    GtkWidget* mainTreeView = find_child(menuHorizontalBox, "mainTreeView");
    searchItem(mainTreeView);
}

/*
Connects functions to event handlers
*/
static void addEventSubscriptions(
        GtkWidget* readFromFile_btn, GtkWidget* addItem_btn, GtkWidget* saveToFile_btn,
        GtkWidget* removeItem_btn, GtkWidget* searchItem_btn, GtkWidget* menuHorizontalBox) {
    g_signal_connect(readFromFile_btn, "clicked", G_CALLBACK(readFromFile_OnClick), menuHorizontalBox);
    g_signal_connect(addItem_btn, "clicked", G_CALLBACK(addItem_OnClick), menuHorizontalBox);
    g_signal_connect(removeItem_btn, "clicked", G_CALLBACK(removeItem_OnClick), menuHorizontalBox);
    g_signal_connect(saveToFile_btn, "clicked", G_CALLBACK(saveToFile_OnClick), menuHorizontalBox);
    g_signal_connect(searchItem_btn, "clicked", G_CALLBACK(searchItem_OnClick), menuHorizontalBox);
}

/*
Basic error print and ignore mechanism
*/
void handleError(GError **err) {
    printf("%d: %s, code: %d", (*err)->domain, (*err)->message, (*err)->code);
    g_error_free(*err);
}

/*
Creates initial empty TreeView widget
*/
static GtkWidget* createMainTreeView() {
    GtkWidget* result = createNewTreeView(NULL, 0);
    return result;
}

/*
Creates UI and connects event handlers
*/
void activate(GtkApplication* app, gpointer user_data) {

    GtkWidget* window = setupWindow(app, &user_data);

    GtkWidget* menuHorizontalBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget* menuVerticalBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget* mainTreeView = createMainTreeView();
    gtk_widget_set_name(mainTreeView, "mainTreeView");
    // printf("mainTreeView adress = %p", mainTreeView);

    GtkWidget* readFromFile_btn = gtk_button_new_with_label("Importuj bazę");
    GtkWidget* saveToFile_btn = gtk_button_new_with_label("Eksportuj bazę");
    GtkWidget* addItem_btn = gtk_button_new_with_label("Dodaj produkt");
    GtkWidget* removeItem_btn = gtk_button_new_with_label("Usuń produkt");
    GtkWidget* searchItem_btn = gtk_button_new_with_label("Znajdź produkt");

    //setupContainers();
    gtk_box_pack_start(GTK_BOX(menuHorizontalBox), mainTreeView, 0, 0, 0);
    gtk_box_pack_end(GTK_BOX(menuHorizontalBox), menuVerticalBox, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(menuVerticalBox), readFromFile_btn, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(menuVerticalBox), addItem_btn, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(menuVerticalBox), removeItem_btn, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(menuVerticalBox), saveToFile_btn, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(menuVerticalBox), searchItem_btn, 0, 0, 0);

    addEventSubscriptions(readFromFile_btn, addItem_btn, saveToFile_btn, removeItem_btn, searchItem_btn, menuHorizontalBox);

    gtk_container_add(GTK_CONTAINER(window), menuHorizontalBox);

    gtk_widget_show_all(window);
}