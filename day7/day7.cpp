#include <iostream>
#include <cmath>
#include "../utilities.h"


bool is_operation_correct(uint64_t key, std::vector<uint64_t> values, std::vector<std::string> operations) {
    uint64_t res = values[0];
    for (int i = 0; i < operations.size(); i++) {
        if (operations[i] == "+") {
            res += values[i + 1];
        }
        else if (operations[i] == "*") {
            res *= values[i + 1];
        }
        else if (operations[i] == "||") {
            // concatenate values: 6 || 4 = 64
            res = res * std::pow((uint64_t)10, std::to_string(values[i + 1]).length()) + values[i + 1];
        }
    }
    return res == key;
}


bool try_operations(uint64_t key, std::vector<uint64_t> values, std::vector<std::string> operations) {
    if (values.size() == 1) {
        return values[0] == key;
    }

    if (operations.size() < values.size() - 1) {
        std::vector<std::string> op_types = {"+", "*", "||"};
        for (const auto& op : op_types) {
            operations.push_back(op);
            if (try_operations(key, values, operations)) {
                return true;
            }
            operations.pop_back();
        }
    } else if (operations.size() == values.size() - 1) {
        if (is_operation_correct(key, values, operations)) {
            return true;
        }
    }

    return false;
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day7\input.txt)";
    std::string content = utilities::readFromFile(path);

    // split content by \n
    auto rows = utilities::splitIntoRows(content);

    uint64_t total = 0;
    for (auto &row : rows) {
        // find index of :
        size_t index = row.find(':');
        if (index != std::string::npos) {
            uint64_t key = std::stoll(row.substr(0, index));
            std::string rest = row.substr(index + 1);
            // split rest by " "
            auto values = utilities::split<std::string>(rest, " ");
            std::vector<uint64_t> result;
            for (auto &value : values) {
                if (!value.empty())
                    result.push_back(std::stoll(value));
            }

            if (try_operations(key, result, {})) {
                total += key;
            }
        }
    }

    std::cout << total << std::endl;


    return 0;
}
