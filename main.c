#include "windowPrepare.h"
#include <gtk-3.0/gtk/gtk.h>
//#include <gtk/gtk.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (geteuid() != 0) {
        printf("Program requires root privileges to run properly\n");
        exit(-1);
    }

    GtkApplication *app;
    int result;
    app = gtk_application_new("ikarmusProject.gtk", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
    result = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    return result;
}

