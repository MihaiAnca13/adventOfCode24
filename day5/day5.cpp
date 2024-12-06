#include <iostream>
#include "../utilities.h"
#include <map>


void fixRow(std::vector<float> &row, std::map<float, std::vector<float>> &rules_map) {
    // while not valid, reverse elements according to rule
    bool valid = false;
    while (!valid) {
        valid = true;
        std::map<float, int> visited;
        for (int i = 0; i < row.size(); i++) {
            auto n = row[i];
            // loop over keys of visited
            for (auto &v : visited) {
                for (auto &r : rules_map[v.first]) {
                    if (r == n) {
                        // swap elements according to rule
                        std::swap(row[i], row[v.second]);
                        valid = false;
                        break;
                    }
                }
                if (!valid) {
                    break;
                }
            }
            if (!valid) {
                break;
            }
            visited[n] = i;
        }
    }
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day5\input.txt)";
    std::string content;
    content = utilities::readFromFile(path);

    // split content by \n\n into 2 parts. first contains rules, second contains messages
    // find the index of the first empty line
    size_t pos = content.find("\n\n");
    std::string rules_str = content.substr(0, pos);
    std::string messages_str = content.substr(pos + 2);

    // split rules
    auto rules = utilities::split(rules_str, "|");
    auto messages = utilities::split(messages_str, ",");

    // print rules
//    utilities::printMatrix(rules, "Rules");

    // print messages
//    utilities::printMatrix(messages, "Messages");

    // convert rules to map
    std::map<float, std::vector<float>> rules_map;
    for (auto &rule: rules) {
        rules_map[rule[1]].push_back(rule[0]);
    }

    float total = 0;
    float total_invalid = 0;

    // go over each message and check if it's valid
    for (auto &row: messages) {
        std::vector<float> visited;
        bool valid = true;
        for (auto &n: row) {
            for (auto &v : visited) {
                for (auto &r : rules_map[v]) {
                    if (r == n) {
                        valid = false;
                        break;
                    }
                }
                if (!valid) {
                    break;
                }
            }
            if (!valid) {
                break;
            }
            visited.push_back(n);
        }
        if (valid) {
            // get element in middle of row
            float middle = row[row.size() / 2];
            total += middle;
        } else {
            fixRow(row, rules_map);
            float middle = row[row.size() / 2];
            total_invalid += middle;
        }
    }

    std::cout << "Total: " << total << std::endl;
    std::cout << "Total invalid: " << total_invalid << std::endl;

    return 0;
}
