#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ranges>
#include "utils.hpp"
#include "Record.hpp"

Record::Record() :
    Time{},
    AutoConsumption{},
    Export{},
    Import{},
    Consumption{},
    Production{} {
    //Default constructor
};

std::istream& operator>>(std::istream& is, Record& Record) {
    std::string line;
    if (!std::getline(is, line) || line.empty()) {
        return is;
    }
    line = trimAllQuotationMarks(line);
    std::cout << line << std::endl;

    std::istringstream ss(line);

    std::getline(ss, Record.Time, ',');
    ss >> Record.AutoConsumption;
    ss.ignore(1);
    ss >> Record.Export;
    ss.ignore(1);
    ss >> Record.Import;
    ss.ignore(1);
    ss >> Record.Consumption;
    ss.ignore(1);
    ss >> Record.Production;
    return is;
}

std::ostream& operator<<(std::ostream& os, const Record& Record) {
    os << "Time: " << Record.Time << " AutoConsumption: " << Record.AutoConsumption << " Export: " << Record.Export
        << " Import: " << Record.Import << " Consumption: " << Record.Consumption << " Production: " << Record.Production;
    return os;
}