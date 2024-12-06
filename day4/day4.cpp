#include <iostream>
#include "../utilities.h"


bool check_right(const std::vector<std::vector<std::string>> &matrix, int i, int j) {
    // check j can still go right
    if (j + 3 >= matrix[i].size()) {
        return false;
    }

    // check if there is a MAS
    if (matrix[i][j + 1][0] == 'M' && matrix[i][j + 2][0] == 'A' && matrix[i][j + 3][0] == 'S') {
        return true;
    }
    return false;
}

bool check_left(const std::vector<std::vector<std::string>> &matrix, int i, int j) {
    // check j can still go left
    if (j - 3 < 0) {
        return false;
    }

    // check if there is a MAS
    if (matrix[i][j - 1][0] == 'M' && matrix[i][j - 2][0] == 'A' && matrix[i][j - 3][0] == 'S') {
        return true;
    }
    return false;
}

bool check_up(const std::vector<std::vector<std::string>> &matrix, int i, int j) {
    // check i can still go up
    if (i - 3 < 0) {
        return false;
    }

    // check if there is a MAS
    if (matrix[i - 1][j][0] == 'M' && matrix[i - 2][j][0] == 'A' && matrix[i - 3][j][0] == 'S') {
        return true;
    }
    return false;
}

bool check_down(const std::vector<std::vector<std::string>> &matrix, int i, int j) {
    // check i can still go down
    if (i + 3 >= matrix.size()) {
        return false;
    }

    // check if there is a MAS
    if (matrix[i + 1][j][0] == 'M' && matrix[i + 2][j][0] == 'A' && matrix[i + 3][j][0] == 'S') {
        return true;
    }
    return false;
}

bool check_diag_right_down(const std::vector<std::vector<std::string>> &matrix, int i, int j) {
    // check i and j can still go right down
    if (i + 3 >= matrix.size() || j + 3 >= matrix[i].size()) {
        return false;
    }

    // check if there is a MAS
    if (matrix[i + 1][j + 1][0] == 'M' && matrix[i + 2][j + 2][0] == 'A' && matrix[i + 3][j + 3][0] == 'S') {
        return true;
    }
    return false;
}

bool check_diag_right_up(const std::vector<std::vector<std::string>> &matrix, int i, int j) {
    // check i and j can still go right up
    if (i - 3 < 0 || j + 3 >= matrix[i].size()) {
        return false;
    }

    // check if there is a MAS
    if (matrix[i - 1][j + 1][0] == 'M' && matrix[i - 2][j + 2][0] == 'A' && matrix[i - 3][j + 3][0] == 'S') {
        return true;
    }
    return false;
}

bool check_diag_left_down(const std::vector<std::vector<std::string>> &matrix, int i, int j) {
    // check i and j can still go left down
    if (i + 3 >= matrix.size() || j - 3 < 0) {
        return false;
    }

    // check if there is a MAS
    if (matrix[i + 1][j - 1][0] == 'M' && matrix[i + 2][j - 2][0] == 'A' && matrix[i + 3][j - 3][0] == 'S') {
        return true;
    }
    return false;
}

bool check_diag_left_up(const std::vector<std::vector<std::string>> &matrix, int i, int j) {
    // check i and j can still go left up
    if (i - 3 < 0 || j - 3 < 0) {
        return false;
    }

    // check if there is a MAS
    if (matrix[i - 1][j - 1][0] == 'M' && matrix[i - 2][j - 2][0] == 'A' && matrix[i - 3][j - 3][0] == 'S') {
        return true;
    }
    return false;
}




int main() {
    // read from input.txt
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day4\input.txt)";
    std::string content;
    content = utilities::readFromFile(path);
    auto matrix = utilities::splitText(content);

//    std::cout << "Original matrix: " << std::endl;
//    for (auto &i : matrix) {
//        for (auto &j : i) {
//            std::cout << j << " ";
//        }
//        std::cout << std::endl;
//    }

    auto total_xmas = 0;

    // find X and then look in all directions for MAS
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j][0] == 'X') {
                total_xmas += check_up(matrix, i, j);
                total_xmas += check_down(matrix, i, j);
                total_xmas += check_left(matrix, i, j);
                total_xmas += check_right(matrix, i, j);
                total_xmas += check_diag_left_down(matrix, i, j);
                total_xmas += check_diag_left_up(matrix, i, j);
                total_xmas += check_diag_right_down(matrix, i, j);
                total_xmas += check_diag_right_up(matrix, i, j);
            }
        }
    }

    std::cout << "Total XMAS: " << total_xmas << std::endl;


    return 0;
}
