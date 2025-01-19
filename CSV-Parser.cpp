#include "CSV-Parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <ctime>




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
    // std::vector<std::string> badLines;
    {
        std::string prefix = "log_";
        char buffer[100];
        std::time_t now = std::time(nullptr);
        std::strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H-%M-%S", std::localtime(&now));
        std::string date(buffer);
        std::string logFilename = prefix + date + ".txt";
        std::ofstream logFile(logFilename, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Error: Could not open file " << logFilename << std::endl;
        }
        logFile << "File: " << filename << std::endl;

        while (!file.eof()) {
            try {
                file >> it;
                records.push_back(it);
                logFile << "Linia " << i << ": OK" << std::endl;
            } catch (const std::exception& e) {
                std::string additionalInfo = " <Line: " + std::to_string(i) + ", file: " + filename + "> ";
                std::string logMessage = additionalInfo + "Cause: " + e.what() + "\n";
                std::cerr << logMessage;
                std::string badFilename = prefix + "error_" + date + ".txt";
                std::ofstream badFile(badFilename, std::ios::app);
                logFile << "Linia: " << i << " " << logMessage;

                badFile << logMessage;
                // badLines.push_back("");
            }


            // std::cout << "Linia: " << i++ << " ";
            // std::cout << it << std::endl;
            i++;
        }
    }

    return records;
}