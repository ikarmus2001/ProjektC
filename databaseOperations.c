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
            break;
        case 1:
            //result = readFromDatabase1(warehouseArray, warehouseLength);
            break;
    }
    return warehouseArray;
}
