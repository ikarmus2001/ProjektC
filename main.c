#include "windowPrepare.h"
#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    GtkApplication *app;
    int result;

    app = gtk_application_new("nazwaAplikacji.gtk", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
    result = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    return result;
}

