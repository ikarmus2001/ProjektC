#include "../FileOperations/fileOperations.h"
#include "../Structures/structures.h"

// #ifdef DEBUG
// #define LOG(x) fprintf(stdin, x);
// #else
// #define LOG(x)
// #endif

/*
Counts how many struct rows might be possibly created by
counting amount of lines in file
*/
int countLines(FILE* file) {
    // https://stackoverflow.com/a/70708991/14345698

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

/*
Opens file selection dialog and on success returns abs path to file
*/
char* selectFile() {
    GtkWidget* dialog;
    char *filename = NULL;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    dialog = gtk_file_chooser_dialog_new ("Open File",
        NULL /*parentWindow*/, action, "Cancel", GTK_RESPONSE_CANCEL,
        "Open", GTK_RESPONSE_ACCEPT, NULL);

    printf("Selecting file: Setup file chooser\n");

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);    
        filename = (char*)gtk_file_chooser_get_filename(chooser);
        printf("Selecting file: Chosen file: %s\n", filename);
    }

    gtk_widget_destroy (dialog);
    printf("Selecting file ended, file_chooser destroyed\n");
    return filename;
}

/* 
Checks data integrity and creates structures,
provides initiated and data-filled struct,
Returns 0, on error 1
*/
JakasStruktura* checkLine(char* str) {
    char* readString;
    int d1;
    float f1;
    
    sscanf(str, "%100[^;];%d;%f", readString, &d1, &f1);
    // printf("Checking line: Read nazwa: \"%s\", ilosc=%d, wartosc=%f\n", readString, d1, f1);
    if (readString[0] != '\0') {
        JakasStruktura* newLine = malloc(sizeof(JakasStruktura));
        // strcpy(newLine->nazwa, readString);
        newLine->nazwa = readString;
        newLine->ilosc = d1;
        newLine->wartosc = f1;
        // printf("Checking line: Created newLine struct - nazwa: \"%s\", ilosc=%d, wartosc=%f\n", 
        //         newLine->nazwa, newLine->ilosc, newLine->wartosc);
        return newLine;
    }
    return NULL;
}

/*
Reads `fileLength` of lines from filestream and saves them to preallocated `js`
Returns amount of invalid lines
*/
JakasStruktura* createStructsFromFile(FILE* filestream, size_t fileLength) {
    printf("Create struct from file: start\n");
    JakasStruktura* js = malloc(fileLength * sizeof(js));
    JakasStruktura* currentItem = malloc(sizeof(js));
    size_t failed = 0;
    size_t j = 0;
    unsigned int buffer_size = 500;
    
    // ERROR: Segfault somewhere here

    for (size_t i = 0; i < fileLength; i++) {
        char buffer[buffer_size];
        printf("start pętli %zd\n", i);
        fgets(buffer, buffer_size, filestream);
        if ((currentItem = checkLine(buffer)) != NULL) {
            printf("createStructsFromFile: STRUCT CHECKED, GOT %s, %d, %f \n", 
                    currentItem->nazwa, 
                    currentItem->ilosc, 
                    currentItem->wartosc);
            currentItem->id = j;
            currentItem->stan = 0;

            printf("createStructsFromFile: before assignment currentItem[%zd] (%s) to js[%zd]\n",
                    i, currentItem->nazwa, j);

            js[j] = *currentItem;
            
            printf("createStructsFromFile: inserted %zdth struct\n\n", j);
            currentItem = (JakasStruktura*)malloc(sizeof(js));
            j++;
        }
        else
            failed++;  // TODO
    }
    printf("Create struct from file: Finished creating, failed reading %zd / %zd lines\n", failed, fileLength);

    // printf("createStructsFromFile: Last in memory %s, %d, %f<\n", 
    //                 currentItem[19]->nazwa, 
    //                 currentItem[19]->ilosc, 
    //                 currentItem[19]->wartosc);
    return js;
}

/*
Handles loading database from file
*/
JakasStruktura* getDataFromFile(size_t* rowsRead) {
    char* filename = selectFile();
    if (filename == NULL)
        return NULL;

    JakasStruktura* structuresArray;

    FILE* filestream = fopen(filename, "r");
    int fileLength = countLines(filestream);
    if (fileLength <= 0)
        return NULL;
    
    // JakasStruktura* js = malloc(fileLength * sizeof(JakasStruktura));

    structuresArray = createStructsFromFile(filestream, fileLength);
    printf("created structures:");
    // printf("[0] %s, [19] %s\n", 
    //         structuresArray[0]->nazwa, 
    //         structuresArray[19]->nazwa);

    fclose(filestream);

    // printf("readFromFile: Escaped, stream closed");
    return structuresArray;
}

/*
Opens file stream and saves provided structs
Returns -1 on file opening error
*/
char saveToFile(char* path, JakasStruktura* structsArray, size_t arrayLength) {
    FILE* fs;

    if ((fs = fopen(path, "w")) == NULL) {
        fclose(fs);
        return -1;
    }

    for (int i = 0; i < arrayLength; i++) {
        if (structsArray->stan != 2) {
            fprintf(fs, "%s;%d;%f\n", 
            structsArray->nazwa, structsArray->ilosc, structsArray->wartosc);

            structsArray = structsArray + sizeof(JakasStruktura);
        }
    }
    fclose(fs);
    return 0;
}
