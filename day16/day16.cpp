#include <climits>
#include <iostream>
#include "../utilities.h"
#include <unordered_map>
#include <queue>
#include <unordered_set>

Vector2 find_char(const std::vector<std::vector<std::string> > &matrix, const char c) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] == std::string(1, c)) {
                return {i, j};
            }
        }
    }
}

int cost(const Vector2& current_dir, const Vector2& new_dir) {
    if (current_dir == new_dir) {
        return 1;  // No direction change
    }
    // if (current_dir == Vector2{-new_dir.x, -new_dir.y}) {
    //     return 2001;  // Complete reversal (two 90-degree turns)
    // }
    return 1001;  // 90-degree turn
}

struct Node {
    Vector2 position, direction;
    int cost;

    bool operator>(const Node& other) const {
        return cost > other.cost;
    }

    bool operator==(const Node& other) const {
        return position == other.position && direction == other.direction;
    }
};

struct PathfindingResult {
    std::unordered_map<std::pair<Vector2, Vector2>, std::vector<std::pair<Vector2, Vector2>>> came_from;
    std::unordered_map<std::pair<Vector2, Vector2>, int> distances;
};

PathfindingResult dijkstra(
    const Vector2 start, const Vector2 end, Vector2 initial_dir,
    const std::vector<std::vector<std::string>>& matrix) {

    std::priority_queue<Node, std::vector<Node>, std::greater<>> pq;
    PathfindingResult result;
    auto& distances = result.distances;
    auto& came_from = result.came_from;

    // Initialize distances with infinity
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            for (Vector2 direction : {Vector2{0, 1}, Vector2{0, -1}, Vector2{1, 0}, Vector2{-1, 0}}) {
                distances[{{i, j}, direction}] = INT_MAX;
            }
        }
    }

    // Set start distance
    distances[{start, initial_dir}] = 0;
    pq.push({start, initial_dir, 0});

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        // If this state has been processed with a better cost, skip it
        if (current.cost > distances[{current.position, current.direction}]) {
            continue;
        }

        for (Vector2 direction : {Vector2{0, 1}, Vector2{0, -1}, Vector2{1, 0}, Vector2{-1, 0}}) {
            Vector2 neighbor = current.position + direction;

            // Check bounds and walls
            if (utilities::out_of_bounds(neighbor.x, neighbor.y, matrix.size(), matrix[0].size()) ||
                matrix[neighbor.x][neighbor.y] == "#") {
                continue;
            }

            int move_cost = cost(current.direction, direction);
            int new_cost = current.cost + move_cost;

            if (new_cost <= distances[{neighbor, direction}]) {
                if (new_cost < distances[{neighbor, direction}]) {
                    came_from[{neighbor, direction}] = {{current.position, current.direction}};
                } else {
                    came_from[{neighbor, direction}].emplace_back(current.position, current.direction);
                }

                distances[{neighbor, direction}] = new_cost;
                pq.push({neighbor, direction, new_cost});
            }
        }
    }

    return result;
}

std::unordered_set<Vector2> get_unique_path_tiles(
    const Vector2& end,
    const PathfindingResult& path_result) {
    std::unordered_set<Vector2> unique_tiles;
    const auto& came_from = path_result.came_from;
    const auto& distances = path_result.distances;

    // Find minimum distance to end
    int min_distance = INT_MAX;
    std::vector<std::pair<Vector2, Vector2>> optimal_end_states;

    for (auto dir : {Vector2{0, 1}, Vector2{0, -1}, Vector2{1, 0}, Vector2{-1, 0}}) {
        min_distance = std::min(min_distance, distances.at({end, dir}));
    }

    // Collect all end states with optimal distance
    for (auto dir : {Vector2{0, 1}, Vector2{0, -1}, Vector2{1, 0}, Vector2{-1, 0}}) {
        if (distances.at({end, dir}) == min_distance) {
            optimal_end_states.emplace_back(end, dir);
        }
    }

    // BFS to collect all tiles in optimal paths
    std::unordered_set<std::pair<Vector2, Vector2>> visited_states;
    std::queue<std::pair<Vector2, Vector2>> to_process;

    for (const auto& end_state : optimal_end_states) {
        to_process.push(end_state);
    }

    while (!to_process.empty()) {
        auto [pos, dir] = to_process.front();
        to_process.pop();

        if (!visited_states.insert({pos, dir}).second) {
            continue;
        }

        unique_tiles.insert(pos);

        // for all directions
        const int best_cost = distances.at({pos, dir});

        for (auto possible_dir : {Vector2{0, 1}, Vector2{0, -1}, Vector2{1, 0}, Vector2{-1, 0}}) {
            if (distances.at({pos, possible_dir}) == best_cost) {
                to_process.emplace(pos, possible_dir);
            }
        }

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
    std::filesystem::path path = R"(/home/mihai/mihai_projs/adventOfCode24/day16/input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::splitText(content);

    auto start = find_char(result, 'S');
    auto end = find_char(result, 'E');

    const Vector2 dir = {0, -1}; // initial direction

    auto path_result = dijkstra(start, end, dir, result);
    auto unique_tiles = get_unique_path_tiles(end, path_result);

    std::cout << "Number of unique tiles in optimal paths: " << unique_tiles.size() << std::endl;

    // Mark paths in the matrix
    for (auto& pos : unique_tiles) {
        result[pos.x][pos.y] = "O";
    }

    utilities::printMatrix(result, "Matrix");

    return 0;
}