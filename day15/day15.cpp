#include <iostream>
#include "../utilities.h"


int empty_or_not(const Vector2 pos, const Vector2 dir, std::vector<std::vector<std::string>> &matrix) {
    Vector2 current = pos + dir;
    auto cell = utilities::get_cell_value(matrix, current.x, current.y);

    int steps = 1;
    while (!cell.empty()) {
        if (cell == "#") {
            return -1;
        }
        if (cell == ".") {
            return steps;
        }

        current = current + dir;
        cell = utilities::get_cell_value(matrix, current.x, current.y);
        steps++;
    }

    return steps;
}

Vector2 move_robot(Vector2 robot, std::vector<std::vector<std::string>> &matrix, char direction) {
    Vector2 dir_vector{};

    switch (direction) {
        case '^':
            dir_vector = {-1, 0};
            break;
        case 'v':
            dir_vector = {1, 0};
            break;
        case '>':
            dir_vector = {0, 1};
            break;
        case '<':
            dir_vector = {0, -1};
            break;
        default:
            break;
    }

    auto steps = empty_or_not(robot, dir_vector, matrix);
    if (steps != -1) {
        Vector2 current = robot + dir_vector * steps;
        for (int step = steps; step >= 0; step--) {
            auto next = current - dir_vector;
            matrix[current.x][current.y] = matrix[next.x][next.y];
            current = next;
        }
        matrix[robot.x][robot.y] = ".";
        robot = robot + dir_vector;
    }
    return robot;
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(/home/mihai/mihai_projs/adventOfCode24/day15/input2.test)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::split<std::string>(content, std::string("\n\n"));

    auto map = result[0];
    auto movement = result[1];

    auto matrix = utilities::splitText(map);
    // utilities::printMatrix(matrix, "Matrix");

    Vector2 robot{};
    bool robotFound = false;
    // find robot initial position @
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] == "@") {
                robot = {i, j};
                robotFound = true;
                break;
            }
        }

        if (robotFound) {
            break;
        }
    }

    for (const char &c : movement) {
        // check if c is not empty
        if (c != '\n') {
            robot = move_robot(robot, matrix, c);
            // utilities::printMatrix(matrix, "Matrix" + std::string(1, c));
        }
    }

    // utilities::printMatrix(matrix, "Matrix");

    uint64_t gps_sum = 0;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] == "O") {
                gps_sum += i * 100 + j;
            }
        }
    }

    std::cout << gps_sum << std::endl;

    return 0;
}
