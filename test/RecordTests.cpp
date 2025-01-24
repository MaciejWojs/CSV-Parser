#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include "../Record.hpp"
#include "../Record.cpp"

TEST(RecordTest, RecordInputStream) {
    std::string input = "\"01.10.2023 12:00:00\",1.1,2.2,3.3,4.4,5.5";
    std::istringstream iss(input);
    Record record;

    iss >> record;

    EXPECT_EQ(record.getTime(), convertToTimeT("01.10.2023 12:00:00"));
    EXPECT_DOUBLE_EQ(record.getAutoConsumption(), 1.1);
    EXPECT_DOUBLE_EQ(record.getExport(), 2.2);
    EXPECT_DOUBLE_EQ(record.getImport(), 3.3);
    EXPECT_DOUBLE_EQ(record.getConsumption(), 4.4);
    EXPECT_DOUBLE_EQ(record.getProduction(), 5.5);
}

TEST(RecordTest, RecordInputStreamMissingField) {
    std::string input = "\"01.10.2023 12:00\",1.1,2.2,3.3,4.4";
    std::istringstream iss(input);
    Record record;

    EXPECT_THROW(iss >> record, std::runtime_error);
}

TEST(RecordTest, RecordInputStreamInvalidDouble) {
    std::string input = "\"01.10.2023 12:00\",1.1,invalid,3.3,4.4,5.5";
    std::istringstream iss(input);
    Record record;

    EXPECT_THROW(iss >> record, std::runtime_error);
}

TEST(RecordTest, RecordOutputStream) {
    std::string expected = "Time: 01.10.2023 12:00:00 AutoConsumption: 1.1 Export: 2.2 Import: 3.3 Consumption: 4.4 Production: 5.5";
    std::ostringstream oss;

    std::string input = "\"01.10.2023 12:00\",1.1,2.2,3.3,4.4,5.5";
    std::istringstream iss(input);
    Record record;
    iss >> record;

    oss << record;

    EXPECT_EQ(oss.str(), expected);
}