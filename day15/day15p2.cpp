#include <iostream>
#include "../utilities.h"
#include "../graphics_utils.h"

bool push(Vector2 current, Vector2 dir_vector, std::vector<std::vector<std::string>> &matrix, GridDrawer &gridDrawer) {
    auto next = current + dir_vector;

    if (matrix[next.x][next.y] == "#") {
        return false;
    }

    bool success = true;
    Vector2 neighbour{-1, -1};
    if (matrix[next.x][next.y] == "]") {
        neighbour = next + Vector2{0, -1};
        success = push(next, dir_vector, matrix, gridDrawer);
    } else if (matrix[next.x][next.y] == "[") {
        neighbour = next + Vector2{0, 1};
        success = push(next, dir_vector, matrix, gridDrawer);
    }

    if (success && dir_vector.x != 0 && neighbour.x != -1) {
        success = push(neighbour, dir_vector, matrix, gridDrawer);
    }

    if (success && matrix[next.x][next.y] == ".") {
        matrix[next.x][next.y] = matrix[current.x][current.y];
        matrix[current.x][current.y] = ".";
        // gridDrawer.updateMatrix(matrix);
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return true;
    }

    return false;
}

Vector2 get_dir(const char direction) {
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

    return dir_vector;
}

Vector2 move_robot(Vector2 robot, std::vector<std::vector<std::string>> &matrix, char direction, GridDrawer &gridDrawer) {
    const auto dir_vector = get_dir(direction);

    if (push(robot, dir_vector, matrix, gridDrawer)) {
        robot = robot + dir_vector;
    }
    // gridDrawer.updateMatrix(matrix);
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // utilities::printMatrix(matrix, "Matrix " + std::string(1, direction));

    return robot;
}

bool check_matrix(std::vector<std::vector<std::string>> &matrix) {
    // check if there's any [ . or . ]
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] == "[" && utilities::get_cell_value(matrix, i, j + 1) != "]") {
                return false;
            }
            if (matrix[i][j] == "]" && utilities::get_cell_value(matrix, i, j - 1) != "[") {
                return false;
            }
        }
    }

    return true;
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(/home/mihai/mihai_projs/adventOfCode24/day15/input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::split<std::string>(content, std::string("\n\n"));

    auto map = result[0];
    auto movement = result[1];

    auto matrix = utilities::splitText(map);

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] == "#") {
                matrix[i].insert(matrix[i].begin() + j + 1, "#");
                j++;
            }
            else if (matrix[i][j] == ".") {
                matrix[i].insert(matrix[i].begin() + j + 1, ".");
                j++;
            }
            else if (matrix[i][j] == "O") {
                matrix[i][j] = "[";
                matrix[i].insert(matrix[i].begin() + j + 1, "]");
                j++;
            }
            else if (matrix[i][j] == "@") {
                matrix[i].insert(matrix[i].begin() + j + 1, ".");
                j++;
            }
        }
    }

    int cell_size = 100;
    GridDrawer gridDrawer(matrix[0].size() * cell_size, matrix.size() * cell_size, cell_size);
    // Start the grid drawing thread
    std::thread drawingThread(&GridDrawer::run, &gridDrawer);
    std::this_thread::sleep_for(std::chrono::seconds(1));

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
            auto old_matrix = matrix;
            auto old_robot = robot;
            robot = move_robot(robot, matrix, c, gridDrawer);
            if (!check_matrix(matrix)) {
                robot = old_robot;
                matrix = old_matrix;
            }
            // utilities::printMatrix(matrix, "Matrix " + std::string(1, c));
        }
    }

    // utilities::printMatrix(matrix, "Matrix");

    uint64_t gps_sum = 0;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] == "[") {
                gps_sum += i * 100 + j;
            }
        }
    }

    std::cout << gps_sum << std::endl;

    return 0;
}
