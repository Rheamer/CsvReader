#include "CsvLibrary/CsvNokia.h"
#include <string>
#include <fstream>
#include <vector>

int main(int argc, char *argv[]){
    std::ifstream file;    
    if (argc > 1)
        file.open(argv[1]);
    if (!file.is_open()) {
        std::cout << "Error opening file" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::vector <std::string> csvString;

    CsvNokia table(file);
    table.print();
    table.printDroppedExpressions();
    file.close();
}