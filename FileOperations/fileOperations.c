#include "fileOperations.h"
#include "../Structures/structures.h"
#include <stdio.h>
#include <gtk/gtk.h>

int countLines(FILE* file) {
    // https://stackoverflow.com/a/70708991/14345698

    // TODO check file length before reading
    // #include <sys/stat.h> https://stackoverflow.com/a/238609

    char buf[65536];
    int counter = 0;

    printf("Counting lines in file\n");

    for(;;)
    {
        printf("Counting lines: inf loop start\n");
        size_t res = fread(buf, 1, 65536, file);
        if (ferror(file))
            return -1;

        int i;
        for(i = 0; i < res; i++) {
            if (buf[i] == '\n') {
                // printf("Counting lines: Found %dth newline\n", counter+1); // overkill
                counter++;
            }
        }

        if (feof(file))
            break;
    }
    fseek(file, 0, SEEK_SET);
    printf("Counting lines: END! Found %d lines \n", counter);
    return counter;
}

void saveToFile(char filename[], JakasStruktura struktury[]) {
    signed char toDelete[sizeof(struktury)/sizeof(JakasStruktura)];
    signed char toAlter[sizeof(struktury)/sizeof(JakasStruktura)];
    signed char alterIndex = 0, deleteIndex = 0;
    FILE* fs;

    if ((fs = fopen(filename, "w")) != NULL) {

    for (int i = 0; i < sizeof(struktury)/sizeof(JakasStruktura); i++) {
        if ((&struktury[i])->stan != 2)
            fprintf(fs, "%s;%d;%f\n", (&struktury[i])->nazwa, (&struktury[i])->ilosc, (&struktury[i])->wartosc);
        // switch ((&struktury[i])->stan) {
        //     case -1:
        //         toDelete[deleteIndex] = (&struktury[i])->id;
        //         deleteIndex++;
        //         continue;
        //     case 1:
        //         toAlter[alterIndex] = (&struktury[i])->id;
        //         alterIndex++;
        //         break;
        //     default:
        //         fprintf(fs, "%s;%d;%f\n", (&struktury[i])->nazwa, (&struktury[i])->ilosc, (&struktury[i])->wartosc);
        // }
    }

    fclose(fs);
    }
}

FILE* selectAndOpenFile() {  //GtkWindow* parentWindow) {
    printf("Selecting file started\n");

    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    FILE* fs = NULL;

    

    dialog = gtk_file_chooser_dialog_new ("Open File",
        NULL /*parentWindow*/, action, "Cancel", GTK_RESPONSE_CANCEL,
        "Open", GTK_RESPONSE_ACCEPT, NULL);

    printf("Selecting file: Setup file chooser\n");

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        
        filename = (char*)gtk_file_chooser_get_filename (chooser);

        printf("Selecting file: Chosen file: %s\n", filename);

        fs = fopen(filename, "r");
    }

    gtk_widget_destroy (dialog);

    printf("Selecting file ended, file_chooser destroyed\n");

    return fs;
}


JakasStruktura* createStructsFromFile(FILE* filestream, size_t fileLength, JakasStruktura js[]) {
    printf("Create struct from file: start\n");
    JakasStruktura* currentItem;
    size_t failed = 0;
    size_t j = 0;
    unsigned int buffer_size = 500;
    char buffer[buffer_size];
    for (size_t i = 0; i < fileLength; i++) {
        fgets(buffer, buffer_size, filestream);
        currentItem = (JakasStruktura*)malloc(sizeof(JakasStruktura));
        // printf("createStructsFromFile: read line: ");
        //printf("%s \n", buffer);
        if ((currentItem = checkLine(buffer)) != NULL) {
            printf("createStructsFromFile: STRUCT CHECKED, GOT %s, %d, %f \n", 
                    currentItem->nazwa, currentItem->ilosc, currentItem->wartosc);
            js[j] = *currentItem;
            printf("createStructsFromFile: Checked, got \"%s\", ilosc=%d, wartosc=%f \n", 
                    currentItem->nazwa, currentItem->ilosc, currentItem->wartosc);
            j++;
            printf("%zd", j);
        }
        else
            failed++;  // todo obsługa?
    }
    printf("Create struct from file: Finished creating, encountered %zd / %zd errors\n", failed, fileLength);
    for (size_t i = 0; i < fileLength; i++) {
        printf("\"%s\", ilosc=%d, wartosc=%f\n", 
            (&js[i])->nazwa, (&js[i])->ilosc, (&js[i])->wartosc);
    }
    return js;
}


// Handles loading database from file
int readFromFile() {
    FILE* filestream = selectAndOpenFile();
    if (filestream == NULL)
        return -1;
    
    int fileLength = countLines(filestream);

    if (fileLength <= 0)
        return -1;
    
    JakasStruktura js[fileLength]; // = malloc(fileLength * sizeof(JakasStruktura));

    createStructsFromFile(filestream, fileLength, js);
    fclose(filestream);
    printf("readFromFile: Escaped, stream closed");
    GtkWidget* listBox = fillWarehouse(js);
    return 0;
}