#include "CSV-Parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>




CSVParser::CSVParser(const std::string& filename) : filename(filename) {}

std::vector<Record> CSVParser::parse() {
    std::vector<Record> records;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return records;
    }


    std::string line;
    // std::istringstream ss(line);
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Record it;
    // file >> it;

    int i = 2;
    while (file >> it) {
        std::cout << "Linia: " << i++ << " ";
        std::cout << it << std::endl;
        records.push_back(it);
    }


    return records;
}