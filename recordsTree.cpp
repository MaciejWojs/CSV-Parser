#include "./recordsTree.hpp"
#include "./Record.hpp"
#include "./CSV-Parser.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

// RecordsTree::RecordsTree() {}
Quarter RecordsTree::getQuarter(const std::string& time) {
    std::istringstream ss(time);
    std::tm tm = {};
    ss >> std::get_time(&tm, "%H:%M");

    int hour = tm.tm_hour;
    if (hour < 6) return Quarter::Q1;
    if (hour < 12) return Quarter::Q2;
    if (hour < 18) return Quarter::Q3;
    return Quarter::Q4;
}

void RecordsTree::parseDateTime(const time_t& time, int& year, int& month, int& day, Quarter& quarter) {
    std::tm tm = *std::localtime(&time);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y");
    year = std::stoi(oss.str());
    oss.str("");

    oss << std::put_time(&tm, "%m");
    month = std::stoi(oss.str());
    oss.str("");

    oss << std::put_time(&tm, "%d");
    day = std::stoi(oss.str());
    oss.str("");

    std::ostringstream timeStream;
    timeStream << std::put_time(&tm, "%H:%M");
    quarter = getQuarter(timeStream.str());
}

void RecordsTree::addRecord(const Record& record) {
    int year, month, day;
    Quarter quarter;
    parseDateTime(record.getTime(), year, month, day, quarter);

    years[year].months[month].days[day].quarters[static_cast<int>(quarter)].records.push_back(record);
}

void RecordsTree::print(void(*callable)(const Record&) = nullptr) const {
    for (const auto& [year, yearNode] : years) {
        std::cout << "Rok: " << year << "\n";
        for (const auto& [month, monthNode] : yearNode.months) {
            std::cout << "  Miesiąc: " << month << "\n";
            for (const auto& [day, dayNode] : monthNode.days) {
                std::cout << "    Dzień: " << day << "\n";
                for (const auto& [quarter, quarterNode] : dayNode.quarters) {
                    std::cout << "      Ćwiartka ";
                    switch (static_cast<Quarter>(quarter)) {
                    case Quarter::Q1:
                        std::cout << "1 (00:00-05:45)";
                        if (callable) {
                            for (const auto& record : quarterNode.records) {
                                callable(record);
                            }
                        }
                        break;
                    case Quarter::Q2:
                        std::cout << "2 (06:00-11:45)";
                        if (callable) {
                            for (const auto& record : quarterNode.records) {
                                callable(record);
                            }
                        }
                        break;
                    case Quarter::Q3:
                        std::cout << "3 (12:00-17:45)";
                        if (callable) {
                            for (const auto& record : quarterNode.records) {
                                callable(record);
                            }
                        }
                        break;
                    case Quarter::Q4:
                        std::cout << "4 (18:00-23:45)";
                        if (callable) {
                            for (const auto& record : quarterNode.records) {
                                callable(record);
                            }
                        }
                        break;
                    }
                    std::cout << ": " << quarterNode.records.size() << " rekordów\n";
                }
            }
        }
    }
}

void RecordsTree::printTree() const {
    print();
}

void RecordsTree::printRecord() {
    print([](const Record& record) {
        std::cout << record << std::endl;
        });
}

void RecordsTree::sortRecords() {
    for (auto& [year, yearNode] : years) {
        for (auto& [month, monthNode] : yearNode.months) {
            for (auto& [day, dayNode] : monthNode.days) {
                for (auto& [quarter, quarterNode] : dayNode.quarters) {
                    std::sort(quarterNode.records.begin(), quarterNode.records.end(), [&](const Record& a, const Record& b) {
                        return a.getTime() < b.getTime();
                        });
                }
            }
        }
    }
}