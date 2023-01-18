#include "databaseOperations.h"
#include <stdio.h>
#include <gtk/gtk.h>

typedef struct cokolwiek{
    char* cosss[100];
    int v;
} JakasStruktura;

static GtkWidget* fillWarehouse(JakasStruktura* js) {


    
    
    fclose(file);

    // int rowsCount = sizeof(js)/sizeof(js[0]);
    // for (int i = 0; i < rowsCount; i++) {
        // gtk_tree_view_insert_column_with_attributes(
        //     GTK_TREE_VIEW(resultTreeView),
        //     i,
        //     column
        // );
    // }

    // return resultTreeView;
}

static void showItems(GtkWidget** ) {
    GtkWidget* listBox = createListbox();
    
    fillListBox(listBox, );
    gtk_list_box_insert();
}

// Todo zmienić "read from file" na "read" 
// - w myśl mvvm

int countLines(FILE* file)
{
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


void readFromFile() {
    FILE* filestream = selectFile();
    int fileLength = countLines(filestream);

    JakasStruktura* js = createStructsFromFile(filestream, fileLength);

    GtkWidget* listBox = fillWarehouse();
}

JakasStruktura* createStructsFromFile(FILE* filestream, size_t fileLength){
    JakasStruktura* js = malloc(fileLength * sizeof(JakasStruktura));
    JakasStruktura* currentItem;
    size_t failed = 0;
    size_t j = 0;
    for (size_t i = 0; i < fileLength; i++) {

        if ((currentItem = checkLine(currentLine)) != NULL) {
            js[j] = currentItem;
            j++;
        }
        else
            failed++;
    }
    return js;
}

    
    //GtkWidget* listBox = fillWarehouse(js);
    // showItems(listBox);
