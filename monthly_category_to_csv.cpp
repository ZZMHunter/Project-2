/* 
    Author: 
    Zhao Zhang Course: CSCI-135 
    Instructor: Tong Yi 
    Assignment: Project 2 
    Find the maximum monthly total across all categories;
    Draw a chart for a selected category;
    Write into a csv file.
*/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>

int month(std::string input) {
    int monthSlash = input.find('/');
    return std::stoi(input.substr(0, monthSlash));
}

void readData(std::string file, std::string categories[], double arr[12][20], int &categoryCount) {
    std::ifstream data(file);
    if (data.fail()) {
        std::cerr << "Error: cannot open file" << std::endl;
        exit(1);
    }
    std::string junk, description, category, date, amountStr;
    getline(data, junk);
    while (getline(data, date, ',')) {
        int currMonth = month(date) - 1;
        getline(data, description, ',');
        getline(data, category, ',');
        getline(data, amountStr);
        double amountDouble = stod(amountStr);
        bool found = false;
        for (int i = 0; i < categoryCount; i++) {
            if (categories[i] == category) {
                arr[currMonth][i] += amountDouble;
                found = true;
                break;
            }
        }
        if (!found) {
            int insertPos = categoryCount;
            for (int i = 0; i < categoryCount; i++) {
                if (category < categories[i]) {
                    insertPos = i;
                    break;
                }
            }
            for (int col = categoryCount; col > insertPos; col--) {
                categories[col] = categories[col - 1];
                for (int m = 0; m < 12; m++) {
                    arr[m][col] = arr[m][col - 1];
                }
            }
            categories[insertPos] = category;
            for (int m = 0; m < 12; m++) {
                arr[m][insertPos] = 0;
            }
            arr[currMonth][insertPos] = amountDouble;
            categoryCount++;
        }
    }
    data.close();
}

void printReport(double arr[12][20], std::string categories[], std::string monthArr[], int categoryCount) {
    double categoryTotals[20] = {0};
    double monthTotals[12] = {0};
    double overallTotal = 0;

    for (int j = 0; j < categoryCount; j++) {
        for (int i = 0; i < 12; i++) {
            categoryTotals[j] += arr[i][j];
            monthTotals[i] += arr[i][j];
            overallTotal += arr[i][j];
        }
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nMON";
    for (int j = 0; j < categoryCount; j++) {
        std::cout << std::setw(15) << categories[j];
    }
    std::cout << std::setw(15) << "TOTAL" << "\n";

    for (int i = 0; i < 12; i++) {
        std::cout << std::left << std::setw(5) << monthArr[i];
        for (int j = 0; j < categoryCount; j++) {
            std::cout << std::right << std::setw(15) << arr[i][j];
        }
        std::cout << std::setw(15) << monthTotals[i] << "\n";
    }

    std::cout << std::left << std::setw(5) << "";
    for (int j = 0; j < categoryCount; j++) {
        std::cout << std::right << std::setw(15) << categoryTotals[j];
    }
    std::cout << std::setw(15) << overallTotal << "\n";

    std::cout << std::left << std::setw(5) << "";
    for (int j = 0; j < categoryCount; j++) {
        double percent = (overallTotal > 0) ? (categoryTotals[j] / overallTotal * 100) : 0;
        std::cout << std::right << std::setw(15) << percent << "%";
    }
    std::cout << "\n";
}

void createOutput(double arr[12][20], std::string categories[], std::string monthArr[], int categoryCount){
    double categoryTotals[20] = {0};
    double monthTotals[12] = {0};
    double overallTotal = 0;

    std::cout << "Enter output file name: ";
    std::string file;
    std::cin >> file;

    std::ofstream fout(file);
    if(fout.fail()){
        std::cerr << "Failed to open file" << std::endl;
        exit(0);    
    }

    for (int j = 0; j < categoryCount; j++) {
        for (int i = 0; i < 12; i++) {
            categoryTotals[j] += arr[i][j];
            monthTotals[i] += arr[i][j];
            overallTotal += arr[i][j];
        }
    }

    std::cout << std::fixed << std::setprecision(2);
    fout << "MON,";
    for (int j = 0; j < categoryCount; j++) {
        fout << categories[j] << ",";
    }
    fout << "TOTAL" << "\n";

    for (int i = 0; i < 12; i++) {
        fout << monthArr[i]<< ",";
        for (int j = 0; j < categoryCount; j++) {
            fout << arr[i][j] << ",";
        }
        fout << monthTotals[i] << "\n";
    }

    fout << ",";
    for (int j = 0; j < categoryCount; j++) {
        fout << categoryTotals[j]<<",";
    }
    fout << overallTotal << "\n";

    fout << ",";
    for (int j = 0; j < categoryCount; j++) {
        double percent = (overallTotal > 0) ? (categoryTotals[j] / overallTotal * 100) : 0;
        fout << std::fixed << std::setprecision(2) << percent << "%,";
    }
    fout << ",";

}

int main() {
    std::string file;
    std::string categories[20];
    std::string monthArr[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    double arr[12][20] = {0};
    int categoryCount = 0;

    std::cout << "Enter a csv file: ";
    std::cin >> file;

    readData(file, categories, arr, categoryCount);
    printReport(arr, categories, monthArr, categoryCount);
    createOutput(arr, categories, monthArr, categoryCount);

    return 0;
}
