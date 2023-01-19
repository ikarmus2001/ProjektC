#include "databaseOperations.h"
#include <stdio.h>
#include <gtk/gtk.h>

typedef struct cokolwiek{
    char* nazwa;
    int ilosc;
    float wartosc;
} JakasStruktura;

static GtkWidget* fillWarehouse(JakasStruktura* js) {

    // int rowsCount = sizeof(js)/sizeof(js[0]);
    // for (int i = 0; i < rowsCount; i++) {
        // gtk_tree_view_insert_column_with_attributes(
        //     GTK_TREE_VIEW(resultTreeView),
        //     i,
        //     column
        // );
    // }
    GtkWidget* listbox = gtk_list_box_new();

    return listbox;
}

// Otwiera nowe okno, w którym uzytkownik wskazuje plik z danymi,
// a następnie otwiera go jako strumień
FILE* selectAndOpenFile() {  //GtkWindow* parentWindow) {
    printf("Selecting file started");

    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    FILE* fs = NULL;

    dialog = gtk_file_chooser_dialog_new ("Open File",
                                        NULL, //parentWindow,
                                        action,
                                        "Cancel",
                                        GTK_RESPONSE_CANCEL,
                                        "Open",
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = (char*)gtk_file_chooser_get_filename (chooser);
        fs = fopen(filename, "r");
    }

    gtk_widget_destroy (dialog);

    printf("Selecting file ended, file_chooser destroyed");

    return fs;
}

// Sprawdza poprawność danych do utworzenia struktury, 
// zwraca zainicjowaną i wypełnioną danymi strukturę, 
// w przypadku błędu zwraca NULL
JakasStruktura* checkLine(char* str) {
    char s1;
    char readString[100];
    int d1;
    float f1;
    sscanf(str, "%s;%d;%f", &s1, &d1, &f1);
    strcpy(readString, &s1);
    if (d1 >= 12 && readString[0] != '\0' && f1 > 0 && f1 < 1) {
        JakasStruktura* newLine = (JakasStruktura*)malloc(sizeof(JakasStruktura));
        newLine->nazwa = readString;
        newLine->ilosc = d1;
        newLine->wartosc = f1;
        return newLine;
    }
    return NULL;
}

// Sprawdza ilość linijek wewnątrz otwartego pliku
int countLines(FILE* file) {
    // https://stackoverflow.com/a/70708991/14345698

    // TODO sprawdzać wielkość pliku zanim odczytam ilość linijek,
    // pozwala na zmniejszenie wielkości bufora, odciąża pamięć
    // #include <sys/stat.h> https://stackoverflow.com/a/238609

    char buf[65536];
    int counter = 0;
    for(;;)
    {
        size_t res = fread(buf, 1, 65536, file);
        if (ferror(file))
            return -1;

        int i;
        for(i = 0; i < res; i++)
            if (buf[i] == '\n')
                counter++;

        if (feof(file))
            break;
    }

    return counter;
}


JakasStruktura* createStructsFromFile(FILE* filestream, size_t fileLength){
    JakasStruktura* js = (JakasStruktura*)malloc(fileLength * sizeof(JakasStruktura));
    JakasStruktura* currentItem;
    size_t failed = 0;
    size_t j = 0;
    unsigned int buffer_size = 500;
    char buffer[buffer_size];
    for (size_t i = 0; i < fileLength; i++) {
        fgets(buffer, buffer_size, filestream);
        puts(buffer);
        if ((currentItem = checkLine(buffer)) != NULL) {
            js[j] = *currentItem;
            j++;
        }
        else
            failed++;  // todo obsługa?
    }
    return js;
}

void readFromFile() {
    FILE* filestream = selectAndOpenFile();
    int fileLength = countLines(filestream);

    JakasStruktura* js = createStructsFromFile(filestream, fileLength);
    fclose(filestream);


    GtkWidget* listBox = fillWarehouse(js);
}
