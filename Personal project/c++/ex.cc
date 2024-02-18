#include <iostream>
#include <libxl.h>
#include <ctime>

using namespace libxl;

int main() {
    double totalValue;
    std::cout << "Enter the total value: ";
    std::cin >> totalValue;

    time_t currentTime = time(nullptr);
    struct tm* localTime = localtime(&currentTime);
    char dateStr[80];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", localTime);

    std::string fileName = "Date_" + std::string(dateStr) + ".xls";

    Book* book = xlCreateBook();
    if (book) {
        Sheet* sheet = book->addSheet("Sheet1");

        sheet->writeStr(1, 1, "Total Value:");
        sheet->writeNum(1, 2, totalValue);

        double peopleShare = totalValue * 0.8 / 25;
        double managerShare = totalValue * 0.2;

        sheet->writeStr(3, 1, "People Share:");
        for (int i = 0; i < 25; ++i) {
            sheet->writeNum(i + 4, 1, peopleShare);
        }

        sheet->writeStr(30, 1, "Manager Share:");
        sheet->writeNum(31, 1, managerShare);

        book->save(fileName.c_str());
        book->release();
        std::cout << "Excel file '" << fileName << "' created successfully." << std::endl;
    } else {
        std::cout << "Error creating Excel workbook!" << std::endl;
    }

    return 0;
}
