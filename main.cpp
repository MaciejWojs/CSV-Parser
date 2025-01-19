#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include "Record.hpp"
#define FILENAME "Chart-Export2.csv"

int main() {
    std::ifstream file(FILENAME);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << FILENAME << std::endl;
        return 1;
    }

    // int i = 0;
    std::string line;
    // std::istringstream ss(line);
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Record it;
    file >> it;
    // while (file >> it) {
    std::cout << it << std::endl;
    // if (i++ == 10) break;
// }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}