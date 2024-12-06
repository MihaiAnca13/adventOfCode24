#include <iostream>
#include "../utilities.h"


int main() {
    // read from input.txt
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day6\input.test)";
    std::string content;
    content = utilities::readFromFile(path);
    auto result = utilities::split(content, std::string("   "));

    for (auto &i : result) {
        for (auto &j : i) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
