#include <gtest.h>
#include <iostream>
#include <sstream>
#include <cctype>
#include "refracted.cc"

struct Shares {
    double totalValue;
    double peopleShare;
    double managerShare;
};

Shares calculateShares(double totalValue) {
    Shares shares;
    shares.totalValue = totalValue;
    shares.peopleShare = totalValue * 0.8 / 25;
    shares.managerShare = totalValue * 0.2;
    return shares;
}

std::string getCurrentDate() {
    time_t currentTime = time(nullptr);
    struct tm* localTime = localtime(&currentTime);
    char dateStr[80];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", localTime);
    return dateStr;
}

bool isInteger(const std::string& input) {
    for (char c : input) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

int getUserInput(const std::string& input) {
    std::istringstream iss(input);
    int userInput;
    iss >> userInput;
    return userInput;
}

TEST(CalculatorTest, PeopleShareCalculation) {
    double totalValue = 1000.0;
    Shares shares = calculateShares(totalValue);
    EXPECT_DOUBLE_EQ(shares.peopleShare, totalValue * 0.8 / 25);
}

TEST(CalculatorTest, ManagerShareCalculation) {
    double totalValue = 1000.0;
    Shares shares = calculateShares(totalValue);
    EXPECT_DOUBLE_EQ(shares.managerShare, totalValue * 0.2);
}

TEST(DateTest, GetCurrentDate) {
    std::string currentDate = getCurrentDate();
    // You might need to adjust the expected format based on your system's locale
    EXPECT_EQ(currentDate, "yyyy-mm-dd");
}

TEST(InputTest, InputIsInteger) {
    std::string integerInput = "123";
    int integerUserInput = getUserInput(integerInput);

    EXPECT_TRUE(isInteger(integerInput));
    EXPECT_TRUE(inputIsInteger(integerUserInput));  // Replace with your input validation function

    std::string nonIntegerInput = "hello";
    int nonIntegerUserInput = getUserInput(nonIntegerInput);

    EXPECT_FALSE(isInteger(nonIntegerInput));
    EXPECT_FALSE(inputIsInteger(nonIntegerUserInput));  // Replace with your input validation function
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    }