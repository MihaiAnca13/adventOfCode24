//
// Created by Mihai on 01/12/2024.
//

#ifndef ADVENTOFCODE24_UTILITIES_H
#define ADVENTOFCODE24_UTILITIES_H

#include <filesystem>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <climits>
#include <cmath>
#include <array>


typedef std::vector<std::vector<std::string> > stringMatrix;
typedef std::vector<std::vector<uint64_t> > intMatrix;

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

    std::size_t operator ()(const Vector2 &t) const {
        const auto h1 = std::hash<int>{}(t.x);
        const auto h2 = std::hash<int>{}(t.y);

        // Same combining technique as for pair
        return h1 ^ h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
    }

    bool operator<(const Vector2 &rhs) const {
        if (x == rhs.x) {
            return y < rhs.y;
        }
        return x < rhs.x;
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector2 &v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }
};

struct Node {
    Vector2 position{};
    int cost{}; // Total cost: f = g + h

    // Comparison operator for the min-heap
    bool operator>(const Node &other) const {
        return cost > other.cost; // Min-heap prioritizes lower cost
    }

    bool operator==(const Node &other) const {
        return position == other.position;
    }
};

const std::vector<Vector2> four_directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

template<>
struct std::hash<Vector2> {
    size_t operator()(const Vector2 &v) const {
        const auto h1 = hash<int>{}(v.x);
        const auto h2 = hash<int>{}(v.y);

        // Combine hash values using bit manipulation
        return h1 ^ h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
    }
};

template<>
struct std::hash<std::pair<Vector2, Vector2> > {
    size_t operator()(const std::pair<Vector2, Vector2> &p) const {
        const auto h1 = std::hash<Vector2>{}(p.first);
        const auto h2 = std::hash<Vector2>{}(p.second);

        // Combine hash values using bit manipulation
        return h1 ^ h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
    }
};

template<>
struct std::hash<std::tuple<Vector2, Vector2, Vector2> > {
    size_t operator()(const std::tuple<Vector2, Vector2, Vector2> &t) const {
        const auto h1 = std::hash<Vector2>{}(std::get<0>(t));
        const auto h2 = std::hash<Vector2>{}(std::get<1>(t));
        const auto h3 = std::hash<Vector2>{}(std::get<2>(t));

        // Combine hash values using bit manipulation
        return h1 ^ h2 ^ h3 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
    }
};

struct PairHash {
    std::size_t operator()(const std::pair<uint64_t, uint64_t> &p) const {
        return std::hash<uint64_t>()(p.first) ^ std::hash<uint64_t>()(p.second);
    }
};

namespace utilities {
    template<typename T>
    T get_cell_value(std::vector<std::vector<T> > &matrix, int i, int j) {
        if (i < 0 || j < 0 || i >= matrix.size() || j >= matrix[0].size()) {
            // if string return '', otherwise return NULL
            if (std::is_same_v<T, std::string>) {
                return "";
            } else {
                return NULL;
            }
        }
        return matrix[i][j];
    }

