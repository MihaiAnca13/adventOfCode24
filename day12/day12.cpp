#include <iostream>
#include "../utilities.h"
#include <unordered_map>
#include <unordered_set>

struct Plot {
    int perimeter, area;
    char letter;
    int sides;
};

std::vector<Vector2> get_neighbours(const std::vector<std::vector<std::string> > &matrix, const int i, const int j) {
    auto rows = matrix.size();
    auto cols = matrix[0].size();
    std::vector<Vector2> neighbours{};
    for (Vector2 dir: {Vector2(0, 1), Vector2(0, -1), Vector2(1, 0), Vector2(-1, 0)}) {
        Vector2 new_pos = Vector2(i, j) + dir;
        if (new_pos.x >= 0 && new_pos.x < rows && new_pos.y >= 0 && new_pos.y < cols) {
            if (matrix[i][j][0] == matrix[new_pos.x][new_pos.y][0]) {
                neighbours.push_back(new_pos);
            }
        }
    }
    return neighbours;
}


int compute_perimeter(const std::vector<std::vector<std::string> > &matrix, const int i, const int j) {
    int perimeter = 0;
    auto rows = matrix.size();
    auto cols = matrix[0].size();


    for (Vector2 dir: {Vector2(0, 1), Vector2(0, -1), Vector2(1, 0), Vector2(-1, 0)}) {
        Vector2 new_pos = Vector2(i, j) + dir;
        if (new_pos.x >= 0 && new_pos.x < rows && new_pos.y >= 0 && new_pos.y < cols) {
            if (matrix[i][j][0] != matrix[new_pos.x][new_pos.y][0]) {
                perimeter++;
            }
        } else {
            perimeter++;
        }
    }
    return perimeter;
}

int get_cell_value(const std::vector<std::vector<std::string> > &matrix, int i, int j) {
    if (i < 0 || i >= matrix.size() || j < 0 || j >= matrix[0].size()) {
        return ' ';
    }
    return matrix[i][j][0];
}

int compute_sides(const std::vector<std::vector<std::string> > &matrix, const int i, const int j) {
    int left = 0, right = 0, top = 0, bottom = 0;

    char current = matrix[i][j][0];
    char left_cell = get_cell_value(matrix, i, j - 1);
    char right_cell = get_cell_value(matrix, i, j + 1);
    char top_cell = get_cell_value(matrix, i - 1, j);
    char bottom_cell = get_cell_value(matrix, i + 1, j);

    // check top side
    if (left_cell != current && top_cell != current) {
        top = 1;
    } else if (left_cell == current && top_cell != current && get_cell_value(matrix, i - 1, j - 1) == current) {
        top = 1;
    }

    // check right side
    if (top_cell != current && right_cell != current) {
        right = 1;
    } else if (top_cell == current && right_cell != current && get_cell_value(matrix, i - 1, j + 1) == current) {
        right = 1;
    }

    // check bottom side
    if (right_cell != current && bottom_cell != current) {
        bottom = 1;
    } else if (right_cell == current && bottom_cell != current && get_cell_value(matrix, i + 1, j + 1) == current) {
        bottom = 1;
    }

    // check left side
    if (bottom_cell != current && left_cell != current) {
        left = 1;
    } else if (bottom_cell == current && left_cell != current && get_cell_value(matrix, i + 1, j - 1) == current) {
        left = 1;
    }

    return left + right + top + bottom;
}


Plot explore(std::unordered_set<Vector2> &visited, const std::vector<std::vector<std::string> > &matrix, const int i,
             const int j) {
    if (visited.contains({i, j})) {
        return {0, 0, ' '};
    }

    visited.insert({i, j});

    // filter neighbours based on current value matrix[i][j] matching the neighbors
    //  start timer
    auto neighbours = get_neighbours(matrix, i, j);
    Plot result{compute_perimeter(matrix, i, j), 1, matrix[i][j][0], compute_sides(matrix, i, j)};

    for (auto &[x, y]: neighbours) {
        const auto n_result = explore(visited, matrix, x, y);
        result.perimeter += n_result.perimeter;
        result.area += n_result.area;
        result.sides += n_result.sides;
    }

    return result;
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(/home/mihai/mihai_projs/adventOfCode24/day12/input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::splitText(content);

    // utilities::printMatrix(result, "input");

    std::unordered_set<Vector2> visited;

    uint64_t price = 0, price_sides = 0;
    std::vector<Plot> plots;
    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < result[i].size(); j++) {
            auto res = explore(visited, result, i, j);
            if (res.area > 0) {
                // std::cout << "Plot: " << res.area << " " << res.perimeter << " " << res.letter << " " << res.sides <<
                        // std::endl;
                plots.push_back(res);
                price += res.area * res.perimeter;
                price_sides += res.area * res.sides;
            }
        }
    }

    std::cout << price << std::endl;
    std::cout << price_sides;

    return 0;
}
