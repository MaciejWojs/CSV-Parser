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


    // for (const auto& record : records) {
    //     std::cout << record << std::endl;
    // }

    std::cout << "Ilosc rekordow: " << records.size() << std::endl;

    RecordsTree tree;
    for (const auto& record : records) {
        tree.addRecord(record);
    }

    tree.sortRecords();
    // tree.printTree();
    // tree.printRecord();

    // double sum1 = tree.getAutoConsumptionSum(records, "01.10.2020 8:00", "01.10.2020 9:30");
    // double sum2 = tree.getExportSum(records, "01.10.2020 8:00", "01.10.2020 9:30");
    // double sum3 = tree.getImportSum(records, "01.10.2020 8:00", "01.10.2020 9:30");
    // double sum4 = tree.getConsumptionSum(records, "01.10.2020 8:00", "01.10.2020 9:30");
    // double sum5 = tree.getProductionSum(records, "01.10.2020 8:00", "01.10.2020 9:30");

    tree.printRecordsBetweenTimes("01.10.2020 8:00", "01.10.2020 9:30");

    // std::cout << "getAutoConsumptionSum: " << sum1 << std::endl;
    // std::cout << "getExportSum: " << sum2 << std::endl;
    // std::cout << "getImportSum: " << sum3 << std::endl;
    // std::cout << "getConsumptionSum: " << sum4 << std::endl;
    // std::cout << "getProductionSum: " << sum5 << std::endl;

    // std::cout << "getAutoConsumptionAverage: " << tree.getAutoConsumptionAverage(records, "01.10.2020 8:00", "01.10.2020 9:30") << std::endl;
    // std::cout << "getExportAverage: " << tree.getExportAverage(records, "01.10.2020 8:00", "01.10.2020 9:30") << std::endl;
    // std::cout << "getImportAverage: " << tree.getImportAverage(records, "01.10.2020 8:00", "01.10.2020 9:30") << std::endl;
    // std::cout << "getConsumptionAverage: " << tree.getConsumptionAverage(records, "01.10.2020 8:00", "01.10.2020 9:30") << std::endl;
    // std::cout << "getProductionAverage: " << tree.getProductionAverage(records, "01.10.2020 8:00", "01.10.2020 9:30") << std::endl;


    // std::cout << "Ilosc rekordow pomiedzy 8:00 a 9:30: " << tree.getNumberOfItemsBetweenTimes(records, "01.10.2020 8:00", "01.10.2020 9:30") << std::endl;

    // tree.compareAutoConsumption(records, "01.10.2020 8:00", "01.10.2020 9:30", "01.10.2020 9:45", "01.10.2020 10:45");
    // tree.compareExport(records, "01.10.2020 8:00", "01.10.2020 9:30", "01.10.2020 9:45", "01.10.2020 10:45");
    // tree.compareImport(records, "01.10.2020 8:00", "01.10.2020 9:30", "01.10.2020 9:45", "01.10.2020 10:45");
    // tree.compareProduction(records, "01.10.2020 8:00", "01.10.2020 9:30", "01.10.2020 9:45", "01.10.2020 10:45");
    // tree.compareConsumption(records, "01.10.2020 8:00", "01.10.2020 9:30", "01.10.2020 9:45", "01.10.2020 10:45");
    return 0;
}