    int get_cell_value(std::vector<std::vector<int> > &matrix, int i, int j) {
        if (i < 0 || j < 0 || i >= matrix.size() || j >= matrix[0].size()) {
            return -1;
        }
        return matrix[i][j];
    }

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
    std::vector<T> split(const std::string &content, std::basic_string<char> delimiter) {
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

    inline std::vector<std::vector<float> > split(const std::string &content, std::basic_string<char> delimiter) {
        // split content by \n
        std::vector<std::vector<float> > result;
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

    inline std::vector<std::vector<std::string> > splitText(const std::string &content) {
        // split content by \n
        std::vector<std::vector<std::string> > result;
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
        // add the last row
        if (!row_values.empty()) {
            result.push_back(row_values);
        }

        return result;
    }

    inline std::vector<uint64_t> str_row_to_int(const std::string &row, const std::string &delimiter) {
        std::vector<uint64_t> result;
        std::string token;
        size_t pos = 0;
        auto row_copy = row;
        while ((pos = row_copy.find(delimiter)) != std::string::npos) {
            token = row_copy.substr(0, pos);
            result.push_back(std::stoull(token));
            row_copy.erase(0, pos + delimiter.length());
        }
        result.push_back(std::stoull(row_copy));
        return result;
    }

    template<typename T>
    bool array_equal(const std::vector<T> &a, const std::vector<T> &b) {
        if (a.size() != b.size()) {
            return false;
        }

        for (int i = 0; i < a.size(); i++) {
            if (a[i] != b[i]) {
                return false;
            }
        }
        return true;
    }

    template<typename T>
    inline std::vector<std::vector<T> > matrixToT(std::vector<std::vector<std::string> > &matrix) {
        std::vector<std::vector<T> > result;
        for (auto &i: matrix) {
            std::vector<T> row;
            for (auto &j: i) {
                row.push_back(static_cast<T>(std::stof(j)));
            }
            result.push_back(row);
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

    inline void sortColumns(std::vector<std::vector<float> > &matrix) {
        if (matrix.empty() || matrix[0].empty()) return;

        size_t rows = matrix.size();
        size_t cols = matrix[0].size();

        // Transpose the matrix
        std::vector<std::vector<float> > transposed(cols, std::vector<float>(rows));
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

    inline void printRow(std::vector<Vector2> &row) {
        for (auto &j: row) {
            std::cout << "(" << j.x << ", " << j.y << ") ";
        }
        std::cout << std::endl;
    }

    inline void printUnMap(std::unordered_map<std::pair<int, int>, long, PairHash> &map) {
        // get all values of x and y
        std::vector<int> x_values;
        std::vector<int> y_values;
        for (auto &i: map) {
            x_values.push_back(i.first.first);
            y_values.push_back(i.first.second);
        }
        // sort x and y values
        std::sort(x_values.begin(), x_values.end());
        std::sort(y_values.begin(), y_values.end());
        // draw a table with x and y values
        std::cout << "x/y ";
        for (auto &i: x_values) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        for (auto &i: y_values) {
            std::cout << i << " ";
            for (auto &j: x_values) {
                if (map.contains({i, j})) {
                    std::cout << map[{i, j}] << " ";
                } else {
                    std::cout << "0 ";
                }
            }
            std::cout << std::endl;
        }
    }

    template<typename T>
    inline void printMatrix(std::vector<std::vector<T> > &matrix, const std::string &header = "") {
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

    inline std::vector<std::string> matrixToString(std::vector<std::vector<std::string> > &matrix) {
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
    inline void reverseEachRow(std::vector<std::vector<T> > &matrix) {
        for (auto &row: matrix) {
            std::reverse(row.begin(), row.end());
        }
    }

    template<typename T>
    inline std::vector<std::vector<T> > upperDiagonals(std::vector<std::vector<T> > &matrix) {
        std::vector<std::vector<T> > result;
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
    inline std::vector<std::vector<T> > empty_like(std::vector<std::vector<T> > &matrix) {
        std::vector<std::vector<T> > result;
        for (auto &row: matrix) {
            std::vector<T> empty_row(row.size());
            result.push_back(empty_row);
        }
        return result;
    }

    inline int num_digits(uint64_t val) {
        int num_digits = 0;
        uint64_t temp = val;
        while (temp) {
            temp /= 10;
            num_digits++;
        }

        return num_digits;
    }

    inline bool out_of_bounds(const int i, const int j, const unsigned long rows, const unsigned long cols) {
        return i < 0 || j < 0 || i > rows - 1 || j > cols - 1;
    }

    inline uint64_t manhattanDistance(const Vector2 &a, const Vector2 &b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }

    inline Vector2 find_char(const char c, const stringMatrix &matrix) {
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix[i].size(); j++) {
                if (matrix[i][j] == std::string(1, c)) {
                    return {i, j};
                }
            }
        }
        return {-1, -1};
    }

    inline int factorial(int n) {
        if (n == 0) {
            return 1;
        }
        return n * factorial(n - 1);
    }
}

#endif //ADVENTOFCODE24_UTILITIES_H
