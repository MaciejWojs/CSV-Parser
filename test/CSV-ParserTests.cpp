#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <iostream>
#include "../CSV-Parser.hpp"  // Include your CSV parser header file

TEST(CSVParserTests, ParseSingleLine) {
    const std::string filename = "ParseSingleLine.csv";
    std::ofstream file(filename, std::ios::out);

    file << "Time,Autokonsumpcja (W),Eksport (W),Import (W),Pobór (W),Produkcja (W)";
    CSVParser parser(filename);
    std::vector<Record> records = parser.parse();

    ASSERT_TRUE(records.empty());
    std::remove(filename.c_str());
}

TEST(CSVParserTests, ParseMultipleLines) {
    const std::string filename = "ParseMultipleLines.csv";
    std::ofstream file(filename, std::ios::out);
    CSVParser parser(filename);

    file << "Time,Autokonsumpcja (W),Eksport (W),Import (W),Pobór (W),Produkcja (W)\n";
    file << "01.10.2020 8:00,\"5.6667\",\"0\",\"1054.6588\",\"1060.3254\",\"5.6667\"\n";
    file << "01.10.2020 8:15,\"17.6667\",\"0\",\"1764.5107\",\"1782.1774\",\"17.6667\"";
    file.close();
    std::vector<Record> records = parser.parse();

    std::cout << records.size() << std::endl;

    ASSERT_TRUE(records.size() == 2);
    std::remove(filename.c_str());
}
