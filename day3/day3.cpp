#include <iostream>
#include "../utilities.h"
#include <regex>


int main() {
    // read from input.txt
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day3\input.txt)";
    std::string content;
    content = utilities::readFromFile(path);

//    std::cout << "Content: " << content << std::endl;

    std::regex pattern(R"(mul\([0-9]+,[0-9]+\))");

    auto matches_begin = std::sregex_iterator(content.begin(), content.end(), pattern);
    auto matches_end = std::sregex_iterator();

    int total = 0;
    for (std::sregex_iterator it = matches_begin; it != matches_end; ++it) {
        std::smatch match = *it;
        std::string match_str = match.str();
        auto dot_index = match_str.find(",");
        // first number is between 3rd character and first comma
        auto first = std::stoi(match_str.substr(4, dot_index - 4));
        // second number is between first comma and last character - 1
        auto second = std::stoi(match_str.substr( dot_index + 1, match_str.size() - dot_index - 2));
        total += first * second;
    }

    std::cout << "Total: " << total << std::endl;

    std::regex flag_pattern(R"((do\(\)|don't\(\)))");

    // add "do()" to the end of the content to make sure the last part is also processed
    content += "do()";

    auto flag_matches_begin = std::sregex_iterator(content.begin(), content.end(), flag_pattern);
    auto flag_matches_end = std::sregex_iterator();

    int total2 = 0;
    bool enabled = true;
    auto current_pos = content.begin();
    for (std::sregex_iterator it = flag_matches_begin; it != flag_matches_end; ++it) {
        std::smatch match = *it;
        std::string match_str = match.str();
        auto match_pos = match.position();

        if (enabled) {
            auto matches_begin_sub = std::sregex_iterator(current_pos, content.begin() + match_pos, pattern);
            auto matches_end_sub = std::sregex_iterator();
            for (std::sregex_iterator it_sub = matches_begin_sub; it_sub != matches_end_sub; ++it_sub) {
                std::smatch match_sub = *it_sub;
                std::string match_str_sub = match_sub.str();
                auto dot_index = match_str_sub.find(",");
                // first number is between 3rd character and first comma
                auto first = std::stoi(match_str_sub.substr(4, dot_index - 4));
                // second number is between first comma and last character - 1
                auto second = std::stoi(match_str_sub.substr( dot_index + 1, match_str_sub.size() - dot_index - 2));
                total2 += first * second;
            }
        }

        if (match_str == "do()") {
            current_pos = content.begin() + match_pos + 4;
            enabled = true;
        } else {
            enabled = false;
        }
    }

    std::cout << "Total: " << total2 << std::endl;

    return 0;
}
