#include "windowPrepare.h"
#include <gtk/gtk.h>

void handleError(GError **err) {
    printf("%d: %s, code: %d", (*err)->domain, (*err)->message, (*err)->code);
    g_error_free(*err);
    // todo jakaś obsługa by się przydała
}

static void button_OnPressed(GtkWidget *widget, gpointer data) {
    
}

static void button_OnRightClick(GtkWidget *widget, gpointer data) {
    gtk_button_set_label(GTK_BUTTON(widget), "Zmieniona zawartosc");
}

static GtkWidget* setupWindow(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Current stash");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    GError* err;
    if(gtk_window_set_icon_from_file(GTK_WINDOW(window), "./jupijej.gif", &err) != TRUE)     {
        handleError(&err);
    }

    return window;
}

// Tworzy obiekt przycisku i nadaje mu zadane właściwości
static GtkWidget* setupButton(char* label)
{
    GtkWidget *btn = gtk_button_new_with_label(label);
    return btn;
}

static void addEventSubscriptions(GtkWidget** button, GtkWidget** window) {
    g_signal_connect(button, "button-press-event", G_CALLBACK(button_OnPressed), NULL);  //todo button click
}

// Ustawia elementy UI
void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = setupWindow(app, &user_data);

    GtkWidget *button = setupButton("Tekst na buttonie");

    addEventSubscriptions(&button, &window);
    gtk_container_add(GTK_CONTAINER(window), button);
    gtk_widget_show_all(window);
    
}
