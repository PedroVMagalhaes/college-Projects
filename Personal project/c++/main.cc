#include <iostream>
#include <fstream>
#include <ctime>

int main() {
    double totalValue;
    std::cout << "Enter the total value: ";
    std::cin >> totalValue;

    time_t currentTime = time(nullptr);
    struct tm* localTime = localtime(&currentTime);
    char dateStr[80];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", localTime);

    std::string fileName = "Date_" + std::string(dateStr) + " -" + std::to_string(int(totalValue)) + "K de pot" + ".txt";

    std::ofstream outputFile(fileName);
    if (outputFile.is_open()) {
        outputFile << "Total Value: " << totalValue << "\n\n";

        double peopleShare = totalValue * 0.8 / 25;
        double managerShare = totalValue * 0.2;

        outputFile << "People Share:\n";
        for (int i = 0; i < 25; ++i) {
            outputFile << "Person " << i + 1 << ": " << peopleShare << "\n";
        }

        outputFile << "\nManager Share: " << managerShare << "\n";

        outputFile.close();
        std::cout << "Text file '" << fileName << "' created successfully." << std::endl;
    } else {
        std::cout << "Error creating text file!" << std::endl;
    }

    return 0;
}