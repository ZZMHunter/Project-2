/*
    Author: Zhao Zhang
    Course: CSCI-135
    Instructor: Tong Yi
    Assignment: Project 2

    Find the maximum monthly total across all categories; draw a chart for a selected category
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <iomanip>

int month(std::string input) {
    int monthSlash = input.find('/');
    return std::stoi(input.substr(0, monthSlash));
}

int main() {
    std::string file, junk, description, category, date, amountStr;
    std::string categories[20];
    std::string monthArr[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    double arr[12][20] = {0};
    double amountDouble = 0, maxMonthly = 0;
    int currMonth = 0, categoryCount = 0, chosen;
    std::cout << "Enter file name: ";
    std::cin >> file;
    std::ifstream data(file);
    if (data.fail()) {
        std::cerr << "Error: cannot open file" << std::endl;
        exit(1);
    }
    getline(data, junk);
    while (getline(data, date, ',')) {
        currMonth = month(date) - 1;
        getline(data, description, ',');
        getline(data, category, ',');
        getline(data, amountStr);
        amountDouble = stod(amountStr);
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
    maxMonthly = 0;
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < categoryCount; j++) {
            if (arr[i][j] > maxMonthly) {
                maxMonthly = arr[i][j];
            }
        }
    }
    std::cout << "\nSelect one of the following categories:\n";
    for (int i = 0; i < categoryCount; i++) {
        std::cout << i << ". " << categories[i] << std::endl;
    }
    std::cout << "Choose a number between [0, " << categoryCount - 1 << "]: ";
    std::cin >> chosen;
    if (chosen < 0 || chosen >= categoryCount) {
        std::cerr << "Invalid choice." << std::endl;
        data.close();
        return 1;
    }
    std::cout << "\n" << categories[chosen] << ":\n";
    for (int i = 0; i < 12; i++) {
        std::cout << monthArr[i] << "   " << " " << arr[i][chosen] << "   ";
        int barLength = (maxMonthly > 0) ? (int)(arr[i][chosen] / maxMonthly * 40) : 0;
        for (int j = 0; j < barLength; j++) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }
    data.close();
    return 0;
}
