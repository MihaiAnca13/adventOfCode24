#include <climits>
#include <iostream>
#include "../utilities.h"


struct Node {
    Vector2 position;
    int cost; // Total cost: f = g + h

    // Comparison operator for the min-heap
    bool operator>(const Node &other) const {
        return cost > other.cost; // Min-heap prioritizes lower cost
    }

    bool operator==(const Node &other) const {
        return position == other.position;
    }
};


int cost(const Vector2 current, const Vector2 end) {
    return utilities::manhattanDistance(current, end);
}


std::vector<Vector2> reconstruct_path(Vector2 current, const std::unordered_map<Vector2, Vector2> &came_from) {
    std::vector<Vector2> path;
    while (came_from.contains(current)) {
        path.push_back(current);
        current = came_from.at(current);
    }
    return path;
}


std::vector<Vector2> a_star(
    const Vector2 start, const Vector2 end,
    const std::vector<std::vector<int> > &matrix) {
    std::vector<Node> open;
    std::ranges::make_heap(open, std::greater<>{});
    open.push_back({start, 0});
    std::ranges::push_heap(open, std::greater<>{});

    std::unordered_map<Vector2, Vector2> came_from;
    std::unordered_map<Vector2, int> g_score;
    std::unordered_map<Vector2, int> f_score;

    // initialize g_score with infinity
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            g_score[{i, j}] = INT_MAX;
            f_score[{i, j}] = INT_MAX;
        }
    }
    g_score[start] = 0;
    f_score[start] = cost(start, end);

    while (!open.empty()) {
        std::ranges::pop_heap(open, std::greater<>{});
        Node current = open.back();
        open.pop_back();

        if (current.position == end) {
            return reconstruct_path(current.position, came_from);
        }

        for (Vector2 direction: {Vector2{0, 1}, Vector2{0, -1}, Vector2{1, 0}, Vector2{-1, 0}}) {
            Vector2 neighbor = current.position + direction;
            if (utilities::out_of_bounds(neighbor.x, neighbor.y, matrix.size(), matrix[0].size())) {
                continue;
            }
            if (matrix[neighbor.x][neighbor.y] == 1) {
                continue;
            }

            int tentative_g_score = g_score[current.position] + 1;

            if (tentative_g_score < g_score[neighbor]) {
                came_from[neighbor] = current.position;

                g_score[neighbor] = tentative_g_score;
                f_score[neighbor] = g_score[neighbor] + cost(neighbor, end);

                auto new_node = Node{neighbor, f_score[neighbor]};
                if (std::find(open.begin(), open.end(), new_node) == open.end()) {
                    open.push_back(new_node);
                    std::ranges::push_heap(open, std::greater<>{});
                }
            }
        }
    }

    return {};
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(/home/mihai/mihai_projs/adventOfCode24/day18/input.txt)";
    std::string content = utilities::readFromFile(path);
    auto rows = utilities::splitIntoRows(content);
    std::vector<Vector2> positions;

    for (auto &row: rows) {
        auto values = utilities::split<std::string>(row, ",");
        positions.push_back(Vector2(std::stoi(values[0]), std::stoi(values[1])));
    }

    // utilities::printRow(positions);

    const size_t height = 71, width = 71;
    const size_t nr_bytes = 1024;
    std::vector<std::vector<int> > matrix{height, std::vector<int>(width, 0)};

    for (int i = 0; i < nr_bytes; i++) {
        matrix[positions[i].y][positions[i].x] = 1;
    }

    // utilities::printMatrix(matrix);

    Vector2 start{0, 0}, end{height - 1, width - 1};
    std::vector<Vector2> found = a_star(start, end, matrix);

    for (auto &p: found) {
        matrix[p.x][p.y] = 2;
    }
    matrix[start.x][start.y] = 2;

    // utilities::printMatrix(matrix);

    std::cout << found.size() << std::endl;

    for (int i = nr_bytes; i < positions.size(); i++) {
        matrix[positions[i].y][positions[i].x] = 1;
        auto possible = a_star(start, end, matrix);
        if (possible.empty()) {
            std::cout << positions[i].x << " " << positions[i].y << std::endl;
            break;
        }
    }

    return 0;
}
