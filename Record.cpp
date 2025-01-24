#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ranges>
#include "utils.hpp"
#include "Record.hpp"
#include <ctime>
#include <iomanip>

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

        // std::cerr << "Error: Could not read line from file or empty line" << std::endl;
        // exit(1);
        // return is;
        throw std::runtime_error("Could not read line from file");

    }
    line = trimAllQuotationMarks(line);
    // std::cout << line << std::endl;

    std::istringstream ss(line);

    std::string value;
    try {
        std::getline(ss, value, ',');
        auto time = convertToTimeT(value);
        Record.Time = time;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Could not convert string to time_t in Time [first field]"));
    }

    std::vector<std::pair<std::string, double&>> fields = {
        {"AutoConsumption", Record.AutoConsumption},
        {"Export", Record.Export},
        {"Import", Record.Import},
        {"Consumption", Record.Consumption},
        {"Production", Record.Production}
    };

    for (auto& [name, field] : fields) {
        try {
            if (!std::getline(ss, value, ',')) {
                throw std::runtime_error("Missing field: " + name);
            }
            value = trimAllQuotationMarks(value);
            field = std::stod(value);
        } catch (const std::exception& e) {
            std::throw_with_nested(std::runtime_error("Could not convert string to double in " + name));
        }
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const Record& Record) {
    std::tm tm = *std::localtime(&Record.Time);
    os << "Time: " << std::put_time(&tm, "%d.%m.%Y %H:%M:%S") << " AutoConsumption: " << Record.AutoConsumption << " Export: " << Record.Export
        << " Import: " << Record.Import << " Consumption: " << Record.Consumption << " Production: " << Record.Production;
    return os;
}

const time_t& Record::getTime() const {
    return Time;
}

double Record::getAutoConsumption() const {
    return AutoConsumption;
}

double Record::getExport() const {
    return Export;
}

double Record::getImport() const {
    return Import;
}

double Record::getConsumption() const {
    return Consumption;
}

double Record::getProduction() const {
    return Production;
}