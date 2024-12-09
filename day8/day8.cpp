#include <iostream>
#include "../utilities.h"
#include <map>


bool within_bounds(const Vector2 &v, const int rows, const int cols) {
    return v.x >= 0 && v.x < rows && v.y >= 0 && v.y < cols;
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day8\input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::splitText(content);
    std::map<std::string, std::vector<Vector2>> antennas;

    int rows = result.size();
    int cols = result[0].size();
    int max_multiplier = std::max(rows, cols);

    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < result[i].size(); j++) {
            if (result[i][j] != ".") {
                Vector2 v = {i, j};
                antennas[result[i][j]].push_back(v);
            }
        }
    }

    auto antinodes = utilities::emtpy_like(result);
    for (auto &freq: antennas) {
        for (int i = 0; i < freq.second.size(); i++) {
            for (int j = i + 1; j < freq.second.size(); j++) {
                auto diff = freq.second[i] - freq.second[j];
                for (int multiplier = -max_multiplier; multiplier <= max_multiplier; multiplier++) {
                    auto corner1 = freq.second[i] + diff * multiplier;
                    auto corner2 = freq.second[j] - diff * multiplier;
                    if (within_bounds(corner1, rows, cols)) {
                        antinodes[corner1.x][corner1.y] = "#";
                    }
                    if (within_bounds(corner2, rows, cols)) {
                        antinodes[corner2.x][corner2.y] = "#";
                    }
                }
            }
        }
    }

    // count number of # in antinodes
    int count = 0;
    for (auto &row: antinodes) {
        for (auto &col: row) {
            if (col == "#") {
                count++;
            }
        }
    }

    std::cout << count << std::endl;

    return 0;
}
