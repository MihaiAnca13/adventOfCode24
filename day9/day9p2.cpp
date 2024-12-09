#include <iostream>
#include "../utilities.h"

typedef struct {
    int length, idx;
    bool empty;
} File;



int main() {
    // read from input.txt
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day9\input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::splitText(content)[0];
    std::vector<File> repr;

//    utilities::printRow(result);

    for (int i = 0; i < result.size(); i++) {
        if ( i % 2 == 0) {
            repr.push_back({std::stoi(result[i]), i / 2, false});
        }
        else {
            repr.push_back({std::stoi(result[i]), -1, true});
        }
    }

//    for (const auto& file : repr) {
//        std::cout << file.idx << " " << file.length << " " << file.empty << std::endl;
//    }

    int i  = 1;
    int latest_contiguous = 1;
    while (i < repr.size()) {
        if (!repr[i].empty) {
            i++;
            continue;
        }
        latest_contiguous = std::min(latest_contiguous, i);
        for (int j = repr.size() - 1; j > i; j--) {
            if (repr[j].empty) {
                continue;
            }
            if (repr[i].length >= repr[j].length) {
                // swap i and j
                File temp = repr[i];
                repr[i] = repr[j];
                repr[j] = temp;

                auto diff = repr[j].length - repr[i].length;

                if (diff > 0) {
                    // add a new file with length diff and empty true
                    repr.insert(repr.begin() + i + 1, {diff, -1, true});
                    // update j
                    j++;
                }

                repr[j].length -= diff;

                i = latest_contiguous;
                break;
            }
        }
        i++;
    }

    std::vector<int> result_repr;
    for (auto & file : repr) {
        for (int n = 0; n < file.length; n++) {
            if ( !file.empty ) {
                result_repr.push_back(file.idx);
            }
            else {
                result_repr.push_back(-1);
            }
        }
    }

//    utilities::printRow(result_repr);

    uint64_t checksum = 0;
    for (int idx = 0; idx < result_repr.size(); idx++) {
        if (result_repr[idx] != -1) {
            checksum += (uint64_t) result_repr[idx] * idx;
        }
    }
    std::cout << checksum << std::endl;

    return 0;
}
