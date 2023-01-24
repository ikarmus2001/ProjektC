#include "windowPrepare.h"
#include "ViewController.h"


// Tworzy okno i ustawia jego właściwości
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

static void readFromFile_OnClick(GtkWidget *widget, gpointer mainTreeView) {
    // printf("readFromFile Clicked\n");
    
    GtkWidget* newTreeView = malloc(sizeof(GtkWidget));

    if(getNewData(0, &newTreeView)) {
        mainTreeView = newTreeView;
    }
}

static void addItem_OnClick(GtkWidget *widget, gpointer data) {
    // CreateNewElement();  // TODO 
}

static void saveToFile_OnClick(GtkWidget *widget, gpointer data) {
    
}

/*
Connects functions to event handlers
*/
static void addEventSubscriptions(
        GtkWidget* readFromFile_btn, 
        GtkWidget* addItem_btn, 
        GtkWidget* saveToFile_btn,
        GtkWidget* mainTreeView) {

    printf("Connecting button handlers start\n");

    g_signal_connect(readFromFile_btn, "clicked", G_CALLBACK(readFromFile_OnClick), &mainTreeView);
    // printf("After connecting onclick handler readFromFile\n");

    g_signal_connect(addItem_btn, "clicked", G_CALLBACK(addItem_OnClick), NULL);
    // printf("After connecting onclick handler addItem_btn\n");

    g_signal_connect(saveToFile_btn, "clicked", G_CALLBACK(saveToFile_OnClick), NULL);
    // printf("After connecting onclick handler saveToFile_btn\n");

    printf("Connecting button handlers end\n");
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
    GtkTreeModel* store = GTK_TREE_MODEL(gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_INT, G_TYPE_FLOAT));
    GtkWidget* result = gtk_tree_view_new();
    gtk_tree_view_set_model(GTK_TREE_VIEW(result), store);
    
    return result;
}

/*
Creates UI and connects event handlers
*/
void activate(GtkApplication *app, gpointer user_data) {

    GtkWidget *window = setupWindow(app, &user_data);

    GtkWidget *menuHorizontalBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *menuVerticalBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget* mainTreeView = createMainTreeView();

    GtkWidget *readFromFile_btn = gtk_button_new_with_label("Wczytaj dane");
    GtkWidget *addItem_btn = gtk_button_new_with_label("Dodaj <coś>");
    GtkWidget *saveToFile_btn = gtk_button_new_with_label("Zapisz");

    // printf("Po utworzeniu przycisków\n");

    //setupContainers();
    gtk_box_pack_start(GTK_BOX(menuHorizontalBox), mainTreeView, 0, 0, 0);
    gtk_box_pack_end(GTK_BOX(menuHorizontalBox), menuVerticalBox, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(menuVerticalBox), readFromFile_btn, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(menuVerticalBox), addItem_btn, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(menuVerticalBox), saveToFile_btn, 0, 0, 0);

    // printf("Po zapakowaniu buttonów\n");

    addEventSubscriptions(readFromFile_btn, addItem_btn, saveToFile_btn, mainTreeView);
    // gtk_container_add(GTK_CONTAINER(window), button);

    gtk_container_add(GTK_CONTAINER(window), menuHorizontalBox);

    // printf("po dodaniu containera");

    gtk_widget_show_all(window);

    // printf("po pokazaniu widgetów");

    
}