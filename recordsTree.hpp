#ifndef C943C827_7391_4D56_A271_0E6494FEC661
#define C943C827_7391_4D56_A271_0E6494FEC661

#include "./Record.hpp"
#include <map>
#include <memory>
#include <vector>
#include <functional>

enum class Quarter {
    Q1,
    Q2,
    Q3,
    Q4
};

enum class Error {
    EmptyRecords,
    EmptyTime,
    SameTime,
    NoError
};

struct QuarterNode {
    std::vector<Record> records;
};

struct DayNode {
    std::map<int, QuarterNode> quarters; // 0-3 dla każdej ćwiartki dnia
};

struct MonthNode {
    std::map<int, DayNode> days;
};

struct YearNode {
    std::map<int, MonthNode> months;
};

class RecordsTree {
    private:
    std::map<int, YearNode> years;

    Quarter getQuarter(const std::string& time);
    void parseDateTime(const time_t& time, int& year, int& month, int& day, Quarter& quarter);
    void print(std::function<void(const Record&)> callable) const;
    double Query(std::function<double(const std::vector<Record>&)> func);
    Error checkError(const std::vector<Record>& records, const std::string& time1, const std::string& time2);


    public:
    int getNumberOfItemsBetweenTimes(const std::vector<Record>& records, const std::string& time1, const std::string& time2);
    void printRecord() const;
    void printRecordsBetweenTimes(const std::string& time1, const std::string& time2) const;
    void sortRecords();
    void addRecord(const Record& record);
    void printTree() const;

    double getAutoConsumptionSum(const std::vector<Record>& records, const std::string& time1, const std::string& time2);
    double getExportSum(const std::vector<Record>& records, const std::string& time1, const std::string& time2);
    double getImportSum(const std::vector<Record>& records, const std::string& time1, const std::string& time2);
    double getConsumptionSum(const std::vector<Record>& records, const std::string& time1, const std::string& time2);
    double getProductionSum(const std::vector<Record>& records, const std::string& time1, const std::string& time2);

    double getAutoConsumptionAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2);
    double getExportAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2);
    double getImportAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2);
    double getConsumptionAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2);
    double getProductionAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2);
};

#endif
