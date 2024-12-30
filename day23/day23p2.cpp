#include <bitset>
#include "../utilities.h"

const int MAX_NODES = 525;
std::vector<std::vector<bool>> adj_matrix(MAX_NODES, std::vector<bool>(MAX_NODES));
std::unordered_map<std::string, int> node_to_id;
std::vector<std::string> id_to_node;
int node_count = 0;

int get_node_id(const std::string& node) {
    auto it = node_to_id.find(node);
    if (it != node_to_id.end()) return it->second;

    node_to_id[node] = node_count;
    id_to_node.push_back(node);
    return node_count++;
}

// Bron-Kerbosch recursive function
void bron_kerbosch(std::vector<int>& R, std::vector<int>& P, std::vector<int>& X, int k) {
    if (R.size() == k) {
        // Found a clique of size k
        std::vector<std::string> clique;
        for (int node : R) {
            clique.push_back(id_to_node[node]);
        }
        std::sort(clique.begin(), clique.end());
        for (const auto& node : clique) {
            std::cout << node << ",";
        }
        std::cout << std::endl;
        return;
    }

    if (P.empty() && X.empty()) {
        return; // No more candidates to extend the clique
    }

    std::vector<int> P_copy = P; // Copy P to iterate safely
    for (int v : P_copy) {
        R.push_back(v);

        // Create new sets for neighbors of v
        std::vector<int> new_P, new_X;
        for (int neighbor = 0; neighbor < node_count; ++neighbor) {
            if (adj_matrix[v][neighbor]) {
                if (std::find(P.begin(), P.end(), neighbor) != P.end()) {
                    new_P.push_back(neighbor);
                }
                if (std::find(X.begin(), X.end(), neighbor) != X.end()) {
                    new_X.push_back(neighbor);
                }
            }
        }

        bron_kerbosch(R, new_P, new_X, k);

        // Backtrack
        R.pop_back();
        P.erase(std::remove(P.begin(), P.end(), v), P.end());
        X.push_back(v);
    }
}

int main() {
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day23\input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::split<std::string>(content, std::string("\n"));

    for (const auto& line : result) {
        if (line.empty()) continue;
        auto row = utilities::split<std::string>(line, std::string("-"));
        int from_id = get_node_id(row[0]);
        int to_id = get_node_id(row[1]);
        adj_matrix[from_id][to_id] = true;
        adj_matrix[to_id][from_id] = true;
    }

    // Prepare candidate nodes
    std::vector<int> P, X, R;
    for (int i = 0; i < node_count; ++i) {
        P.push_back(i);
    }

    int k = 13; // Example clique size
    bron_kerbosch(R, P, X, k);

    return 0;
}
