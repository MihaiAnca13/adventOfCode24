#include "../utilities.h"


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
    const stringMatrix &matrix) {
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
            if (matrix[neighbor.x][neighbor.y] == "#") {
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
    std::filesystem::path path = R"(/home/mihai/mihai_projs/adventOfCode24/day20/input.txt)";
    std::string content = utilities::readFromFile(path);
    auto matrix = utilities::splitText(content);

    // utilities::printMatrix(matrix, "Matrix");

    auto start = utilities::find_char('S', matrix);
    auto end = utilities::find_char('E', matrix);

    std::cout << "Start: " << start << std::endl;
    std::cout << "End: " << end << std::endl;

    matrix[start.x][start.y] = ".";
    matrix[end.x][end.y] = ".";

    auto found_path = a_star(start, end, matrix);

    std::cout << "original: " << found_path.size() << std::endl;

    // create a set from found_path
    std::unordered_set<std::pair<Vector2, Vector2> > tried;

    int nr_shortcuts = 0;
    for (int i = 0; i < found_path.size(); i++) {
        const auto &pos = found_path[i];
        for (const auto &dir : four_directions) {
            auto next = pos + dir;
            auto next_next = next + dir;

            if (utilities::out_of_bounds(next_next.x, next_next.y, matrix.size(), matrix[0].size())) {
                continue;
            }

            if (matrix[next.x][next.y] == "#" && matrix[next_next.x][next_next.y] != "#") {
                if (tried.contains({pos, next_next})) {
                    continue;
                }

                tried.insert({pos, next_next});
                tried.insert({next_next, pos});

                // Find if next_next exists in our path
                auto it = std::find(found_path.begin() + i, found_path.end(), next_next);
                if (it != found_path.end()) {
                    int current_distance = std::distance(found_path.begin() + i, it);
                    // If going through the wall saves at least 100 steps
                    if (current_distance >= 102) {  // 102 because we're replacing with 2 steps (pos -> next_next)
                        nr_shortcuts++;
                    }
                }
            }
        }
    }

    std::cout << "shortcuts: " << nr_shortcuts << std::endl;

    return 0;
}
