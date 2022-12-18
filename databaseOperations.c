#include "databaseOperations.h"
#include <stdio.h>
#include <gtk/gtk.h>

typedef struct cokolwiek{
    char* cosss[100];
    int v;
} JakasStruktura;

static GtkWidget* fillWarehouse(JakasStruktura js[]) {

    // definiuję kolumny, a nie, 
    // tak jak mówi nazwa funkcji, wypełniam danymi, 
    // REFACTOR

    GtkCellRenderer* renderer;
    GtkWidget* resultTreeView = gtk_tree_view_new();
    int rowsCount = sizeof(js)/sizeof(js[0]);
    for (int i = 0; i < rowsCount; i++) {
        renderer = gtk_cell_renderer_text_new();
        // gtk_tree_view_insert_column_with_attributes(
        //     GTK_TREE_VIEW(resultTreeView),
        //     i,
        //     column
        // );
    }

    return resultTreeView;
}

static void showItems() {

}

// Todo zmienić "read from file" na "read" 
// - w myśl mvvm

void readFromFile(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
    // printf("Brak pliku z danymi\n");
    // return 0;
    }
    
    fclose(file);

    JakasStruktura js[10] = (JakasStruktura*)malloc(sizeof(JakasStruktura));
    fillWarehouse(js);
    showItems();
}
