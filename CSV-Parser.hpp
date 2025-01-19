#ifndef DCC709D6_440B_4E5C_8368_658E24487EFD
#define DCC709D6_440B_4E5C_8368_658E24487EFD

#include <vector>
#include <string>
#include "Record.hpp"

class CSVParser {
    public:
    CSVParser(const std::string& filename);
    std::vector<Record> parse();

    private:
    std::string filename;
};

#endif /* DCC709D6_440B_4E5C_8368_658E24487EFD */