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
    printf("Selecting file started\n");

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

// Sprawdza poprawność danych do utworzenia struktury, 
// zwraca zainicjowaną i wypełnioną danymi strukturę, 
// w przypadku błędu zwraca NULL
JakasStruktura* checkLine(char* str) {
    // char* s1;
    char readString[100];
    int d1;
    float f1;
    sscanf(str, "%100[^;];%d;%f", &readString[0], &d1, &f1);
    // strcpy(readString, s1);
    printf("Checking line: Read nazwa: \"%s\", ilosc=%d, wartosc=%f\n", readString, d1, f1);
    if (d1 >= 12 && readString[0] != '\0' && f1 > 0 && f1 < 1) {
        JakasStruktura* newLine = (JakasStruktura*)malloc(sizeof(JakasStruktura));
        newLine->nazwa = readString;
        newLine->ilosc = d1;
        newLine->wartosc = f1;
        printf("Checking line: Created newLine struct - nazwa: \"%s\", ilosc=%d, wartosc=%f\n", 
                newLine->nazwa, newLine->ilosc, newLine->wartosc);
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


JakasStruktura* createStructsFromFile(FILE* filestream, size_t fileLength){
    printf("Create struct from file: start\n");

    JakasStruktura* js = (JakasStruktura*)malloc(fileLength * sizeof(JakasStruktura));
    JakasStruktura* currentItem;
    size_t failed = 0;
    size_t j = 0;
    unsigned int buffer_size = 500;
    char buffer[buffer_size];
    for (size_t i = 0; i < fileLength; i++) {
        fgets(buffer, buffer_size, filestream);
        printf("Create struct from file: read line: ");
        printf("%s\n", buffer);
        if ((currentItem = checkLine(buffer)) != NULL) {
            printf("STRUCT CHECKED, GOT %s, %d, %f", currentItem->nazwa, currentItem->ilosc, currentItem->wartosc);
            js[j] = *currentItem;
            printf("Create struct from file: Checked, got \"%s\", ilosc=%d, wartosc=%f\n", 
                    currentItem->nazwa, currentItem->ilosc, currentItem->wartosc);
            j++;
        }
        else
            failed++;  // todo obsługa?
    }
    printf("Create struct from file: Finished creating, encountered %zd / %zd errors\n", failed, fileLength);
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
    
    JakasStruktura* js = createStructsFromFile(filestream, fileLength);
    fclose(filestream);
    GtkWidget* listBox = fillWarehouse(js);
    return 0;
}
