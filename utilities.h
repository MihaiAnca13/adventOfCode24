//
// Created by Mihai on 01/12/2024.
//

#ifndef ADVENTOFCODE24_UTILITIES_H
#define ADVENTOFCODE24_UTILITIES_H

#include <filesystem>
#include <fstream>
#include <vector>
#include <algorithm>


struct Vector2 {
    int x;
    int y;

    bool operator==(const Vector2 &rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    struct Vector2 operator-(const Vector2 &rhs) const {
        return {x - rhs.x, y - rhs.y};
    }

    struct Vector2 operator+(const Vector2 &rhs) const {
        return {x + rhs.x, y + rhs.y};
    }

    [[nodiscard]] struct Vector2 abs() const {
        return {std::abs(x), std::abs(y)};
    }

    struct Vector2 operator*(const int &rhs) const {
        return {x * rhs, y * rhs};
    }
};


namespace utilities {
    inline std::string readFromFile(const std::filesystem::path &path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + path.string());
        }

        std::string content;
        std::string line;
        int nr_rows = 0;
        while (std::getline(file, line)) {
            content += line + '\n';
            nr_rows++;
        }

        return content;
    }

    template<typename T>
    inline std::vector<T> split(const std::string &content, std::basic_string<char> delimiter) {
        std::vector<T> result;
        std::string token;
        size_t pos = 0;
        auto content_copy = content;
        while ((pos = content_copy.find(delimiter)) != std::string::npos) {
            token = content_copy.substr(0, pos);
            result.push_back(token);
            content_copy.erase(0, pos + delimiter.length());
        }
        result.push_back(content_copy);
        return result;
    }

    inline std::vector<std::vector<float>> split(const std::string &content, std::basic_string<char> delimiter) {
        // split content by \n
        std::vector<std::vector<float>> result;
        std::vector<float> row_values;
        std::vector<std::string> rows;
        std::string row;
        std::string token;
        size_t pos = 0;
        for (char i: content) {
            if (i == '\n') {
                // split row by delimiter
                while ((pos = row.find(delimiter)) != std::string::npos) {
                    token = row.substr(0, pos);
                    row_values.push_back(std::stof(token));
                    row.erase(0, pos + delimiter.length());
                }
                row_values.push_back(std::stof(row));
                result.push_back(row_values);
                row = "";
                row_values.clear();
            } else {
                row += i;
            }
        }

        return result;
    }

    inline std::vector<std::vector<std::string>> splitText(const std::string &content) {
        // split content by \n
        std::vector<std::vector<std::string>> result;
        std::vector<std::string> row_values;
        // split row by each character
        for (char i: content) {
            if (i == '\n') {
                result.push_back(row_values);
                row_values.clear();
            } else {
                row_values.push_back(std::string(1, i));
            }
        }

        return result;
    }

    inline std::vector<std::string> splitIntoRows(const std::string &content) {
        std::vector<std::string> result;
        std::string row;
        for (char i: content) {
            if (i == '\n') {
                result.push_back(row);
                row = "";
            } else {
                row += i;
            }
        }
        return result;
    }

    inline void sortColumns(std::vector<std::vector<float>> &matrix) {
        if (matrix.empty() || matrix[0].empty()) return;

        size_t rows = matrix.size();
        size_t cols = matrix[0].size();

        // Transpose the matrix
        std::vector<std::vector<float>> transposed(cols, std::vector<float>(rows));
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                transposed[j][i] = matrix[i][j];
            }
        }

        // Sort each "column" in the transposed matrix (row of original)
        for (auto &row: transposed) {
            std::sort(row.begin(), row.end());
        }

        // Transpose back
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                matrix[i][j] = transposed[j][i];
            }
        }
    }

    template<typename T>
    inline void printRow(std::vector<T> &row) {
        for (auto &j: row) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

    template<typename T>
    inline void printMatrix(std::vector<std::vector<T>> &matrix, const std::string &header = "") {
        if (!header.empty()) {
            std::cout << header << ": " << std::endl;
        }
        for (auto &i: matrix) {
            for (auto &j: i) {
                std::cout << j << " ";
            }
            std::cout << std::endl;
        }
    }

    inline std::vector<std::string> matrixToString(std::vector<std::vector<std::string>> &matrix) {
        std::vector<std::string> result;
        for (auto &i: matrix) {
            std::string row;
            for (auto &j: i) {
                row += j;
            }
            result.push_back(row);
        }
        return result;
    }

    template<typename T>
    inline void reverseEachRow(std::vector<std::vector<T>> &matrix) {
        for (auto &row: matrix) {
            std::reverse(row.begin(), row.end());
        }
    }

    template<typename T>
    inline std::vector<std::vector<T>> upperDiagonals(std::vector<std::vector<T>> &matrix) {
        std::vector<std::vector<T>> result;
        size_t rows = matrix.size();
        size_t cols = matrix[0].size();

        for (size_t n = 0; n < cols; n++) {
            std::vector<T> diagonal;
            // Collect elements along the diagonal starting from the nth column in the first row
            for (size_t i = 0; i < rows - n; i++) {
                diagonal.push_back(matrix[i][i + n]);
            }

            // Add the diagonal to the result if it's not empty
            if (!diagonal.empty()) {
                result.push_back(diagonal);
            }
        }

        return result;
    }

    template<typename T>
    inline std::vector<std::vector<T>> emtpy_like(std::vector<std::vector<T>> &matrix) {
        std::vector<std::vector<T>> result;
        for (auto &row: matrix) {
            std::vector<T> empty_row(row.size());
            result.push_back(empty_row);
        }
        return result;
    }
}

#endif //ADVENTOFCODE24_UTILITIES_H
