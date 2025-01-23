#include "./recordsTree.hpp"
#include "./Record.hpp"
#include "./CSV-Parser.hpp"
#include "./utils.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ranges>
#include <numeric>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <functional>

inline void run_callables(const std::vector<Record>& records, std::function<void(const Record&)> callable = nullptr) {
    if (callable) {
        for (const auto& record : records) {
            callable(record);
        }
    }
}

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

void RecordsTree::print(std::function<void(const Record&)> callable = nullptr) const {
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
                        run_callables(quarterNode.records, callable);
                        break;
                    case Quarter::Q2:
                        std::cout << "2 (06:00-11:45)";
                        run_callables(quarterNode.records, callable);
                        break;
                    case Quarter::Q3:
                        std::cout << "3 (12:00-17:45)";
                        run_callables(quarterNode.records, callable);
                        break;
                    case Quarter::Q4:
                        std::cout << "4 (18:00-23:45)";
                        run_callables(quarterNode.records, callable);
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

void RecordsTree::printRecord() const {
    print([](const Record& record) {
        std::cout << record << std::endl;
        });
}

void RecordsTree::sortRecords() {
    for (auto& [year, yearNode] : years) {
        for (auto& [month, monthNode] : yearNode.months) {
            for (auto& [day, dayNode] : monthNode.days) {
                for (auto& [quarter, quarterNode] : dayNode.quarters) {
                    std::ranges::sort(quarterNode.records, [&](const Record& a, const Record& b) {
                        return a.getTime() < b.getTime();
                        });
                }
            }
        }
    }
}

double RecordsTree::Query(std::function<double(const std::vector<Record>&)> func) const {
    double total = 0.0;
    for (auto& [year, yearNode] : years) {
        for (auto& [month, monthNode] : yearNode.months) {
            for (auto& [day, dayNode] : monthNode.days) {
                for (auto& [quarter, quarterNode] : dayNode.quarters) {
                    total += func(quarterNode.records);
                }
            }
        }
    }
    return total;
}


double RecordsTree::getAutoConsumptionSum(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    if (checkError(records, time1, time2) != Error::NoError) {
        return 0.0;
    }
    // convert time1 and time2 to time_t
    time_t time1_t = convertToTimeT(time1);
    time_t time2_t = convertToTimeT(time2);


    return Query([time1_t, time2_t](const std::vector<Record>& records) {
        return std::accumulate(records.begin(), records.end(), 0.0, [&](double sum, const Record& record) {
            time_t recordTime = record.getTime();
            if (recordTime >= time1_t && recordTime <= time2_t) {
                // std::cout << record << std::endl;
                return sum + record.getAutoConsumption();
            }
            return sum;
            });
        });
};


double RecordsTree::getExportSum(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    if (checkError(records, time1, time2) != Error::NoError) {
        return 0.0;
    }
    // convert time1 and time2 to time_t
    time_t time1_t = convertToTimeT(time1);
    time_t time2_t = convertToTimeT(time2);

    return Query([time1_t, time2_t](const std::vector<Record>& records) {
        return std::accumulate(records.begin(), records.end(), 0.0, [&](double sum, const Record& record) {
            time_t recordTime = record.getTime();
            if (recordTime >= time1_t && recordTime <= time2_t) {
                return sum + record.getExport();
            }
            return sum;
            });
        });
};

double RecordsTree::getImportSum(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    if (checkError(records, time1, time2) != Error::NoError) {
        return 0.0;
    }

    // convert time1 and time2 to time_t
    time_t time1_t = convertToTimeT(time1);
    time_t time2_t = convertToTimeT(time2);

    return Query([time1_t, time2_t](const std::vector<Record>& records) {
        return std::accumulate(records.begin(), records.end(), 0.0, [&](double sum, const Record& record) {
            time_t recordTime = record.getTime();
            if (recordTime >= time1_t && recordTime <= time2_t) {
                return sum + record.getImport();
            }
            return sum;
            });
        });
};

double RecordsTree::getConsumptionSum(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    if (checkError(records, time1, time2) != Error::NoError) {
        return 0.0;
    }

    // convert time1 and time2 to time_t
    time_t time1_t = convertToTimeT(time1);
    time_t time2_t = convertToTimeT(time2);

    return Query([time1_t, time2_t](const std::vector<Record>& records) {
        return std::accumulate(records.begin(), records.end(), 0.0, [&](double sum, const Record& record) {
            time_t recordTime = record.getTime();
            if (recordTime >= time1_t && recordTime <= time2_t) {
                return sum + record.getConsumption();
            }
            return sum;
            });
        });
};


double RecordsTree::getProductionSum(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    if (checkError(records, time1, time2) != Error::NoError) {
        return 0.0;
    }

    // convert time1 and time2 to time_t
    time_t time1_t = convertToTimeT(time1);
    time_t time2_t = convertToTimeT(time2);

    return Query([time1_t, time2_t](const std::vector<Record>& records) {
        return std::accumulate(records.begin(), records.end(), 0.0, [&](double sum, const Record& record) {
            time_t recordTime = record.getTime();
            if (recordTime >= time1_t && recordTime <= time2_t) {
                return sum + record.getProduction();
            }
            return sum;
            });
        });
};

Error RecordsTree::checkError(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    if (records.empty()) {
        std::cerr << "Brak rekordów w drzewie\n";
        return Error::EmptyRecords;
    }

    if (time1.empty() || time2.empty()) {
        std::cerr << "Brak czasu\n";
        return Error::EmptyTime;
    }

    if (time1 == time2) {
        std::cerr << "Czas początkowy i końcowy są takie same\n";
        return Error::SameTime;
    }

    return Error::NoError;
}

int RecordsTree::getNumberOfItemsBetweenTimes(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    if (checkError(records, time1, time2) != Error::NoError) {
        return 0;
    }

    // convert time1 and time2 to time_t
    time_t time1_t = convertToTimeT(time1);
    time_t time2_t = convertToTimeT(time2);

    return std::count_if(records.begin(), records.end(), [&](const Record& record) {
        time_t recordTime = record.getTime();
        return recordTime >= time1_t && recordTime <= time2_t;
        });
}

void RecordsTree::printRecordsBetweenTimes(const std::string& time1, const std::string& time2) const {
    if (time1.empty() || time2.empty()) {
        std::cerr << "Brak czasu\n";
        return;
    }

    if (time1 == time2) {
        std::cerr << "Czas początkowy i końcowy są takie same\n";
        return;
    }

    for (const auto& [year, yearNode] : years) {
        for (const auto& [month, monthNode] : yearNode.months) {
            for (const auto& [day, dayNode] : monthNode.days) {
                for (const auto& [quarter, quarterNode] : dayNode.quarters) {
                    for (const auto& record : quarterNode.records) {
                        time_t recordTime = record.getTime();
                        if (recordTime >= convertToTimeT(time1) && recordTime <= convertToTimeT(time2)) {
                            std::cout << record << std::endl;
                        }
                    }
                }
            }
        }
    }
}

double RecordsTree::getAutoConsumptionAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    return getAutoConsumptionSum(records, time1, time2) / getNumberOfItemsBetweenTimes(records, time1, time2);
}

double RecordsTree::getExportAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    return getExportSum(records, time1, time2) / getNumberOfItemsBetweenTimes(records, time1, time2);
}

double RecordsTree::getImportAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    return getImportSum(records, time1, time2) / getNumberOfItemsBetweenTimes(records, time1, time2);
}

double RecordsTree::getConsumptionAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    return getConsumptionSum(records, time1, time2) / getNumberOfItemsBetweenTimes(records, time1, time2);
}

double RecordsTree::getProductionAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    return getProductionSum(records, time1, time2) / getNumberOfItemsBetweenTimes(records, time1, time2);
}