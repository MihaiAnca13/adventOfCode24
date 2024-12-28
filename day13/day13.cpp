#include <iostream>
#include "../utilities.h"
#include <tuple>
#include <unordered_map>


struct Button {
    uint64_t x, y, cost;
};

std::pair<uint64_t, uint64_t> get_x_y(const std::string &condition) {
    auto condition_nums = utilities::split<std::string>(condition, std::string(": "))[1];
    auto condition_xy = utilities::split<std::string>(condition_nums, std::string(", "));
    uint64_t condition_x = std::stoi(condition_xy[0].substr(2, condition_xy[0].size() - 2));
    uint64_t condition_y = std::stoi(condition_xy[1].substr(2, condition_xy[1].size() - 2));
    return {condition_x, condition_y};
}

uint64_t cost(const std::vector<Button> &buttons, uint64_t target_x, uint64_t target_y) {
    std::unordered_map<std::pair<uint64_t, uint64_t>, uint64_t, PairHash> costs;
    std::vector<std::pair<uint64_t, uint64_t> > stack;

    // extract max cost of buttons
    uint64_t max_cost = 0;
    for (auto &button: buttons) {
        max_cost = std::max(button.cost, max_cost);
    }

    // start from (0, 0)
    costs[{0, 0}] = 0;
    stack.emplace_back(0, 0);

    while (!stack.empty()) {
        // get the last element
        auto [x, y] = stack.back();
        stack.pop_back();

        const auto current_cost = costs[{x, y}];

        for (auto &button: buttons) {
            uint64_t new_x = x + button.x;
            uint64_t new_y = y + button.y;

            const uint64_t new_cost = current_cost + button.cost;

            // new_cost > max_cost * 100 -> this should actually not be enforced? not sure why
            if (new_x > target_x || new_y > target_y) {
                continue;
            }

            if (!costs.contains({new_x, new_y}) || new_cost < costs[{new_x, new_y}]) {
                costs[{new_x, new_y}] = new_cost;
                stack.emplace_back(new_x, new_y);
            }
        }
    }

    if (!costs.contains({target_x, target_y})) {
        return -1;
    }

    return costs[{target_x, target_y}];
}

std::pair<double, double> cramer(const std::vector<std::vector<double>> &system) {
    double D = (system[0][0] * system[1][1]) - (system[1][0] * system[0][1]);
    double Dx = (system[0][2] * system[1][1]) - (system[1][2] * system[0][1]);
    double Dy = (system[0][0] * system[1][2]) - (system[1][0] * system[0][2]);

    double x = Dx / D;
    double y = Dy / D;

    return {x, y};
}

uint64_t cost_cramer(const std::vector<Button> &buttons, double target_x, double target_y) {
    std::vector<std::vector<double>> systems;
    uint64_t totalTokens = 0;

    systems.push_back({{static_cast<double>(buttons[0].x), static_cast<double>(buttons[1].x), target_x}});
    systems.push_back({{static_cast<double>(buttons[0].y), static_cast<double>(buttons[1].y), target_y}});

    auto [a, b] = cramer(systems);

    //&& a < 100 && b < 100
    if ((unsigned long long) a == a && (unsigned long long) b == b) {
        totalTokens += (a * 3) + (b * 1);
    }

    return totalTokens;
}

int main() {
    // read from input.txt
    std::filesystem::path path = R"(/home/mihai/mihai_projs/adventOfCode24/day13/input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::split<std::string>(content, std::string("\n\n"));

    uint64_t sum = 0;
    for (int i = 0; i < result.size(); i++) {
        const auto row = result[i];
        auto conditions = utilities::split<std::string>(row, std::string("\n"));
        auto [condition_a_x, condition_a_y] = get_x_y(conditions[0]);
        auto [condition_b_x, condition_b_y] = get_x_y(conditions[1]);
        Button button_a = {condition_a_x, condition_a_y, 3};
        Button button_b = {condition_b_x, condition_b_y, 1};
        auto [condition_t_x, condition_t_y] = get_x_y(conditions[2]);

        // auto res = cost({button_a, button_b}, condition_t_x + 10000000000000, condition_t_y + 10000000000000);
        auto res = cost_cramer({button_a, button_b}, static_cast<double>(condition_t_x + 10000000000000), static_cast<double>(condition_t_y + 10000000000000));
        if (res != -1) {
            sum += res;
        }
    }

    std::cout << sum << std::endl;

    return 0;
}
