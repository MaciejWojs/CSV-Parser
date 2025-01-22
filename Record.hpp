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

class Record {
    time_t Time;
    float AutoConsumption;
    float Export;
    float Import;
    float Consumption;
    float Production;

    public:
    Record();
    const time_t& getTime() const;
    friend std::ostream& operator<<(std::ostream& os, const Record& Record);
    friend std::istream& operator>>(std::istream& is, Record& Record);
};


#endif /* FFCD09C6_ACDA_4273_B86D_DEDC69507119 */
