#include <iostream>
#include <ctime>
#include <string>

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

int main() {
    double totalValue;
    std::cout << "Enter the total value: ";
    std::cin >> totalValue;

    Shares shares = calculateShares(totalValue);
    std::string currentDate = getCurrentDate();

    std::string fileName = "Date_" + currentDate + ".xls";

    // Code for interacting with the library and saving to a file
    // ... (not shown here)

    std::cout << "Excel file '" << fileName << "' created successfully." << std::endl;

    return 0;
}
