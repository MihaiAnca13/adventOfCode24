#include <iostream>
#include "../utilities.h"


uint64_t find_match(std::string &pattern, const std::vector<std::string> &towels, std::unordered_map<std::string, uint64_t> &memo) {
    if (pattern.empty()) {
        return 1;
    }

    // check memo
    if (memo.contains(pattern)) {
        return memo[pattern];
    }

    uint64_t count = 0;
    for (auto &towel : towels) {
        if (pattern.find(towel) == 0) {
            std::string new_pattern = pattern.substr(towel.size());
            count += find_match(new_pattern, towels, memo);
        }
    }

    memo[pattern] = count;
    return count;
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(/home/mihai/mihai_projs/adventOfCode24/day19/input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::split<std::string>(content, std::string("\n\n"));
    auto towels_str = result[0];
    auto towels = utilities::split<std::string>(towels_str, std::string(", "));

    // sort towels based on length
    std::sort(towels.begin(), towels.end(), [](const std::string &a, const std::string &b) {
        return a.size() > b.size();
    });

    auto patterns_str = result[1];
    auto requested = utilities::split<std::string>(patterns_str, std::string("\n"));

    // utilities::printRow(towels);
    // utilities::printRow(requested);

    uint64_t nr_matches = 0;
    std::unordered_map<std::string, uint64_t> memo;
    for (auto &pattern: requested) {
        if (pattern.empty()) {
            continue;
        }
        nr_matches += find_match(pattern, towels, memo);
    }

    std::cout << nr_matches << std::endl;

    return 0;
}
