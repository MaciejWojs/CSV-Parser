#include "./utils.hpp"
#include <string>
#include <ranges>
#include <algorithm>



std::string trimAllQuotationMarks(const std::string& str) {
    auto copy = str;
    auto const first = std::ranges::remove(copy, '"');
    copy.erase(first.begin(), first.end());
    return copy;
}
