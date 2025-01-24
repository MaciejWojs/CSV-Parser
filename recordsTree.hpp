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
#include "RecordsTreeIterator.hpp"
#include <map>
#include <memory>
#include <vector>
#include <functional>

class RecordsTreeIterator;
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
    AutoConsumption = 1,
    /**
     * @~polish
     * @brief Wyszukiwanie według eksportu.
     * @~english
     * @brief Search by export.
     */
    Export,
    /**
     * @~polish
     * @brief Wyszukiwanie według importu.
     * @~english
     * @brief Search by import.
     */
    Import,
    /**
     * @~polish
     * @brief Wyszukiwanie według poboru.
     * @~english
     * @brief Search by consumption.
     */
    Consumption,
    /**
     * @~polish
     * @brief Wyszukiwanie według produkcji.
     * @~english
     * @brief Search by production.
     */
    Production,
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
 * @class RecordsTree
 * @brief A class representing a tree structure for storing and querying records.
 *
 * Klasa reprezentująca strukturę drzewa do przechowywania i zapytań rekordów.
 */


class RecordsTree {
    private:
    std::map<int, YearNode> years;
    friend class RecordsTreeIterator;

    /**
   * @brief Get the quarter of the year from a given time string.
   *
   * Pobiera kwartał roku z podanego ciągu znaków czasu.
   *
   * @param time The time string.
   * @return Quarter The quarter of the year.
   */
    Quarter getQuarter(const std::string& time);

    /**
     * @brief Parse a time_t object into year, month, day, and quarter.
     *
     * Parsuje obiekt time_t na rok, miesiąc, dzień i kwartał.
     *
     * @param time The time_t object.
     * @param year The parsed year.
     * @param month The parsed month.
     * @param day The parsed day.
     * @param quarter The parsed quarter.
     */
    void parseDateTime(const time_t& time, int& year, int& month, int& day, Quarter& quarter);

    /**
     * @brief Print records using a provided callable function.
     *
     * Drukuje rekordy za pomocą podanej funkcji wywoływalnej.
     *
     * @param callable The callable function to process each record.
     */
    void print(std::function<void(const Record&)> callable) const;

    /**
     * @brief Query records within a time range using a provided function.
     *
     * Zapytanie rekordów w określonym przedziale czasowym za pomocą podanej funkcji.
     *
     * @param func The function to process each record.
     * @param time1 The start time.
     * @param time2 The end time.
     */
    void Query(std::function<void(const Record&)> func, const std::string time1, std::string time2) const;

    /**
     * @brief Query records using a provided function and return a double result.
     *
     * Zapytanie rekordów za pomocą podanej funkcji i zwrócenie wyniku typu double.
     *
     * @param func The function to process the records.
     * @return double The result of the query.
     */
    double Query(std::function<double(const Record&)> func) const;

    /**
     * @brief Check for errors between two time strings.
     *
     * Sprawdza błędy między dwoma ciągami znaków czasu.
     *
     * @param time1 The start time.
     * @param time2 The end time.
     * @return Error The error status.
     */
    Error checkError(const std::string& time1, const std::string& time2) const;

    /**
     * @brief Compare sums and averages between two time periods.
     *
     * Porównuje sumy i średnie między dwoma okresami czasu.
     *
     * @param time1_begin The start time of the first period.
     * @param time1_end The end time of the first period.
     * @param time2_begin The start time of the second period.
     * @param time2_end The end time of the second period.
     * @param period1_sum The sum of the first period.
     * @param period2_sum The sum of the second period.
     * @param period1_avg The average of the first period.
     * @param period2_avg The average of the second period.
     */
    void compareSumsAndAverages(const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end, double period1_sum, double period2_sum, double period1_avg, double period2_avg) const;

    /**
     * @brief Check if the tree is empty.
     *
     * Sprawdza, czy drzewo jest puste.
     *
     * @return Error The error status.
     */
    Error checkIfEmpty() const;

    public:
    using iterator = RecordsTreeIterator;

    /**
     * @brief Get an iterator to the beginning of the tree.
     *
     * Pobiera iterator na początek drzewa.
     *
     * @return iterator The iterator to the beginning.
     */
    iterator begin() const;

    /**
     * @brief Get an iterator to the end of the tree.
     *
     * Pobiera iterator na koniec drzewa.
     *
     * @return iterator The iterator to the end.
     */
    iterator end() const;

    /**
     * @brief Search and print records within a time range based on a search operation and value.
     *
     * Wyszukuje i drukuje rekordy w określonym przedziale czasowym na podstawie operacji wyszukiwania i wartości.
     *
     * @param time_begin The start time.
     * @param time_end The end time.
     * @param operation The search operation.
     * @param value The value to compare.
     * @param tolerance The tolerance for comparison.
     */
    void searchAndPrint(const std::string& time_begin, const std::string& time_end, SearchOperation operation, double value, double tolerance) const;

    /**
     * @brief Get the number of items between two times.
     *
     * Pobiera liczbę elementów między dwoma czasami.
     *
     * @param time1 The start time.
     * @param time2 The end time.
     * @return int The number of items.
     */
    int getNumberOfItemsBetweenTimes(const std::string& time1, const std::string& time2) const;

    /**
     * @brief Print a single record.
     *
     * Drukuje pojedynczy rekord.
     */
    void printRecord() const;

    /**
     * @brief Print records between two times.
     *
     * Drukuje rekordy między dwoma czasami.
     *
     * @param time1 The start time.
     * @param time2 The end time.
     */
    void printRecordsBetweenTimes(const std::string& time1, const std::string& time2) const;

