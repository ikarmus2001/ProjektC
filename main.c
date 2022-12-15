#include <gtk/gtk.h>

void button_OnClick(GtkWidget *widget, gpointer data)
{
    gtk_button_set_label(widget, "Zmieniona zawartosc");
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    GtkWidget *button = gtk_button_new_with_label("Tekst na buttonie");

    g_signal_connect(button, "clicked", G_CALLBACK(button_OnClick), NULL);
    gtk_container_add(GTK_CONTAINER(window), button);
    gtk_widget_show_all(window);
    
}

int main(int argc, char *argv[])
{
    GtkApplication *app;
    int result;

    app = gtk_application_new("nazwaAplikacji.gtk", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    result = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    return result;
}

