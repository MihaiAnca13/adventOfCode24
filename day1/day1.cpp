#include <iostream>
#include "../utilities.h"


int main() {
    // read from input.txt
    std::filesystem::path path = "D:\\Projects\\adventOfCode24\\day1\\input2.txt";
    std::string content;
    content = utilities::readFromFile(path);
    auto result = utilities::split(content, std::string("   "));

    utilities::sortColumns(result);

//    for (auto &i : result) {
//        for (auto &j : i) {
//            std::cout << j << " ";
//        }
//        std::cout << std::endl;
//    }

    // dissimilarity score
    int dif = 0;
    for (auto &row: result) {
        dif += (int) std::abs(row.back() - row.front());
    }
    std::cout << dif << std::endl;


    // similarity score
    int i = 0, j = 0;
    int count = 0;
    int sum = 0;
    float current_value;
    while (i < result.size() && j < result.size()) {
        if (result[i][0] != result[j][1]) {
            if (count != 0) {
                sum += count * (int) result[i][0];

                current_value = result[i][0];
                // if row[i+1] is the same as row[i], we counted already
                while (i + 1 < result.size() && current_value == result[i + 1][0]) {
                    sum += count * (int) result[i][0];
                    i++;
                }

                count = 0;

                continue;
            }

            if (result[i][0] < result[j][1]) {
                i++;
            } else {
                j++;
            }
        } else {
            count++;
            j++;
        }
    }

    std::cout << sum << std::endl;

    return 0;
}
