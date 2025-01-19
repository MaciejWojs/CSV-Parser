#include <gtest/gtest.h>
#include <string>
#include "../utils.cpp"

TEST(UtilsTest, TrimAllQuotationMarks) {
    std::string str = "\"Jesli tylko \"chcemy\", mozemy osiagnac \"wszystko\", nawet jesli 'droga' jest pelna trudnosci.\"";
    std::string result = trimAllQuotationMarks(str);
    EXPECT_EQ(result, "Jesli tylko chcemy, mozemy osiagnac wszystko, nawet jesli 'droga' jest pelna trudnosci.");
    EXPECT_TRUE(result.size() < str.size());
}