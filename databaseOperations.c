#include "databaseOperations.h"
#include "FileOperations/fileOperations.h"

/*
Reads data from chosen source, populates warehouse
Returns result of data retrievement
*/
unsigned char readDatabase(unsigned char source, JakasStruktura** warehouseArray, size_t* warehouseLength) {
    unsigned char result;
    switch (source) {
        case 0:
            result = getDataFromFile(&warehouseArray, &warehouseLength);
            break;
        case 1:
            //result = readFromDatabase1(warehouseArray, warehouseLength);
            break;
    }
    return result;
}


