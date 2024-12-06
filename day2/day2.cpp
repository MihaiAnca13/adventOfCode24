//
// Created by Mihai on 02/12/2024.
//
#include <iostream>
#include "../utilities.h"
#include <tuple>


std::tuple<bool, int> isRowSafe(const std::vector<float> &row) {
    auto current = row[0];
    auto direction = row[1] - row[0] > 0 ? 1 : -1;
    for (int i = 1; i < row.size(); i++) {
        auto diff = row[i] - current;
        auto abs_diff = std::abs(diff);

        if (diff * direction < 0 || abs_diff < 1 || abs_diff > 3) {
            return {false, i};
        }

        current = row[i];
    }

    return {true, row.size() - 1};
}


bool tryDampeningRow(const std::vector<float>& row) {
    // Try removing each element and check sequence
    for (int remove_index = 0; remove_index < row.size(); ++remove_index) {
        std::vector<float> rowCopy = row;
        rowCopy.erase(rowCopy.begin() + remove_index);

        auto [safe, _] = isRowSafe(rowCopy);
        if (safe) return true;
    }
    return false;
}


int main() {
    std::filesystem::path path = "D:\\Projects\\adventOfCode24\\day2\\input2.txt";
    std::string content;
    content = utilities::readFromFile(path);
    auto result = utilities::split(content, std::string(" "));

//    for (auto &i : result) {
//        for (auto &j : i) {
//            std::cout << j << " ";
//        }
//        std::cout << std::endl;
//    }

    auto safe_count = 0;
    for (auto &row : result) {
        auto [safe, _] = isRowSafe(row);
        if (safe) {
            safe_count++;
        }
    }

    std::cout<<safe_count<<std::endl;

    auto almost_safe_count = 0;
    for (auto &row : result) {
        auto [safe, i] = isRowSafe(row);

        if (!safe) {
            safe = tryDampeningRow(row);
        }

        if (safe) {
            almost_safe_count++;
        }
    }

    std::cout<<almost_safe_count<<std::endl;
}

