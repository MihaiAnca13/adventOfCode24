#include <iostream>
#include "../utilities.h"
#include <map>
#include <ranges>


bool out_of_bounds(const int i, const int j, const int rows, const int cols) {
    return i < 0 || j < 0 || i > rows || j > cols;
}


std::tuple<int, int, bool> get_start_pos(const std::vector<std::vector<std::string>> &result) {
    int pos_i = 0, pos_j = 0;
    bool found = false;
    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < result[i].size(); j++) {
            if (result[i][j] == "^") {
                pos_i = i;
                pos_j = j;
                found = true;
                break;
            }
            if (found) {
                break;
            }
        }
    }
    return std::tuple(pos_i, pos_j, found);
}

int navigate(std::vector<std::vector<std::string>> &result, int pos_i, int pos_j) {
    int dir = 0; // 0 - up, 1 - right, 2 - down, 3 - left
    int nr_visited = 1;
    result[pos_i][pos_j] = "V";
    std::map<std::tuple<int, int>, std::vector<int>> visited;
    visited[std::tuple(pos_i, pos_j)].push_back(dir);
    while (!out_of_bounds(pos_i, pos_j, result.size() - 1, result[pos_i].size() - 1)) {
        const auto current = std::tuple(pos_i, pos_j);
        // move in dir
        switch (dir) {
            case 0:
                pos_i--;
            break;
            case 1:
                pos_j++;
            break;
            case 2:
                pos_i++;
            break;
            case 3:
                pos_j--;
            break;
            case 4:
                dir = 0;
            continue;
        }
        if (out_of_bounds(pos_i, pos_j, result.size() - 1, result[pos_i].size() - 1)) {
            break;
        }
        if (result[pos_i][pos_j] == "#") {
            dir++;
            std::tie(pos_i, pos_j) = current;
        } else {
            if (result[pos_i][pos_j] != "V") {
                nr_visited += 1;
                visited[std::tuple(pos_i, pos_j)].push_back(dir);
            } else if (std::ranges::find(visited[std::tuple(pos_i, pos_j)], dir) != visited[std::tuple(pos_i, pos_j)].end()) {
                return -1; // stuck in a loop
            }
            result[pos_i][pos_j] = "V";
        }
    }
    return nr_visited;
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(/home/mihai/mihai_projs/adventOfCode24/day6/input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::splitText(content);
    auto original = result;

    // find where ^ is
    int pos_i = 0, pos_j = 0;
    bool found = false;
    std::tie(pos_i, pos_j, found) = get_start_pos(result);
    int original_pos_i = pos_i;
    int original_pos_j = pos_j;

    if (!found) {
        return -1;
    }

    std::cout << "Found at: " << pos_i << " " << pos_j << std::endl;

    const int nr_visited = navigate(result, pos_i, pos_j);

    std::cout << "Visited: " << nr_visited << std::endl;

    int nr_loops_found = 0;
    for (int i = 0; i < original.size(); i++) {
        for (int j = 0; j < original[i].size(); j++) {
            if (result[i][j] == "V" and (i != original_pos_i or j != original_pos_j)) {
                auto copy_of_original = original;
                copy_of_original[i][j] = "#";
                auto loop = navigate(copy_of_original, original_pos_i, original_pos_j);
                if (loop == -1) {
                    nr_loops_found++;
                }
            }
        }
    }

    std::cout << "Loops found: " << nr_loops_found << std::endl;

    return 0;
}
