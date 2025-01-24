#include <gtest/gtest.h>
#include <string>
#include "../utils.cpp"

TEST(UtilsTest, TrimAllQuotationMarks) {
    std::string str = "\"Jesli tylko \"chcemy\", mozemy osiagnac \"wszystko\", nawet jesli 'droga' jest pelna trudnosci.\"";
    std::string result = trimAllQuotationMarks(str);
    EXPECT_EQ(result, "Jesli tylko chcemy, mozemy osiagnac wszystko, nawet jesli 'droga' jest pelna trudnosci.");
    EXPECT_TRUE(result.size() < str.size());
}

TEST(UtilsTest, convertToTimeT) {
    std::string timeStr = "05.10.2023 12:34:56";
    time_t result = convertToTimeT(timeStr);

    long long expected_epoch = 1696502096;
    EXPECT_EQ(result, expected_epoch);
}
