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
char* selectFile(GtkFileChooserAction action) {
    GtkWidget* dialog;
    char *filename = NULL;

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
    char readString[100+1];
    int d1;
    float f1;
    
    if (sscanf(str, "%100[^;];%d;%f", readString, &d1, &f1) != 3) {
        printf("sscanf reading failed\n\n");
        return NULL;
    }
    // printf("Checking line: Read nazwa: \"%s\", ilosc=%d, wartosc=%f\n", readString, d1, f1);
    if (readString[0] != '\0') {
        JakasStruktura* newLine = malloc(sizeof(JakasStruktura));
        newLine->nazwa = malloc(strlen(readString) + 1);
        if (newLine->nazwa == NULL) {
            printf("Mallocking failed\n\n");
            return NULL;
        }
        strcpy(newLine->nazwa, readString);
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
JakasStruktura* createStructsFromFile(FILE* filestream, size_t fileLength, size_t* rowsRead) {
    printf("Create struct from file: start\n");
    JakasStruktura* js = malloc(fileLength * sizeof(JakasStruktura));
    JakasStruktura* currentItem = malloc(sizeof(JakasStruktura));
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
            currentItem = (JakasStruktura*)malloc(sizeof(JakasStruktura));
            j++;
        }
        else
            failed++;  // TODO
    }
    *rowsRead = fileLength - failed;
    printf("Create struct from file: Finished creating, failed reading %zd / %zd lines\n", failed, fileLength);
    return js;
}

/*
Handles loading database from file
*/
JakasStruktura* getDataFromFile(size_t* rowsRead) {
    char* filename = selectFile(GTK_FILE_CHOOSER_ACTION_OPEN);
    if (filename == NULL)
        return NULL;

    JakasStruktura* structuresArray;

    FILE* filestream = fopen(filename, "r");
    int fileLength = countLines(filestream);
    if (fileLength <= 0)
        return NULL;
    
    // JakasStruktura* js = malloc(fileLength * sizeof(JakasStruktura));

    structuresArray = createStructsFromFile(filestream, fileLength, rowsRead);
    printf("created structures:");
    printf("[0] %s, [14] %s\n", 
            structuresArray[0].nazwa, 
            structuresArray[14].nazwa);

    fclose(filestream);

    // printf("readFromFile: Escaped, stream closed");
    return structuresArray;
}

/*
Opens file stream and saves provided structs
Returns -1 on file opening error
*/
char saveToFile(JakasStruktura* structsArray, size_t arrayLength) {
    char* filename = selectFile(GTK_FILE_CHOOSER_ACTION_SAVE);
    if (filename == NULL)
        return NULL;

    FILE* filestream = fopen(filename, "r");
    int fileLength = countLines(filestream);
    if (fileLength <= 0)
        return NULL;

    for (int i = 0; i < arrayLength; i++) {
        if (structsArray->stan != 2) {
            fprintf(filestream, "%s;%d;%f\n", 
            structsArray->nazwa, structsArray->ilosc, structsArray->wartosc);

            structsArray = structsArray + sizeof(JakasStruktura);
        }
    }
    fclose(filestream);
    return 0;
}

void saveToFileManually(char* nazwa, int* ilosc, float* wartosc, FILE* stream) {
    fprintf(stream, "%s;%d;%f\n", nazwa, *ilosc, *wartosc);
}
