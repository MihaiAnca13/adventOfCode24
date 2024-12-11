#include <iostream>
#include <cmath>
#include "../utilities.h"
#include <chrono>
#include <tuple>
#include <map>


uint64_t blink(uint64_t val, int num_blinks, std::map<std::tuple<uint64_t, int>, uint64_t> &cache) {
    if (num_blinks == 0) {
        return 1;
    }

    if (cache.find(std::make_tuple(val, num_blinks)) != cache.end()) {
        return cache[std::make_tuple(val, num_blinks)];
    }

    if (val == 0) {
        auto result = blink(1, num_blinks - 1, cache);
        cache[std::make_tuple(val, num_blinks)] = result;
        return result;
    }

    int num_digits = utilities::num_digits(val);

    if (num_digits % 2 == 0) {
        // split into two
        int half = num_digits / 2;
        uint64_t first = val / pow(10, half);
        uint64_t second = val % (uint64_t) pow(10, half);
        auto result = blink(first, num_blinks - 1, cache) + blink(second, num_blinks - 1, cache);
        cache[std::make_tuple(val, num_blinks)] = result;
        return result;
    }

    auto result = blink(val * 2024, num_blinks - 1, cache);
    cache[std::make_tuple(val, num_blinks)] = result;
    return result;
}


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
    uint64_t total_nums = 0;

    auto cache = std::map<std::tuple<uint64_t, int>, uint64_t>();
    for (auto v : state) {
        total_nums += blink(v, num_blinks, cache);
    }

    std::cout << total_nums << std::endl;


    return 0;
}
