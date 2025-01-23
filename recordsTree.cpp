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

RecordsTree::iterator RecordsTree::begin() const {
    return RecordsTreeIterator(const_cast<RecordsTree*>(this), false);
}

// Returns an iterator pointing to the end of the RecordsTree
RecordsTree::iterator RecordsTree::end() const {
    return RecordsTreeIterator(const_cast<RecordsTree*>(this), true);
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
    for (auto it = begin(); it != end(); ++it) {
        const auto& record = *it;
        if (callable) {
            callable(record);
        } else {
            std::cout << record << std::endl;
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

double RecordsTree::Query(std::function<double(const Record&)> func) const {
    double total = 0.0;
    for (const auto& record : *this) {
        total += func(record);
    }
    return total;
}

void RecordsTree::Query(std::function<void(const Record&)> func, const std::string time1, std::string time2) const {
    for (const auto& record : *this) {
        time_t recordTime = record.getTime();
        if (recordTime >= convertToTimeT(time1) && recordTime <= convertToTimeT(time2)) {
            func(record);
        }
    }
}


double RecordsTree::getAutoConsumptionSum(const std::string& time1, const std::string& time2) const {
    if (checkError(time1, time2) != Error::NoError) {
        return 0.0;
    }
    // convert time1 and time2 to time_t
    time_t time1_t = convertToTimeT(time1);
    time_t time2_t = convertToTimeT(time2);

    return Query([time1_t, time2_t](const Record& record) {
        time_t recordTime = record.getTime();
        bool inRange = recordTime >= time1_t && recordTime <= time2_t;
        auto finalValue = (inRange) ? record.getAutoConsumption() : 0.0;
        return finalValue;
        });
};


double RecordsTree::getExportSum(const std::string& time1, const std::string& time2) const {
    if (checkError(time1, time2) != Error::NoError) {
        return 0.0;
    }
    // convert time1 and time2 to time_t
    time_t time1_t = convertToTimeT(time1);
    time_t time2_t = convertToTimeT(time2);

    return Query([time1_t, time2_t](const Record& record) {
        time_t recordTime = record.getTime();
        bool inRange = recordTime >= time1_t && recordTime <= time2_t;
        auto finalValue = (inRange) ? record.getExport() : 0.0;
        return finalValue;
        });
};

double RecordsTree::getImportSum(const std::string& time1, const std::string& time2) const {
    if (checkError(time1, time2) != Error::NoError) {
        return 0.0;
    }

    // convert time1 and time2 to time_t
    time_t time1_t = convertToTimeT(time1);
    time_t time2_t = convertToTimeT(time2);

    return Query([time1_t, time2_t](const Record& record) {
        time_t recordTime = record.getTime();
        bool inRange = recordTime >= time1_t && recordTime <= time2_t;
        auto finalValue = (inRange) ? record.getImport() : 0.0;
        return finalValue;
        });
};

double RecordsTree::getConsumptionSum(const std::string& time1, const std::string& time2) const {
    if (checkError(time1, time2) != Error::NoError) {
        return 0.0;
    }

    // convert time1 and time2 to time_t
    time_t time1_t = convertToTimeT(time1);
    time_t time2_t = convertToTimeT(time2);

    return Query([time1_t, time2_t](const Record& record) {
        time_t recordTime = record.getTime();
        bool inRange = recordTime >= time1_t && recordTime <= time2_t;
        auto finalValue = (inRange) ? record.getConsumption() : 0.0;
        return finalValue;
        });
};


double RecordsTree::getProductionSum(const std::string& time1, const std::string& time2) const {
    if (checkError(time1, time2) != Error::NoError) {
        return 0.0;
    }

    // convert time1 and time2 to time_t
    time_t time1_t = convertToTimeT(time1);
    time_t time2_t = convertToTimeT(time2);

    return Query([time1_t, time2_t](const Record& record) {
        time_t recordTime = record.getTime();
        bool inRange = recordTime >= time1_t && recordTime <= time2_t;
        auto finalValue = (inRange) ? record.getProduction() : 0.0;
        return finalValue;
        });
};

Error RecordsTree::checkError(const std::string& time1, const std::string& time2) const {
    if (checkIfEmpty() == Error::EmptyRecords) {
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

int RecordsTree::getNumberOfItemsBetweenTimes(const std::string& time1, const std::string& time2) const {
    if (checkError(time1, time2) != Error::NoError) {
        return 0;
    }

    // convert time1 and time2 to time_t
    time_t time1_t = convertToTimeT(time1);
    time_t time2_t = convertToTimeT(time2);

    return static_cast<int>(Query([time1_t, time2_t](const Record& record) {
        time_t recordTime = record.getTime();
        if (recordTime >= time1_t && recordTime <= time2_t) {
            return 1.0;
        }
        return 0.0;
        }));

    // return std::count_if(records.begin(), records.end(), [&](const Record& record) {
    //     time_t recordTime = record.getTime();
    //     return recordTime >= time1_t && recordTime <= time2_t;
    //     });
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

    Query([](const Record& record) {
        std::cout << record << std::endl;
        }, time1, time2);
}

double RecordsTree::getAutoConsumptionAverage(const std::string& time1, const std::string& time2) const {
    if (checkError(time1, time2) != Error::NoError) {
        return 0.0;
    }

    auto numberOfItems = getNumberOfItemsBetweenTimes(time1, time2);
    if (numberOfItems == 0) {
        std::cerr << "Brak rekordów w podanym przedziale czasowym\n";
        return 0.0;
    }
    return getAutoConsumptionSum(time1, time2) / numberOfItems;
}

double RecordsTree::getExportAverage(const std::string& time1, const std::string& time2) const {
    if (checkError(time1, time2) != Error::NoError) {
        return 0.0;
    }
    auto numberOfItems = getNumberOfItemsBetweenTimes(time1, time2);
    if (numberOfItems == 0) {
        std::cerr << "Brak rekordów w podanym przedziale czasowym\n";
        return 0.0;
    }
    return getExportSum(time1, time2) / numberOfItems;
}

double RecordsTree::getImportAverage(const std::string& time1, const std::string& time2) const {
    if (checkError(time1, time2) != Error::NoError) {
        return 0.0;
    }
    auto numberOfItems = getNumberOfItemsBetweenTimes(time1, time2);
    if (numberOfItems == 0) {
        std::cerr << "Brak rekordów w podanym przedziale czasowym\n";
        return 0.0;
    }
    return getImportSum(time1, time2) / numberOfItems;
}

double RecordsTree::getConsumptionAverage(const std::string& time1, const std::string& time2) const {
    if (checkError(time1, time2) != Error::NoError) {
        return 0.0;
    }
    auto numberOfItems = getNumberOfItemsBetweenTimes(time1, time2);
    if (numberOfItems == 0) {
        std::cerr << "Brak rekordów w podanym przedziale czasowym\n";
        return 0.0;
    }
    return getConsumptionSum(time1, time2) / numberOfItems;
}

double RecordsTree::getProductionAverage(const std::string& time1, const std::string& time2) const {
    if (checkError(time1, time2) != Error::NoError) {
        return 0.0;
    }
    auto numberOfItems = getNumberOfItemsBetweenTimes(time1, time2);
    if (numberOfItems == 0) {
        std::cerr << "Brak rekordów w podanym przedziale czasowym\n";
        return 0.0;
    }
    return getProductionSum(time1, time2) / numberOfItems;
}

void RecordsTree::compareAutoConsumption(const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const {
    bool error_occurred_case1 = checkError(time1_begin, time1_end) != Error::NoError;
    bool error_occurred_case2 = checkError(time2_begin, time2_end) != Error::NoError;
    bool error_occurred = error_occurred_case1 || error_occurred_case2;
    if (error_occurred) {
        return;
    }

    auto period1_sum = getAutoConsumptionSum(time1_begin, time1_end);
    auto period2_sum = getAutoConsumptionSum(time2_begin, time2_end);
    auto period1_avg = getAutoConsumptionAverage(time1_begin, time1_end);
    auto period2_avg = getAutoConsumptionAverage(time2_begin, time2_end);
    compareSumsAndAverages(time1_begin, time1_end, time2_begin, time2_end, period1_sum, period2_sum, period1_avg, period2_avg);
}

void RecordsTree::compareConsumption(const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const {
    bool error_occurred_case1 = checkError(time1_begin, time1_end) != Error::NoError;
    bool error_occurred_case2 = checkError(time2_begin, time2_end) != Error::NoError;
    bool error_occurred = error_occurred_case1 || error_occurred_case2;
    if (error_occurred) {
        return;
    }

    auto period1_sum = getConsumptionSum(time1_begin, time1_end);
    auto period2_sum = getConsumptionSum(time2_begin, time2_end);
    auto period1_avg = getConsumptionAverage(time1_begin, time1_end);
    auto period2_avg = getConsumptionAverage(time2_begin, time2_end);
    compareSumsAndAverages(time1_begin, time1_end, time2_begin, time2_end, period1_sum, period2_sum, period1_avg, period2_avg);
}

void RecordsTree::compareExport(const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const {
    bool error_occurred_case1 = checkError(time1_begin, time1_end) != Error::NoError;
    bool error_occurred_case2 = checkError(time2_begin, time2_end) != Error::NoError;
    bool error_occurred = error_occurred_case1 || error_occurred_case2;
    if (error_occurred) {
        return;
    }

    auto period1_sum = getExportSum(time1_begin, time1_end);
    auto period2_sum = getExportSum(time2_begin, time2_end);
    auto period1_avg = getExportAverage(time1_begin, time1_end);
    auto period2_avg = getExportAverage(time2_begin, time2_end);
    compareSumsAndAverages(time1_begin, time1_end, time2_begin, time2_end, period1_sum, period2_sum, period1_avg, period2_avg);
}

void RecordsTree::compareImport(const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const {
    bool error_occurred_case1 = checkError(time1_begin, time1_end) != Error::NoError;
    bool error_occurred_case2 = checkError(time2_begin, time2_end) != Error::NoError;
    bool error_occurred = error_occurred_case1 || error_occurred_case2;
    if (error_occurred) {
        return;
    }

    auto period1_sum = getImportSum(time1_begin, time1_end);
    auto period2_sum = getImportSum(time2_begin, time2_end);
    auto period1_avg = getImportAverage(time1_begin, time1_end);
    auto period2_avg = getImportAverage(time2_begin, time2_end);
    compareSumsAndAverages(time1_begin, time1_end, time2_begin, time2_end, period1_sum, period2_sum, period1_avg, period2_avg);
}

void RecordsTree::compareProduction(const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const {
    bool error_occurred_case1 = checkError(time1_begin, time1_end) != Error::NoError;
    bool error_occurred_case2 = checkError(time2_begin, time2_end) != Error::NoError;
    bool error_occurred = error_occurred_case1 || error_occurred_case2;
    if (error_occurred) {
        return;
    }

    auto period1_sum = getProductionSum(time1_begin, time1_end);
    auto period2_sum = getProductionSum(time2_begin, time2_end);
    auto period1_avg = getProductionAverage(time1_begin, time1_end);
    auto period2_avg = getProductionAverage(time2_begin, time2_end);
    compareSumsAndAverages(time1_begin, time1_end, time2_begin, time2_end, period1_sum, period2_sum, period1_avg, period2_avg);
}


void RecordsTree::compareSumsAndAverages(const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end, double period1_sum, double period2_sum, double period1_avg, double period2_avg) const {
    std::cout << "Suma zużycia energii elektrycznej w okresie " << time1_begin << " - " << time1_end << ": " << period1_sum << " kWh\n";
    std::cout << "Suma zużycia energii elektrycznej w okresie " << time2_begin << " - " << time2_end << ": " << period2_sum << " kWh\n";
    std::cout << "Średnie zużycie energii elektrycznej w okresie " << time1_begin << " - " << time1_end << ": " << period1_avg << " kWh\n";
    std::cout << "Średnie zużycie energii elektrycznej w okresie " << time2_begin << " - " << time2_end << ": " << period2_avg << " kWh\n";

    auto diff_sum = period1_sum - period2_sum;
    auto diff_avg = period1_avg - period2_avg;
    auto diff_sum_percent = (diff_sum / period1_sum) * 100;
    auto diff_avg_percent = (diff_avg / period1_avg) * 100;

    if (period1_sum != period2_sum) {
        std::cout << "Okres " << (period1_sum > period2_sum ? time1_begin + " - " + time1_end : time2_begin + " - " + time2_end)
            << " charakteryzuje się większym zużyciem energii elektrycznej\n";
        std::cout << "Różnica w zużyciu energii elektrycznej wynosi " << diff_sum << " kWh (" << diff_sum_percent << "%)\n";
    } else {
        std::cout << "Okresy " << time1_begin << " - " << time1_end << " oraz " << time2_begin << " - " << time2_end << " charakteryzują się takim samym zużyciem energii elektrycznej\n";
    }

    if (period1_avg != period2_avg) {
        std::cout << "Okres " << (period1_avg > period2_avg ? time1_begin + " - " + time1_end : time2_begin + " - " + time2_end)
            << " charakteryzuje się większym średnim zużyciem energii elektrycznej\n";
        std::cout << "Różnica w średnim zużyciu energii elektrycznej wynosi " << diff_avg << " kWh (" << diff_avg_percent << "%)\n";
    } else {
        std::cout << "Okresy " << time1_begin << " - " << time1_end << " oraz " << time2_begin << " - " << time2_end << " charakteryzują się takim samym średnim zużyciem energii elektrycznej\n";
    }
}

void RecordsTree::searchAndPrint(const std::string& time_begin, const std::string& time_end, SearchOperation operation, double value, double tolerance) const {
    Query([&](const Record& record) {
        auto lower_bound = value - tolerance;
        auto upper_bound = value + tolerance;
        double recordValue{};


        switch (operation) {
        case SearchOperation::AutoConsumption:
            {
                recordValue = record.getAutoConsumption();
                break;
            }
        case SearchOperation::Export:
            {
                recordValue = record.getExport();
                break;
            }
        case SearchOperation::Import:
            {
                recordValue = record.getImport();
                break;
            }
        case SearchOperation::Consumption:
            {
                recordValue = record.getConsumption();
                break;
            }
        case SearchOperation::Production:
            {
                recordValue = record.getProduction();
                break;
            }
        }
        if (lower_bound <= recordValue && recordValue <= upper_bound) {
            std::cout << record << std::endl;
#ifdef DEBUG
            std::cout << record.getAutoConsumption() << std::endl;
            std::cout << "Record value: " << recordValue << std::endl;

            std::cout << "Tolerance: " << tolerance << std::endl;
            std::cout << "Value: " << value << std::endl;
            std::cout << "Lower bound: " << lower_bound << std::endl;
            std::cout << "Upper bound: " << upper_bound << std::endl;
#endif
        }
        }, time_begin, time_end);
}

Error RecordsTree::checkIfEmpty() const {
    if (years.empty()) {
        std::cerr << "Brak rekordów w drzewie\n";
        return Error::EmptyRecords;
    }

    return Error::NoError;
}