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

        // std::cerr << "Error: Could not read line from file or empty line" << std::endl;
        // exit(1);
        // return is;
        throw std::runtime_error("Could not read line from file");

    }
    line = trimAllQuotationMarks(line);
    std::cout << line << std::endl;

    std::istringstream ss(line);

    std::getline(ss, Record.Time, ',');
    std::string value;

    try {
        std::getline(ss, value, ',');
        Record.AutoConsumption = std::stod(value);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Could not convert string to double in AutoConsumption [second field]"));
    }
    try {
        std::getline(ss, value, ',');
        Record.Export = std::stod(value);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Could not convert string to double in Export [third field]"));
    }

    try {
        std::getline(ss, value, ',');
        Record.Import = std::stod(value);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Could not convert string to double in Import [fourth field]"));
    }

    try {
        std::getline(ss, value, ',');
        Record.Consumption = std::stod(value);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Could not convert string to double in Consumption [fifth field]"));
    }

    try {
        std::getline(ss, value, ',');
        Record.Production = std::stod(value);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Could not convert string to double in Production [sixth field]"));
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const Record& Record) {
    os << "Time: " << Record.Time << " AutoConsumption: " << Record.AutoConsumption << " Export: " << Record.Export
        << " Import: " << Record.Import << " Consumption: " << Record.Consumption << " Production: " << Record.Production;
    return os;
}