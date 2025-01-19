#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include "CSV-Parser.hpp"

int main() {
    CSVParser parser("Chart-Export.csv");
    std::vector<Record> records = parser.parse();


    for (const auto& record : records) {
        std::cout << record << std::endl;
    }

    std::cout << "Ilosc rekordow: " << records.size() << std::endl;

    return 0;
}