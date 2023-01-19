#include "windowPrepare.h"
#include <gtk/gtk.h>
#include "databaseOperations.h"



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

// Tworzy obiekt przycisku i nadaje mu zadane właściwości
static GtkWidget* setupButton(char* label) {
    GtkWidget *btn = gtk_button_new_with_label(label);
    return btn;
}


static void readFromFile_OnClick(GtkWidget *widget, gpointer data) {
    printf("readFromFile Clicked\n");
    if (readFromFile() != 0)
        printf("Error in readFromFile\n");
}

static void addItem_OnClick(GtkWidget *widget, gpointer data) {
    
}

static void saveToFile_OnClick(GtkWidget *widget, gpointer data) {
    
}

// Dodaje funkcje do zdarzeń
static void addEventSubscriptions(
        GtkWidget* readFromFile_btn, 
        GtkWidget* addItem_btn, 
        GtkWidget* saveToFile_btn,
        GtkWidget* window) {

    printf("Connecting button handlers start\n");

    g_signal_connect(readFromFile_btn, "clicked", G_CALLBACK(readFromFile_OnClick), NULL);
    printf("After connecting onclick handler readFromFile\n");


    g_signal_connect(addItem_btn, "clicked", G_CALLBACK(addItem_OnClick), NULL);
    printf("After connecting onclick handler addItem_btn\n");

    g_signal_connect(saveToFile_btn, "clicked", G_CALLBACK(saveToFile_OnClick), NULL);
    printf("After connecting onclick handler saveToFile_btn\n");

    printf("Connecting button handlers end\n");
}

void handleError(GError **err) {
    printf("%d: %s, code: %d", (*err)->domain, (*err)->message, (*err)->code);
    g_error_free(*err);
    // todo jakaś obsługa by się przydała
}

static GtkWidget* createMainTreeView() {
    GtkWidget* result = gtk_tree_view_new();
    return result;
}

// Ustawia elementy UI
void activate(GtkApplication *app, gpointer user_data) {

    GtkWidget *window = setupWindow(app, &user_data);

    GtkWidget *menuHorizontalBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *menuVerticalBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget* mainTreeView = createMainTreeView();

    GtkWidget *readFromFile_btn = setupButton("Wczytaj dane");
    GtkWidget *addItem_btn = setupButton("Dodaj <coś>");
    GtkWidget *saveToFile_btn = setupButton("Zapisz");

    fprintf(stdout, "Po utworzeniu przycisków\n");

    //setupContainers();
    gtk_box_pack_start(GTK_BOX(menuHorizontalBox), mainTreeView, 0, 0, 0);
    gtk_box_pack_end(GTK_BOX(menuHorizontalBox), menuVerticalBox, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(menuVerticalBox), readFromFile_btn, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(menuVerticalBox), addItem_btn, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(menuVerticalBox), saveToFile_btn, 0, 0, 0);

    fprintf(stdout, "Po zapakowaniu buttonów\n");

    addEventSubscriptions(readFromFile_btn, addItem_btn, saveToFile_btn, window);
    // gtk_container_add(GTK_CONTAINER(window), button);
    gtk_container_add(GTK_CONTAINER(window), menuHorizontalBox);

    fprintf(stdout, "po dodaniu containera");

    gtk_widget_show_all(window);

    fprintf(stdout, "po pokazaniu widgetów");

    
}