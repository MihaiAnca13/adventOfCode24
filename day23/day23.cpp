#include <iostream>
#include "../utilities.h"


// hash for array<string, 3>
struct hash_array {
    size_t operator()(const std::array<std::string, 3> &a) const {
        return std::hash<std::string>()(a[0]) ^ std::hash<std::string>()(a[1]) ^ std::hash<std::string>()(a[2]);
    }
};

std::unordered_set<std::array<std::string, 3>, hash_array> cycles;
std::unordered_map<std::string, std::vector<std::string>> map;


void find_cycle(const std::string &current, std::vector<std::string> path) {
    auto r = std::find(path.begin(), path.end(), "qp") != path.end();
    if (path.size() < 3 && std::find(path.begin(), path.end(), current) != path.end()) {
        return;
    }

    path.push_back(current);

    if (path.size() == 4) {
        if (path[0] == path[3]) {
            std::sort(path.begin(), path.end() - 1);
            cycles.insert({path[0], path[1], path[2]});
        }
        return;
    }

    for (auto &i: map[current]) {
        find_cycle(i, path);
    }
}


void find_all_cycles() {
    // get all keys from map
    auto all_nodes = std::vector<std::string>();
    for (auto &i: map) {
        if (i.first[0] == 't') {
            all_nodes.push_back(i.first);
        }
    }

    while (!all_nodes.empty()) {
        auto current = all_nodes[0];
        all_nodes.erase(all_nodes.begin());
        find_cycle(current, std::vector<std::string>());
    }
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day23\input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::split<std::string>(content, std::string("\n"));

//    utilities::printRow(result);

    for (auto &i: result) {
        if (i.empty()) continue;
        auto row = utilities::split<std::string>(i, std::string("-"));
        map[row[0]].push_back(row[1]);
        map[row[1]].push_back(row[0]);
    }

    find_all_cycles();

    std::cout << cycles.size() << std::endl;

    return 0;
}
