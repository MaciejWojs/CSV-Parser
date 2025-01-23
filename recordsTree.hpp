/**
 * @file recordsTree.hpp
 * @~english
 * @brief Header file containing the RecordsTree class and related structures for managing and querying records.
 *
 * @~polish
 * @brief Plik nagłówkowy zawierający klasę RecordsTree i powiązane struktury do zarządzania i zapytań rekordów.
 */

#ifndef C943C827_7391_4D56_A271_0E6494FEC661
#define C943C827_7391_4D56_A271_0E6494FEC661

#include "./Record.hpp"
#include <map>
#include <memory>
#include <vector>
#include <functional>

 /**
  * @~english
  * @brief Enumeration representing the quarters of a year.
  *
  * @~polish
  * @brief Wyliczenie reprezentujące kwartały roku.
  */
enum class Quarter {
    Q1,
    Q2,
    Q3,
    Q4
};

/**
 * @enum SearchOperation
 * @~polish
 * @brief Operacje wyszukiwania w drzewie rekordów.
 * @~english
 * @brief Search operations in the records tree.
 */
enum class SearchOperation {
    SearchByAutoConsumption,
    /**
     * @~polish
     * @brief Wyszukiwanie według eksportu.
     * @~english
     * @brief Search by export.
     */
    SearchByExport,
    /**
     * @~polish
     * @brief Wyszukiwanie według importu.
     * @~english
     * @brief Search by import.
     */
    SearchByImport,
    /**
     * @~polish
     * @brief Wyszukiwanie według poboru.
     * @~english
     * @brief Search by consumption.
     */
    SearchByConsumption,
    /**
     * @~polish
     * @brief Wyszukiwanie według produkcji.
     * @~english
     * @brief Search by production.
     */
    SearchByProduction,
};

/**
 * @~english
 * @brief Enumeration representing possible error states.
 *
 * @~polish
 * @brief Wyliczenie reprezentujące możliwe stany błędów.
 */
enum class Error {
    EmptyRecords,
    EmptyTime,
    SameTime,
    NoError
};

/**
 * @~english
 * @brief Structure representing a node for a quarter containing records.
 *
 * @~polish
 * @brief Struktura reprezentująca węzeł dla kwartału zawierającego rekordy.
 */
struct QuarterNode {
    std::vector<Record> records;
};

/**
 * @~english
 * @brief Structure representing a node for a day containing quarters.
 *
 * @~polish
 * @brief Struktura reprezentująca węzeł dla dnia zawierającego kwartały.
 */
struct DayNode {
    std::map<int, QuarterNode> quarters; // 0-3 dla każdej ćwiartki dnia
};

/**
 * @~english
 * @brief Structure representing a node for a month containing days.
 *
 * @~polish
 * @brief Struktura reprezentująca węzeł dla miesiąca zawierającego dni.
 */
struct MonthNode {
    std::map<int, DayNode> days;
};

/**
 * @~english
 * @brief Structure representing a node for a year containing months.
 *
 * @~polish
 * @brief Struktura reprezentująca węzeł dla roku zawierającego miesiące.
 */
struct YearNode {
    std::map<int, MonthNode> months;
};

/**
 * @~english
 * @brief Class for managing and querying records organized by year, month, day, and quarter.
 *
 * @~polish
 * @brief Klasa do zarządzania i zapytań rekordów zorganizowanych według roku, miesiąca, dnia i kwartału.
 */
class RecordsTree {
    private:
    std::map<int, YearNode> years;

    Quarter getQuarter(const std::string& time);
    void parseDateTime(const time_t& time, int& year, int& month, int& day, Quarter& quarter);
    void print(std::function<void(const Record&)> callable) const;
    double Query(std::function<double(const std::vector<Record>&)> func) const;
    Error checkError(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const;
    void compareSumsAndAverages(const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end, double period1_sum, double period2_sum, double period1_avg, double period2_avg) const;

    public:
    void searchAndPrint(const std::string& time_begin, const std::string& time_end, SearchOperation operation, double value, double tolerance) const;
    int getNumberOfItemsBetweenTimes(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const;
    void printRecord() const;
    void printRecordsBetweenTimes(const std::string& time1, const std::string& time2) const;
    void sortRecords();
    void addRecord(const Record& record);
    void printTree() const;

    double getAutoConsumptionSum(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const;
    double getExportSum(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const;
    double getImportSum(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const;
    double getConsumptionSum(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const;
    double getProductionSum(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const;

    double getAutoConsumptionAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const;
    double getExportAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const;
    double getImportAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const;
    double getConsumptionAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const;
    double getProductionAverage(const std::vector<Record>& records, const std::string& time1, const std::string& time2) const;

    void compareAutoConsumption(const std::vector<Record>& records, const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const;
    void compareExport(const std::vector<Record>& records, const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const;
    void compareImport(const std::vector<Record>& records, const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const;
    void compareConsumption(const std::vector<Record>& records, const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const;
    void compareProduction(const std::vector<Record>& records, const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const;
};

#endif /* C943C827_7391_4D56_A271_0E6494FEC661 */