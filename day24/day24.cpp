#include <iostream>
#include <queue>
#include "../utilities.h"


struct Operation {
    std::string gate1, gate2, gate_res;
    std::string operation;
};


Operation extract(std::string &rule) {
    Operation op;
    auto parts = utilities::split<std::string>(rule, std::string(" "));

    for (int i = 0; i < parts.size(); i++) {
        if (i == 1) {
            op.operation = parts[i];
        }
        if (i == 0) {
            op.gate1 = parts[i];
        } else if (i == 2) {
            op.gate2 = parts[i];
        } else if (i == 4) {
            op.gate_res = parts[i];
        }
    }

    return op;
}

bool apply_op(Operation &op, std::unordered_map<std::string, bool> &gates) {
    // check if the gates are available
    if (!gates.contains(op.gate1) || !gates.contains(op.gate2)) {
        return false;
    }

    if (op.operation == "AND") {
        gates[op.gate_res] = gates[op.gate1] && gates[op.gate2];
    } else if (op.operation == "OR") {
        gates[op.gate_res] = gates[op.gate1] || gates[op.gate2];
    } else { // XOR
        gates[op.gate_res] = gates[op.gate1] ^ gates[op.gate2];
    }

    return true;
}


void check_leftover(std::vector<Operation> &leftover, std::unordered_map<std::string, bool> &gates) {
    bool changed = true;
    while (changed) {
        changed = false;
        if (!leftover.empty()) {
            // check if the leftover operations can be applied
            for (int i = 0; i < leftover.size(); i++) {
                auto l = leftover[i];
                if (apply_op(l, gates)) {
                    leftover.erase(leftover.begin() + i);
                    changed = true;
                    break;
                }
            }
        }
    }
}


typedef std::unordered_map<char, std::vector<bool>> AlgOutput;

void print_values(std::vector<bool> &values) {
    uint64_t number = 0;
    for (size_t i = 0; i < values.size(); ++i) {
        std::cout << values[values.size() - i - 1] << "";
        if (values[i]) {
            number += (1ULL << i); // Add 2^i if the bit is 1
        }
    }
    std::cout << std::endl;
    std::cout << "The number is: " << number << std::endl;
}


AlgOutput run_alg(const std::unordered_map<std::string, bool> &gates_org, std::vector<std::string> &rules) {
    auto gates = gates_org;

    std::vector<Operation> leftover;
    for (auto &r : rules) {
        auto op = extract(r);
        check_leftover(leftover, gates);
        if (!apply_op(op, gates)) {
            leftover.push_back(op);
        }
    }

    // apply the leftover operations
    check_leftover(leftover, gates);

    // count all gates starting with z and print their values
    int count = 0;
    for (auto &g : gates) {
        if (g.first[0] == 'z') {
            count++;
        }
    }

    // initialize a vector of bools of size count
    AlgOutput output;
    for (auto &c : {'x', 'y', 'z'}) {
        std::vector<bool> values(count);
        for (auto &g : gates) {
            if (g.first[0] == c) {
                auto nr = std::stoi(g.first.substr(1));
                values[nr] = g.second;
            }
        }
        output[c] = values;
    }

    return output;
}

std::vector<int> check_x_y_z(AlgOutput &values) {
    // x, y, z are vectors of bools, representing numbers in binary, with the least significant bit at the end
    // check if x + y == z
    // keep track of reminders
    int carry = 0;
    std::vector<int> bad_indices{};

    for (int i = 0; i < values['x'].size(); ++i) {
        int sum = values['x'][i] + values['y'][i] + carry;
        if (sum % 2 != values['z'][i]) {
            bad_indices.emplace_back(i);
        }
        carry = sum / 2;
    }
    // check if
    auto last_z = values['z'][values['z'].size() - 1];
    if (values['z'].size() > values['x'].size() && carry != last_z) {
        bad_indices.emplace_back(values['z'].size() - 1);
    }

    return bad_indices;
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day24\input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::split<std::string>(content, std::string("\n\n"));

    auto inputs = utilities::split<std::string>(result[0], std::string("\n"));
    auto rules = utilities::split<std::string>(result[1], std::string("\n"));

    std::unordered_map<std::string, bool> gates;

    for (auto &i : inputs) {
        auto input = utilities::split<std::string>(i, std::string(": "));
        gates[input[0]] = input[1] == "1";
    }

    auto values = run_alg(gates, rules);

//    print_values(values['z']);

    auto bad_idx = check_x_y_z(values);

    utilities::printRow(bad_idx);

    return 0;
}
