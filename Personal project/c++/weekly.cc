#include <xlsxwriter.h>
#include <xls.h> // For reading Excel files (you might need to install this library too)

int main() {
    // Read the existing value from A2 using libxls
    xlsWorkBook* pWB = xls_open("weekly.xlsx", "UTF-8");
    xlsWorkSheet* pWS = xls_getWorkSheet(pWB, 0);
    double existingValue = xls_readNum(pWS, 1, 0); // Assuming A2 is row 1, column 0
    xls_close_WS(pWS);
    xls_close_WB(pWB);

    // Increment the existing value
    double newValue = existingValue + 1;

    // Write the modified value back to A2 using libxlsxwriter
    lxw_workbook* workbook = workbook_open("weekly.xlsx");
    lxw_worksheet* worksheet = workbook_get_worksheet(workbook, 0);
    worksheet_write_number(worksheet, 1, 0, newValue, NULL);

    // Close the workbook to save changes
    workbook_close(workbook);

    return 0;
}