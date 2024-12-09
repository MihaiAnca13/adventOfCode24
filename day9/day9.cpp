#include <iostream>
#include "../utilities.h"


int main() {
    // read from input.txt
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day9\input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::splitText(content)[0];
    std::vector<int> repr;

//    utilities::printRow(result);

    for (int i = 0; i < result.size(); i++) {
        for (int n = 0; n < std::stoi(result[i]); n++) {
            if ( i % 2 == 0) {
                repr.push_back(i / 2);
            }
            else {
                repr.push_back(-1);
            }
        }
    }

//    utilities::printRow(repr);

    int i = 0, j = repr.size() - 1;
    while (i < j) {
        // find next -1 for i
        while (repr[i] != -1 && i < j) {
            i++;
        }

        // find next non -1 for j
        while (repr[j] == -1 && i < j) {
            j--;
        }

        if (i >= j) {
            break;
        }

        if (repr[i] == -1) {
            repr[i] = repr[j];
            repr[j] = -1;
            i++;
            j--;
        }
    }

//    utilities::printRow(repr);

    uint64_t checksum = 0;
    for (int idx = 0; idx < repr.size(); idx++) {
        if (repr[idx] != -1) {
            checksum += (uint64_t) repr[idx] * idx;
        } else {
            break;
        }
    }
    std::cout << checksum << std::endl;

    return 0;
}
