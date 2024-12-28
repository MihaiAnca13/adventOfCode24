#include <climits>
#include <iostream>
#include "../utilities.h"
#include <unordered_map>
#include <cmath>
#include <queue>
#include <set>


void draw(const std::vector<std::pair<Vector2, Vector2> > &path, const std::vector<std::vector<std::string> > &matrix) {
    auto matrix_copy = matrix;
    for (auto &i: path) {
        std::string symbol;
        if (i.second == Vector2{0, 1}) {
            symbol = '>';
        } else if (i.second == Vector2{0, -1}) {
            symbol = '<';
        } else if (i.second == Vector2{1, 0}) {
            symbol = 'v';
        } else if (i.second == Vector2{-1, 0}) {
            symbol = '^';
        }
        matrix_copy[i.first.x][i.first.y] = symbol;
    }
    utilities::printMatrix(matrix_copy, "Matrix");
}


Vector2 find_char(const std::vector<std::vector<std::string> > &matrix, const char c) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] == std::string(1, c)) {
                return {i, j};
            }
        }
    }
}


struct Node {
    Vector2 position, direction;
    int cost; // Total cost: f = g + h

    // Comparison operator for the min-heap
    bool operator>(const Node &other) const {
        return cost > other.cost; // Min-heap prioritizes lower cost
    }

    bool operator==(const Node &other) const {
        return position == other.position && direction == other.direction;
    }
};

int cost(const Vector2 current, const Vector2 end, const Vector2 dir) {
    int distance = utilities::manhattanDistance(current, end);
    Vector2 diff = end - current;
    // compute angle between diff and dir
    double angle = std::abs(std::atan2(diff.y, diff.x) - std::atan2(dir.y, dir.x));
    angle = std::min(angle, M_PI * 2 - angle);
    int direction = std::ceil(angle / (M_PI / 2));
    return distance + direction * 1000;
}


std::vector<std::pair<Vector2, Vector2> > reconstruct_path(Vector2 current, Vector2 dir,
                                                           const std::unordered_map<std::pair<Vector2, Vector2>,
                                                               std::pair<Vector2, Vector2> > &came_from) {
    std::vector<std::pair<Vector2, Vector2> > path;
    path.emplace_back(current, dir);
    while (came_from.contains({current, dir})) {
        auto [fst, snd] = came_from.at({current, dir});
        current = fst;
        dir = snd;
        path.emplace_back(current, dir);
    }
    return path;
}


struct PathfindingResult {
    std::unordered_map<std::pair<Vector2, Vector2>, std::vector<std::pair<Vector2, Vector2>>> came_from;
    std::unordered_map<std::pair<Vector2, Vector2>, int> g_score;
};

PathfindingResult a_star(
    const Vector2 start, const Vector2 end, Vector2 dir,
    const std::vector<std::vector<std::string>>& matrix) {
    std::vector<Node> open;
    std::ranges::make_heap(open, std::greater<>{});
    open.push_back({start, dir, 0});
    std::ranges::push_heap(open, std::greater<>{});

    PathfindingResult result;
    auto& came_from = result.came_from;
    auto& g_score = result.g_score;
    std::unordered_map<std::pair<Vector2, Vector2>, int> f_score;

    // initialize g_score with infinity
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            for (Vector2 direction : {Vector2{0, 1}, Vector2{0, -1}, Vector2{1, 0}, Vector2{-1, 0}}) {
                g_score[{{i, j}, direction}] = INT_MAX;
                f_score[{{i, j}, direction}] = INT_MAX;
            }
        }
    }
    g_score[{start, dir}] = 0;
    f_score[{start, dir}] = cost(start, end, dir);

    while (!open.empty()) {
        std::ranges::pop_heap(open, std::greater<>{});
        Node current = open.back();
        open.pop_back();

        if (current.position == end) {
            continue;
        }

        for (Vector2 direction : {Vector2{0, 1}, Vector2{0, -1}, Vector2{1, 0}, Vector2{-1, 0}}) {
            Vector2 neighbor = current.position + direction;
            if (utilities::out_of_bounds(neighbor.x, neighbor.y, matrix.size(), matrix[0].size())) {
                continue;
            }
            if (matrix[neighbor.x][neighbor.y] == "#") {
                continue;
            }

            int tentative_g_score = g_score[{current.position, current.direction}] +
                                  cost(current.position, neighbor, current.direction);

            if (tentative_g_score <= g_score[{neighbor, direction}]) {
                if (tentative_g_score < g_score[{neighbor, direction}]) {
                    came_from[{neighbor, direction}] = {{current.position, current.direction}};
                } else {
                    came_from[{neighbor, direction}].emplace_back(current.position, current.direction);
                }

                g_score[{neighbor, direction}] = tentative_g_score;
                f_score[{neighbor, direction}] = g_score[{neighbor, direction}] +
                                               cost(neighbor, end, direction);

                auto new_node = Node{neighbor, direction, f_score[{neighbor, direction}]};
                if (std::find(open.begin(), open.end(), new_node) == open.end()) {
                    open.push_back(new_node);
                    std::ranges::push_heap(open, std::greater<>{});
                }
            }
        }
    }

    return result;
}

std::set<Vector2> get_unique_path_tiles(
    const Vector2& end,
    const PathfindingResult& path_result) {

    std::set<Vector2> unique_tiles;
    const auto& came_from = path_result.came_from;
    const auto& g_score = path_result.g_score;

    // Find all end states with optimal cost
    int min_cost = INT_MAX;
    std::vector<std::pair<Vector2, Vector2>> optimal_end_states;

    for (const auto& score_entry : g_score) {
        if (score_entry.first.first == end) {
            if (score_entry.second < min_cost) {
                min_cost = score_entry.second;
                optimal_end_states.clear();
                optimal_end_states.push_back(score_entry.first);
            } else if (score_entry.second == min_cost) {
                optimal_end_states.push_back(score_entry.first);
            }
        }
    }

    std::set<std::pair<Vector2, Vector2>> visited_states;
    std::queue<std::pair<Vector2, Vector2>> to_process;

    // Add initial end states
    for (const auto& end_state : optimal_end_states) {
        to_process.push(end_state);
    }

    while (!to_process.empty()) {
        auto [pos, dir] = to_process.front();
        to_process.pop();

        if (!visited_states.insert({pos, dir}).second) {
            continue;  // Already processed this state
        }

        unique_tiles.insert(pos);

        auto it = came_from.find({pos, dir});
        if (it == came_from.end()) continue;

        for (const auto& parent : it->second) {
            to_process.push(parent);
        }
    }

    return unique_tiles;
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(/home/mihai/mihai_projs/adventOfCode24/day16/input.test)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::splitText(content);

    // utilities::printMatrix(result, "Matrix");

    auto start = find_char(result, 'S');
    auto end = find_char(result, 'E');

std::cout << start.x << " " << start.y << std::endl;
    std::cout << end.x << " " << end.y << std::endl;

    const Vector2 dir = {0, -1}; // facing E (left)

    auto path_result = a_star(start, end, dir, result);
    auto unique_tiles = get_unique_path_tiles(end, path_result);

    // print length of unique tiles
    std::cout << "Length of unique tiles: " << unique_tiles.size() << std::endl;

    // replace all unique tiles with 'O'
    for (auto &i: unique_tiles) {
        result[i.x][i.y] = "O";
    }

    utilities::printMatrix(result, "Matrix");

    return 0;
}