    /**
     * @brief Sort the records in the tree.
     *
     * Sortuje rekordy w drzewie.
     */
    void sortRecords();

    /**
     * @brief Add a record to the tree.
     *
     * Dodaje rekord do drzewa.
     *
     * @param record The record to add.
     */
    void addRecord(const Record& record);

    /**
     * @brief Print the entire tree.
     *
     * Drukuje całe drzewo.
     */
    void printTree() const;

    /**
     * @brief Get the sum of auto consumption between two times.
     *
     * Pobiera sumę auto konsumpcji między dwoma czasami.
     *
     * @param time1 The start time.
     * @param time2 The end time.
     * @return double The sum of auto consumption.
     */
    double getAutoConsumptionSum(const std::string& time1, const std::string& time2) const;

    /**
     * @brief Get the sum of export between two times.
     *
     * Pobiera sumę eksportu między dwoma czasami.
     *
     * @param time1 The start time.
     * @param time2 The end time.
     * @return double The sum of export.
     */
    double getExportSum(const std::string& time1, const std::string& time2) const;

    /**
     * @brief Get the sum of import between two times.
     *
     * Pobiera sumę importu między dwoma czasami.
     *
     * @param time1 The start time.
     * @param time2 The end time.
     * @return double The sum of import.
     */
    double getImportSum(const std::string& time1, const std::string& time2) const;

    /**
     * @brief Get the sum of consumption between two times.
     *
     * Pobiera sumę konsumpcji między dwoma czasami.
     *
     * @param time1 The start time.
     * @param time2 The end time.
     * @return double The sum of consumption.
     */
    double getConsumptionSum(const std::string& time1, const std::string& time2) const;

    /**
     * @brief Get the sum of production between two times.
     *
     * Pobiera sumę produkcji między dwoma czasami.
     *
     * @param time1 The start time.
     * @param time2 The end time.
     * @return double The sum of production.
     */
    double getProductionSum(const std::string& time1, const std::string& time2) const;

    /**
     * @brief Get the average of auto consumption between two times.
     *
     * Pobiera średnią auto konsumpcji między dwoma czasami.
     *
     * @param time1 The start time.
     * @param time2 The end time.
     * @return double The average of auto consumption.
     */
    double getAutoConsumptionAverage(const std::string& time1, const std::string& time2) const;

    /**
     * @brief Get the average of export between two times.
     *
     * Pobiera średnią eksportu między dwoma czasami.
     *
     * @param time1 The start time.
     * @param time2 The end time.
     * @return double The average of export.
     */
    double getExportAverage(const std::string& time1, const std::string& time2) const;

    /**
     * @brief Get the average of import between two times.
     *
     * Pobiera średnią importu między dwoma czasami.
     *
     * @param time1 The start time.
     * @param time2 The end time.
     * @return double The average of import.
     */
    double getImportAverage(const std::string& time1, const std::string& time2) const;

    /**
     * @brief Get the average of consumption between two times.
     *
     * Pobiera średnią konsumpcji między dwoma czasami.
     *
     * @param time1 The start time.
     * @param time2 The end time.
     * @return double The average of consumption.
     */
    double getConsumptionAverage(const std::string& time1, const std::string& time2) const;

    /**
     * @brief Get the average of production between two times.
     *
     * Pobiera średnią produkcji między dwoma czasami.
     *
     * @param time1 The start time.
     * @param time2 The end time.
     * @return double The average of production.
     */
    double getProductionAverage(const std::string& time1, const std::string& time2) const;

    /**
     * @brief Compare auto consumption between two time periods.
     *
     * Porównuje auto konsumpcję między dwoma okresami czasu.
     *
     * @param time1_begin The start time of the first period.
     * @param time1_end The end time of the first period.
     * @param time2_begin The start time of the second period.
     * @param time2_end The end time of the second period.
     */
    void compareAutoConsumption(const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const;

    /**
     * @brief Compare export between two time periods.
     *
     * Porównuje eksport między dwoma okresami czasu.
     *
     * @param time1_begin The start time of the first period.
     * @param time1_end The end time of the first period.
     * @param time2_begin The start time of the second period.
     * @param time2_end The end time of the second period.
     */
    void compareExport(const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const;

    /**
     * @brief Compare import between two time periods.
     *
     * Porównuje import między dwoma okresami czasu.
     *
     * @param time1_begin The start time of the first period.
     * @param time1_end The end time of the first period.
     * @param time2_begin The start time of the second period.
     * @param time2_end The end time of the second period.
     */
    void compareImport(const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const;

    /**
     * @brief Compare consumption between two time periods.
     *
     * Porównuje konsumpcję między dwoma okresami czasu.
     *
     * @param time1_begin The start time of the first period.
     * @param time1_end The end time of the first period.
     * @param time2_begin The start time of the second period.
     * @param time2_end The end time of the second period.
     */
    void compareConsumption(const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const;

    /**
     * @brief Compare production between two time periods.
     *
     * Porównuje produkcję między dwoma okresami czasu.
     *
     * @param time1_begin The start time of the first period.
     * @param time1_end The end time of the first period.
     * @param time2_begin The start time of the second period.
     * @param time2_end The end time of the second period.
     */
    void compareProduction(const std::string& time1_begin, const std::string& time1_end, const std::string& time2_begin, const std::string& time2_end) const;
};

#endif /* C943C827_7391_4D56_A271_0E6494FEC661 */