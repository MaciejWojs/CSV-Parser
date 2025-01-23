/**
 * @file CSV-Parser.hpp
 * @~english
 * @brief Header file containing the CSVParser class for parsing CSV files into records.
 *
 * @~polish
 * @brief Plik nagłówkowy zawierający klasę CSVParser do parsowania plików CSV na rekordy.
 */

#ifndef DCC709D6_440B_4E5C_8368_658E24487EFD
#define DCC709D6_440B_4E5C_8368_658E24487EFD

#include <vector>
#include <string>
#include "Record.hpp"

/**
 * @~english
 * @brief Class for parsing CSV files into records.
 *
 * @~polish
 * @brief Klasa do parsowania plików CSV na rekordy.
 */
class CSVParser {
    public:
    /**
     * @~english
     * @brief Constructor that initializes the parser with the given filename.
     * @param filename The name of the CSV file to parse.
     *
     * @~polish
     * @brief Konstruktor inicjalizujący parser z podaną nazwą pliku.
     * @param filename Nazwa pliku CSV do parsowania.
     */
    CSVParser(const std::string& filename);

    /**
     * @~english
     * @brief Parses the CSV file and returns a vector of records.
     * @return A vector of parsed records.
     *
     * @~polish
     * @brief Parsuje plik CSV i zwraca wektor rekordów.
     * @return Wektor sparsowanych rekordów.
     */
    std::vector<Record> parse();

    private:
    std::string filename; ///< @~english The name of the CSV file to parse @~polish Nazwa pliku CSV do parsowania
};

#endif /* DCC709D6_440B_4E5C_8368_658E24487EFD */