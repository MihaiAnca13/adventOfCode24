#include <iostream>
#include "../utilities.h"


int main() {
    // read from input.txt
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day4\input.txt)";
    std::string content;
    content = utilities::readFromFile(path);
    auto matrix = utilities::splitText(content);

    std::vector<std::vector<std::vector<std::string>>> lookup = {
            {
                    {"M", ".", "S"},
                    {".", "A", "."},
                    {"M", ".", "S"}
            },
            {
                    {"M", ".", "M"},
                    {".", "A", "."},
                    {"S", ".", "S"}
            },
            {
                    {"S", ".", "S"},
                    {".", "A", "."},
                    {"M", ".", "M"}
            },
            {
                    {"S", ".", "M"},
                    {".", "A", "."},
                    {"S", ".", "M"}
            }
    };

//    std::cout << "Original matrix: " << std::endl;
//    for (auto &i : matrix) {
//        for (auto &j : i) {
//            std::cout << j << " ";
//        }
//        std::cout << std::endl;
//    }

    auto total_xmas = 0;

    // find A and then look in all directions to check if it matches any of the lookups
    // dots are ignored
    for (int i = 1; i < matrix.size() - 1; ++i) {
        for (int j = 1; j < matrix[i].size() - 1; ++j) {
            if (matrix[i][j][0] == 'A') {
                for (auto &l : lookup) {
                    bool match = true;
                    for (int k = 0; k < 3; ++k) {
                        for (int m = 0; m < 3; ++m) {
                            if (l[k][m][0] == '.') {
                                continue;
                            }
                            if (l[k][m] != matrix[i + k - 1][j + m - 1]) {
                                match = false;
                                break;
                            }
                        }
                        if (!match) {
                            break;
                        }
                    }
                    if (match) {
                        total_xmas++;
                    }
                }
            }
        }
    }

    std::cout << "Total XMAS: " << total_xmas << std::endl;


    return 0;
}
