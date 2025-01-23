/**
 * @file Record.hpp
 * @~english
 * @brief Header file containing the Record class for storing and managing energy data.
 *
 * @~polish
 * @brief Plik nagłówkowy zawierający klasę Record do przechowywania i zarządzania danymi energetycznymi.
 */

#ifndef FFCD09C6_ACDA_4273_B86D_DEDC69507119
#define FFCD09C6_ACDA_4273_B86D_DEDC69507119

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ranges>

#define FIELDS_IN_CSV 6

/**
 * @~english
 * @brief Class for storing and managing energy data records.
 *
 * @~polish
 * @brief Klasa do przechowywania i zarządzania rekordami danych energetycznych.
 */
class Record {
    time_t Time; ///< @~english Time of the record @~polish Czas rekordu
    double AutoConsumption; ///< @~english Auto consumption value @~polish Wartość autokonsumpcji
    double Export; ///< @~english Export value @~polish Wartość eksportu
    double Import; ///< @~english Import value @~polish Wartość importu
    double Consumption; ///< @~english Consumption value @~polish Wartość konsumpcji
    double Production; ///< @~english Production value @~polish Wartość produkcji

    public:
    /**
     * @~english
     * @brief Default constructor for the Record class.
     *
     * @~polish
     * @brief Domyślny konstruktor klasy Record.
     */
    Record();

    /**
     * @~english
     * @brief Gets the time of the record.
     * @return The time of the record.
     *
     * @~polish
     * @brief Pobiera czas rekordu.
     * @return Czas rekordu.
     */
    const time_t& getTime() const;

    /**
     * @~english
     * @brief Gets the auto consumption value.
     * @return The auto consumption value.
     *
     * @~polish
     * @brief Pobiera wartość autokonsumpcji.
     * @return Wartość autokonsumpcji.
     */
    double getAutoConsumption() const;

    /**
     * @~english
     * @brief Gets the export value.
     * @return The export value.
     *
     * @~polish
     * @brief Pobiera wartość eksportu.
     * @return Wartość eksportu.
     */
    double getExport() const;

    /**
     * @~english
     * @brief Gets the import value.
     * @return The import value.
     *
     * @~polish
     * @brief Pobiera wartość importu.
     * @return Wartość importu.
     */
    double getImport() const;

    /**
     * @~english
     * @brief Gets the consumption value.
     * @return The consumption value.
     *
     * @~polish
     * @brief Pobiera wartość konsumpcji.
     * @return Wartość konsumpcji.
     */
    double getConsumption() const;

    /**
     * @~english
     * @brief Gets the production value.
     * @return The production value.
     *
     * @~polish
     * @brief Pobiera wartość produkcji.
     * @return Wartość produkcji.
     */
    double getProduction() const;

    /**
     * @~english
     * @brief Overloads the output stream operator for the Record class.
     * @param os The output stream.
     * @param Record The record to output.
     * @return The output stream.
     *
     * @~polish
     * @brief Przeciąża operator strumienia wyjściowego dla klasy Record.
     * @param os Strumień wyjściowy.
     * @param Record Rekord do wyjścia.
     * @return Strumień wyjściowy.
     */
    friend std::ostream& operator<<(std::ostream& os, const Record& Record);

    /**
     * @~english
     * @brief Overloads the input stream operator for the Record class.
     * @param is The input stream.
     * @param Record The record to input.
     * @return The input stream.
     *
     * @~polish
     * @brief Przeciąża operator strumienia wejściowego dla klasy Record.
     * @param is Strumień wejściowy.
     * @param Record Rekord do wejścia.
     * @return Strumień wejściowy.
     */
    friend std::istream& operator>>(std::istream& is, Record& Record);
};

#endif /* FFCD09C6_ACDA_4273_B86D_DEDC69507119 */