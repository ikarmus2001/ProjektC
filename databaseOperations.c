#include "databaseOperations.h"
#include "FileOperations/fileOperations.h"

/*
Reads data from chosen source, populates warehouse
Returns result of data retrievement
*/
JakasStruktura* readDatabase(unsigned char source, size_t* warehouseLength) {
    // unsigned char result;  // TODO
    JakasStruktura* warehouseArray;
    switch (source) {
        case 0:
            warehouseArray = getDataFromFile(warehouseLength);
            printf("Found %zd items in warehouse", *warehouseLength);
            break;
        case 1:
            //result = readFromDatabase1(warehouseArray, warehouseLength);
            break;
    }
    return warehouseArray;
}

unsigned char saveDatabase(unsigned char source, JakasStruktura* warehouseArray, size_t warehouseLength) {
    unsigned char result;
    switch (source) {
        case 0:
            result = saveToFile(warehouseArray, warehouseLength);
            break;
        case 1:
            //result = saveToDatabase1(warehouseArray, warehouseLength);
            break;
    }
    return result;
}

void saveToFileManuallyLink(char* nazwa, int* ilosc, float* wartosc) {

    char* filename = selectFile(GTK_FILE_CHOOSER_ACTION_SAVE);
    if (filename == NULL)
        return;

    FILE* filestream = fopen(filename, "a");

    saveToFileManually(nazwa, ilosc, wartosc, filestream);
}