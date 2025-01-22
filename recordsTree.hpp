#ifndef C943C827_7391_4D56_A271_0E6494FEC661
#define C943C827_7391_4D56_A271_0E6494FEC661

#include "./Record.hpp"
#include <map>
#include <memory>
#include <vector>

enum class Quarter {
    Q1,
    Q2,
    Q3,
    Q4
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
    void print(void(*callable)(const Record& Record)) const;

    public:
    void printRecord();
    void sortRecords();
    void addRecord(const Record& record);
    void printTree() const;
};

#endif
