#include <iostream>
#include "../utilities.h"
#include "../graphics_utils.h"


struct Robot {
    Vector2 pos;
    Vector2 vel;
};


Vector2 pass_time(const Vector2 pos, const Vector2 vel, const int width, const int height, const int time) {
    auto current_pos = pos;
    for (int i = 0; i < time; i++) {
        current_pos = current_pos + vel;
        current_pos.x = (current_pos.x % height + height) % height;
        current_pos.y = (current_pos.y % width + width) % width;
    }
    // std::cout << "pos: " << current_pos.x << " " << current_pos.y << std::endl;
    return current_pos;
}

int main() {
    // read from input.txt
    std::filesystem::path path = R"(/home/mihai/mihai_projs/adventOfCode24/day14/input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::split<std::string>(content, std::string("\n"));

    const int width = 101;
    const int height = 103;
    const int time = 6397;

    std::vector<Robot> robots;

    for (auto &row : result) {
        if (row == "") continue;
        auto pv = utilities::split<std::string>(row, std::string(" "));
        auto p = pv[0].substr(2, pv[0].size() - 2);
        auto v = pv[1].substr(2, pv[2].size() - 2);
        auto p_res = utilities::split<std::string>(p, ",");
        Vector2 pos(std::stoi(p_res[1]), std::stoi(p_res[0]));
        auto v_res = utilities::split<std::string>(v, ",");
        Vector2 vel(std::stoi(v_res[1]), std::stoi(v_res[0]));

        auto robot_pos = pass_time(pos, vel, width, height, time);
        robots.push_back({robot_pos, vel});
    }

    GridDrawer gridDrawer(width * 10, height * 10, 10);

    // Start the grid drawing thread
    std::thread drawingThread(&GridDrawer::run, &gridDrawer);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    uint64_t idx = time;
    while (gridDrawer.isOpen() && idx < 48009) {
        std::vector<std::vector<int>> matrix(height, std::vector<int>(width, 0));

        for (auto &robot : robots) {
            auto robot_pos = pass_time(robot.pos, robot.vel, width, height, 1);
            robot.pos = robot_pos;
            matrix[robot.pos.x][robot.pos.y]++;
        }
        idx++;

        // get max value of matrix
        uint64_t max = 0;
        double four_n = 0;
        auto nr_robots = static_cast<double>(robots.size());
        bool t_shape = false;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                // compute number of neighbours
                int nr_neighbours = 0;
                if (i > 0 && matrix[i - 1][j] > 0) {
                    nr_neighbours++;
                }
                if (i < height - 1 && matrix[i + 1][j] > 0) {
                    nr_neighbours++;
                }
                if (j > 0 && matrix[i][j - 1] > 0) {
                    nr_neighbours++;
                }
                if (j < width - 1 && matrix[i][j + 1] > 0) {
                    nr_neighbours++;
                }

                if (nr_neighbours >= 2) {
                    four_n++;
                }

                if (matrix[i][j] > max) {
                    max = matrix[i][j];
                }

                // get T shape neighbours
                auto current = matrix[i][j];
                auto above = utilities::get_cell_value(matrix, i - 1, j);
                auto above_left = utilities::get_cell_value(matrix, i - 1, j - 1);
                auto above_right = utilities::get_cell_value(matrix, i - 1, j + 1);
                auto above_above = utilities::get_cell_value(matrix, i - 2, j);

                // check all these are different from NULL and above 0
                if (current > 0 && above > 0 && above_left > 0 && above_right > 0 && above_above > 0) {
                    t_shape = true;
                    matrix[i][j] = -100;
                    matrix[i - 1][j] = -100;
                    matrix[i - 1][j - 1] = -100;
                    matrix[i - 1][j + 1] = -100;
                    matrix[i - 2][j] = -100;
                }
            }
        }

        gridDrawer.updateMatrix(matrix);

        // if (max > 3) {
        //     std::cout << "Max: " << max << " at " << idx << std::endl;
        //     std::this_thread::sleep_for(std::chrono::milliseconds(200));
        // }
        // if (four_n / nr_robots > 0.35f) {
        //     std::cout << "Four neighbours: " << four_n << " at " << idx << std::endl;
        //     std::this_thread::sleep_for(std::chrono::milliseconds(200));
        // }
        if (t_shape) {
            std::cout << "T shape at: " << idx << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }

        // std::cout << ++i << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    drawingThread.join();

    auto middle_row = height / 2;
    auto middle_col = width / 2;

    uint64_t safety = 0, q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    for (auto &robot : robots) {
        if (robot.pos.x < middle_row && robot.pos.y < middle_col) {
            q1++;
        } else if (robot.pos.x < middle_row && robot.pos.y > middle_col) {
            q2++;
        } else if (robot.pos.x > middle_row && robot.pos.y < middle_col) {
            q3++;
        } else if (robot.pos.x > middle_row && robot.pos.y > middle_col) {
            q4++;
        }
    }

    safety = q1 * q2 * q3 * q4;

    std::cout << q1 << " " << q2 << " " << q3 << " " << q4 << std::endl;
    std::cout << "Safety: " << safety << std::endl;

    return 0;
}
