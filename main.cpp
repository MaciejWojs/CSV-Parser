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
<<<<<<< HEAD

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
=======

    // tree.printRecordsBetweenTimes("01.10.2020 8:00", "01.10.2020 9:30");

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


    tree.printRecordsBetweenTimes("01.10.2020 8:00", "01.10.2020 9:30");
    tree.compareAutoConsumption(records, "01.10.2020 8:00", "01.10.2020 9:30", "01.10.2020 9:45", "01.10.2020 10:45");
>>>>>>> 8315efa (switch case menu)
    // tree.compareExport(records, "01.10.2020 8:00", "01.10.2020 9:30", "01.10.2020 9:45", "01.10.2020 10:45");
    // tree.compareImport(records, "01.10.2020 8:00", "01.10.2020 9:30", "01.10.2020 9:45", "01.10.2020 10:45");
    // tree.compareProduction(records, "01.10.2020 8:00", "01.10.2020 9:30", "01.10.2020 9:45", "01.10.2020 10:45");
    // tree.compareConsumption(records, "01.10.2020 8:00", "01.10.2020 9:30", "01.10.2020 9:45", "01.10.2020 10:45");
<<<<<<< HEAD
=======

    //------------------------------------------------------------------------------------------------------------

    int operation;

    while (true) {
        std::cout << "1. Wyswietl rekordy pomiedzy dwoma godzinami" << std::endl;
        std::cout << "2. Porownaj sumy i srednie autokonsumpcji" << std::endl;
        std::cout << "3. Porownaj sumy i srednie eksportu" << std::endl;
        std::cout << "4. Porownaj sumy i srednie importu" << std::endl;
        std::cout << "5. Porownaj sumy i srednie konsumpcji" << std::endl;
        std::cout << "6. Porownaj sumy i srednie produkcji" << std::endl;
        std::cout << "7. Wyjdz" << std::endl;

        std::cin >> operation;

        switch (operation) {
        case 1:
            {
                std::string time1, time2;
                std::cout << "Podaj pierwsza godzine: ";
                std::cin >> time1;
                std::cout << "Podaj druga godzine: ";
                std::cin >> time2;

                tree.printRecordsBetweenTimes(time1, time2);
                break;
            }
        case 2:
            {
                std::string time1_begin, time1_end, time2_begin, time2_end;
                std::cout << "Podaj poczatek pierwszego okresu: ";
                std::cin >> time1_begin;
                std::cout << "Podaj koniec pierwszego okresu: ";
                std::cin >> time1_end;
                std::cout << "Podaj poczatek drugiego okresu: ";
                std::cin >> time2_begin;
                std::cout << "Podaj koniec drugiego okresu: ";
                std::cin >> time2_end;

                tree.compareAutoConsumption(records, time1_begin, time1_end, time2_begin, time2_end);
                break;
            }
        case 3:
            {
                std::string time1_begin, time1_end, time2_begin, time2_end;
                std::cout << "Podaj poczatek pierwszego okresu: ";
                std::cin >> time1_begin;
                std::cout << "Podaj koniec pierwszego okresu: ";
                std::cin >> time1_end;
                std::cout << "Podaj poczatek drugiego okresu: ";
                std::cin >> time2_begin;
                std::cout << "Podaj koniec drugiego okresu: ";
                std::cin >> time2_end;

                tree.compareExport(records, time1_begin, time1_end, time2_begin, time2_end);
                break;
            }
        case 4:
            {
                std::string time1_begin, time1_end, time2_begin, time2_end;
                std::cout << "Podaj poczatek pierwszego okresu: ";
                std::cin >> time1_begin;
                std::cout << "Podaj koniec pierwszego okresu: ";
                std::cin >> time1_end;
                std::cout << "Podaj poczatek drugiego okresu: ";
                std::cin >> time2_begin;
                std::cout << "Podaj koniec drugiego okresu: ";
                std::cin >> time2_end;

                tree.compareImport(records, time1_begin, time1_end, time2_begin, time2_end);
                break;
            }
        case 5:
            {
                std::string time1_begin, time1_end, time2_begin, time2_end;
                std::cout << "Podaj poczatek pierwszego okresu: ";
                std::cin >> time1_begin;
                std::cout << "Podaj koniec pierwszego okresu: ";
                std::cin >> time1_end;
                std::cout << "Podaj poczatek drugiego okresu: ";
                std::cin >> time2_begin;
                std::cout << "Podaj koniec drugiego okresu: ";
                std::cin >> time2_end;

                tree.compareConsumption(records, time1_begin, time1_end, time2_begin, time2_end);
                break;
            }
        case 6:
            {
                std::string time1_begin, time1_end, time2_begin, time2_end;
                std::cout << "Podaj poczatek pierwszego okresu: ";
                std::cin >> time1_begin;
                std::cout << "Podaj koniec pierwszego okresu: ";
                std::cin >> time1_end;
                std::cout << "Podaj poczatek drugiego okresu: ";
                std::cin >> time2_begin;
                std::cout << "Podaj koniec drugiego okresu: ";
                std::cin >> time2_end;

                tree.compareProduction(records, time1_begin, time1_end, time2_begin, time2_end);
                break;
            }
        case 7:
            return 0;
        default:
            std::cout << "Niepoprawna operacja" << std::endl;
            break;
        }
    }





>>>>>>> 8315efa (switch case menu)
    return 0;
}