#include "databaseOperations.h"
#include "Structures/structures.h"
#include "FileOperations/fileOperations.h"
#include <stdio.h>
#include <gtk/gtk.h>

unsigned char retrieveData(unsigned char source, JakasStruktura* warehouseArray, size_t warehouseLength){
    unsigned char result;
    switch (source) {
        case 0:
            result = readFromFile(&warehouseArray, &warehouseLength);
            break;
        case 1:
            result = readFromDatabase1(&warehouseArray, &warehouseLength);
            break;
    }
    return result;
}

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


