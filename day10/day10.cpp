#include <iostream>
#include "../utilities.h"


std::vector<Vector2> getNeighbours(int i, int j, std::vector<std::vector<int>> &matrix) {
    std::vector<Vector2> neighbours;
    for (Vector2 dir: {Vector2{0, 1}, Vector2{0, -1}, Vector2{1, 0}, Vector2{-1, 0}}) {
        int x = dir.x;
        int y = dir.y;

        if (i + x >= 0 && i + x < matrix.size() && j + y >= 0 && j + y < matrix[i].size()) {
            auto diff = matrix[i + x][j + y] - matrix[i][j];
            if (diff == 1) {
                neighbours.push_back({i + x, j + y});
            }
        }
    }

    return neighbours;
}


int findPath(std::vector<std::vector<int>> &matrix, std::vector<Vector2> &visited, std::vector<Vector2> &stack) {
    if (stack.empty()) {
        return 0;
    }

    auto current = stack.back();
//    std::cout << "Current: " << current.x << " " << current.y << ": " << matrix[current.x][current.y] << std::endl;
    stack.pop_back();
    if (matrix[current.x][current.y] == 9) {
        return 1;
    }
    auto neighbours = getNeighbours(current.x, current.y, matrix);
    int sum = 0;
    for (auto n: neighbours) {
        // check if n is not in visited
        if (std::find(visited.begin(), visited.end(), n) == visited.end()) {
            stack.push_back(n);
            visited.push_back(n);

            // Recurse and accumulate the result
            sum += findPath(matrix, visited, stack);

            // Backtrack for part 2 only -> discover all paths
            visited.pop_back();
        }
    }

    return sum;
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day10\input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result_str = utilities::splitText(content);
    auto result = utilities::matrixToT<int>(result_str);

//    utilities::printMatrix(result, "Input");

    int sum = 0;
    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < result[i].size(); j++) {
            if (result[i][j] == 0) {
                std::vector<Vector2> stack{{i, j}};
                std::vector<Vector2> visited;
                sum += findPath(result, visited, stack);
            }
        }
    }

    std::cout << "Result: " << sum << std::endl;

    return 0;
}
