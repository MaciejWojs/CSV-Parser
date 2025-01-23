#include "./utils.hpp"
#include <string>
#include <ranges>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <iomanip>


std::string trimAllQuotationMarks(const std::string& str) {
    auto copy = str;
    auto const first = std::ranges::remove(copy, '"');
    copy.erase(first.begin(), first.end());
    return copy;
}

time_t convertToTimeT(const std::string& timeStr) {
    std::tm tm = {};
    std::istringstream ss(timeStr);
    ss >> std::get_time(&tm, "%d.%m.%Y %H:%M");
    //naprawia dodawanie 1h do czasu
    tm.tm_isdst = -1; // Let mktime determine whether daylight saving time is in effect
    return std::mktime(&tm);
}