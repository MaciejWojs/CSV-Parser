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
    auto numberOfItems = getNumberOfItemsBetweenTimes(records, time1, time2);
    if (numberOfItems == 0) {
        std::cerr << "Brak rekordów w podanym przedziale czasowym\n";
        return 0.0;
    }
    return getAutoConsumptionSum(records, time1, time2) / numberOfItems;
}

double RecordsTree::getExportAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    auto numberOfItems = getNumberOfItemsBetweenTimes(records, time1, time2);
    if (numberOfItems == 0) {
        std::cerr << "Brak rekordów w podanym przedziale czasowym\n";
        return 0.0;
    }
    return getExportSum(records, time1, time2) / numberOfItems;
}

double RecordsTree::getImportAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    auto numberOfItems = getNumberOfItemsBetweenTimes(records, time1, time2);
    if (numberOfItems == 0) {
        std::cerr << "Brak rekordów w podanym przedziale czasowym\n";
        return 0.0;
    }
    return getImportSum(records, time1, time2) / numberOfItems;
}

double RecordsTree::getConsumptionAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    auto numberOfItems = getNumberOfItemsBetweenTimes(records, time1, time2);
    if (numberOfItems == 0) {
        std::cerr << "Brak rekordów w podanym przedziale czasowym\n";
        return 0.0;
    }
    return getConsumptionSum(records, time1, time2) / numberOfItems;
}

double RecordsTree::getProductionAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const {
    auto numberOfItems = getNumberOfItemsBetweenTimes(records, time1, time2);
    if (numberOfItems == 0) {
        std::cerr << "Brak rekordów w podanym przedziale czasowym\n";
        return 0.0;
    }
    return getProductionSum(records, time1, time2) / numberOfItems;
}

void RecordsTree::compareAutoConsumption(const std::vector<Record>& records, const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const {
    bool error_occurred_case1 = checkError(records, time1_begin, time1_end) != Error::NoError;
    bool error_occurred_case2 = checkError(records, time2_begin, time2_end) != Error::NoError;
    bool error_occurred = error_occurred_case1 || error_occurred_case2;
    if (error_occurred) {
        return;
    }

    auto period1_sum = getAutoConsumptionSum(records, time1_begin, time1_end);
    auto period2_sum = getAutoConsumptionSum(records, time2_begin, time2_end);
    auto period1_avg = getAutoConsumptionAverage(records, time1_begin, time1_end);
    auto period2_avg = getAutoConsumptionAverage(records, time2_begin, time2_end);
    compareSumsAndAverages(time1_begin, time1_end, time2_begin, time2_end, period1_sum, period2_sum, period1_avg, period2_avg);
}

void RecordsTree::compareConsumption(const std::vector<Record>& records, const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const {
    bool error_occurred_case1 = checkError(records, time1_begin, time1_end) != Error::NoError;
    bool error_occurred_case2 = checkError(records, time2_begin, time2_end) != Error::NoError;
    bool error_occurred = error_occurred_case1 || error_occurred_case2;
    if (error_occurred) {
        return;
    }

    auto period1_sum = getConsumptionSum(records, time1_begin, time1_end);
    auto period2_sum = getConsumptionSum(records, time2_begin, time2_end);
    auto period1_avg = getConsumptionAverage(records, time1_begin, time1_end);
    auto period2_avg = getConsumptionAverage(records, time2_begin, time2_end);
    compareSumsAndAverages(time1_begin, time1_end, time2_begin, time2_end, period1_sum, period2_sum, period1_avg, period2_avg);
}

void RecordsTree::compareExport(const std::vector<Record>& records, const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const {
    bool error_occurred_case1 = checkError(records, time1_begin, time1_end) != Error::NoError;
    bool error_occurred_case2 = checkError(records, time2_begin, time2_end) != Error::NoError;
    bool error_occurred = error_occurred_case1 || error_occurred_case2;
    if (error_occurred) {
        return;
    }

    auto period1_sum = getExportSum(records, time1_begin, time1_end);
    auto period2_sum = getExportSum(records, time2_begin, time2_end);
    auto period1_avg = getExportAverage(records, time1_begin, time1_end);
    auto period2_avg = getExportAverage(records, time2_begin, time2_end);
    compareSumsAndAverages(time1_begin, time1_end, time2_begin, time2_end, period1_sum, period2_sum, period1_avg, period2_avg);
}

void RecordsTree::compareImport(const std::vector<Record>& records, const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const {
    bool error_occurred_case1 = checkError(records, time1_begin, time1_end) != Error::NoError;
    bool error_occurred_case2 = checkError(records, time2_begin, time2_end) != Error::NoError;
    bool error_occurred = error_occurred_case1 || error_occurred_case2;
    if (error_occurred) {
        return;
    }

    auto period1_sum = getImportSum(records, time1_begin, time1_end);
    auto period2_sum = getImportSum(records, time2_begin, time2_end);
    auto period1_avg = getImportAverage(records, time1_begin, time1_end);
    auto period2_avg = getImportAverage(records, time2_begin, time2_end);
    compareSumsAndAverages(time1_begin, time1_end, time2_begin, time2_end, period1_sum, period2_sum, period1_avg, period2_avg);
}

void RecordsTree::compareProduction(const std::vector<Record>& records, const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const {
    bool error_occurred_case1 = checkError(records, time1_begin, time1_end) != Error::NoError;
    bool error_occurred_case2 = checkError(records, time2_begin, time2_end) != Error::NoError;
    bool error_occurred = error_occurred_case1 || error_occurred_case2;
    if (error_occurred) {
        return;
    }

    auto period1_sum = getProductionSum(records, time1_begin, time1_end);
    auto period2_sum = getProductionSum(records, time2_begin, time2_end);
    auto period1_avg = getProductionAverage(records, time1_begin, time1_end);
    auto period2_avg = getProductionAverage(records, time2_begin, time2_end);
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
    std::vector<Record> matchingRecords;
    Query([&](const std::vector<Record>& records) {
        auto getValue = [](const Record& r, SearchOperation op) {
            switch (op) {
            case SearchOperation::SearchByAutoConsumption: return r.getAutoConsumption();
            case SearchOperation::SearchByExport: return r.getExport();
            case SearchOperation::SearchByImport: return r.getImport();
            case SearchOperation::SearchByConsumption: return r.getConsumption();
            case SearchOperation::SearchByProduction: return r.getProduction();
            default: return 0.0;
            }
            };

        std::ranges::copy_if(records, std::back_inserter(matchingRecords),
            [&](const Record& r) {
                return r.getTime() >= convertToTimeT(time_begin) &&
                    r.getTime() <= convertToTimeT(time_end) &&
                    getValue(r, operation) >= value - tolerance &&
                    getValue(r, operation) <= value + tolerance;
            });
        return 0.0;
        });

    std::cout << "Rekordy spełniające kryteria wyszukiwania:\n";
    std::ranges::for_each(matchingRecords, [](const Record& r) { std::cout << r << '\n'; });
}