/**
 * @file utils.hpp
 * @~english
 * @brief Header file containing utility functions for string manipulation and time conversion.
 *
 * @~polish
 * @brief Plik nagłówkowy zawierający funkcje pomocnicze do manipulacji ciągami znaków i konwersji czasu.
 */

#ifndef F27D7DC8_74C1_4775_AB6B_1BE0B7878DF4
#define F27D7DC8_74C1_4775_AB6B_1BE0B7878DF4

#include <string>

/**
 * @~english
 * @brief Removes all quotation marks from the given string.
 * @param str The input string from which quotation marks will be removed.
 * @return A new string with all quotation marks removed.
 *
 * @~polish
 * @brief Usuwa wszystkie cudzysłowy z podanego ciągu znaków.
 * @param str Ciąg wejściowy, z którego zostaną usunięte cudzysłowy.
 * @return Nowy ciąg znaków bez cudzysłowów.
 */
std::string trimAllQuotationMarks(const std::string& str);

/**
 * @~english
 * @brief Converts a string representation of time to a time_t object.
 * @param timeStr The input string representing the time.
 * @return A time_t object representing the converted time.
 *
 * @~polish
 * @brief Konwertuje ciąg znaków reprezentujący czas na obiekt time_t.
 * @param timeStr Ciąg wejściowy reprezentujący czas.
 * @return Obiekt time_t reprezentujący przekonwertowany czas.
 */
time_t convertToTimeT(const std::string& timeStr);

#endif /* F27D7DC8_74C1_4775_AB6B_1BE0B7878DF4 */