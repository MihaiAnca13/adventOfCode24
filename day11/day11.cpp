#include <iostream>
#include <cmath>
#include "../utilities.h"
#include <chrono>


int main() {
    // read from input.txt
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day11\input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::split(content, std::string(" "))[0];

//    utilities::printRow(result);

    // convert to vector<uint64_t>
    std::vector<uint64_t> state;
    for (auto &i: result) {
        state.push_back((uint64_t) i);
    }

    int num_blinks = 75;

    for (int i = 0; i < num_blinks; ++i) {
        // start timer
        auto start = std::chrono::high_resolution_clock::now();
        // same size as state
        std::vector<uint64_t> new_state;
        for (auto val: state) {
            if (val == 0) {
                new_state.push_back(1);
            }
            else if (utilities::num_digits(val) % 2 == 0) {
                // split into two
                int num_digits = utilities::num_digits(val);
                int half = num_digits / 2;
                uint64_t first = val / pow(10, half);
                uint64_t second = val % (uint64_t) pow(10, half);
                new_state.push_back(first);
                new_state.push_back(second);
            }
            else {
                new_state.push_back(val * 2024);
            }
        }
        state = new_state;
        // end timer
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Blink " << i << ". Elapsed time: " << elapsed.count() << " s\n";
    }

//    utilities::printRow(state);
    std::cout << state.size() << std::endl;

    return 0;
}
