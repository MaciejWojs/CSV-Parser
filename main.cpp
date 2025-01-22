#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include "CSV-Parser.hpp"
#include "recordsTree.hpp"

int main() {
    CSVParser parser("Chart-Export.csv");
    std::vector<Record> records = parser.parse();


    for (const auto& record : records) {
        std::cout << record << std::endl;
    }

    std::cout << "Ilosc rekordow: " << records.size() << std::endl;

    RecordsTree tree;
    for (const auto& record : records) {
        tree.addRecord(record);
    }

    tree.sortRecords();
    tree.printTree();
    tree.printRecord();

    return 0;
}