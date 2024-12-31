#include <iostream>
#include <set>
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

struct CircuitValidator {
    std::unordered_map<std::string, std::vector<Operation>> wire_operations;
    std::set<std::string> incorrect_outputs;
    int max_z_index = 0;
};

bool is_input_wire(const std::string& wire) {
    return wire[0] == 'x' || wire[0] == 'y';
}

void map_wire_operations(CircuitValidator& validator, const std::vector<Operation>& operations) {
    for (const auto& op : operations) {
        validator.wire_operations[op.gate1].push_back(op);
        validator.wire_operations[op.gate2].push_back(op);

        if (op.gate_res[0] == 'z') {
            const auto idx = std::stoi(op.gate_res.substr(1));
            validator.max_z_index = std::max(validator.max_z_index, idx);
        }
    }

    // Sort operations for each wire by operation type
    for (auto& [wire, ops] : validator.wire_operations) {
        std::sort(ops.begin(), ops.end(),
                  [](const auto& op1, const auto& op2) {
                      return op1.operation < op2.operation;
                  });
    }
}

void validate_operation(CircuitValidator& validator, const Operation& op) {
    const auto& gate1 = op.gate1;
    const auto& gate2 = op.gate2;
    const auto& result = op.gate_res;
    const auto& operation = op.operation;

    // Skip validation for first and last z-wires
    if (result[0] == 'z') {
        const auto idx = std::stoi(result.substr(1));
        if (idx == 0 || idx == validator.max_z_index) return;
    }

    const bool is_input1 = is_input_wire(gate1);
    const bool is_input2 = is_input_wire(gate2);

    // Basic input wire validation
    if (is_input1 && !is_input2) {
        validator.incorrect_outputs.insert(result);
    }

    // Operation-specific validation
    if (operation == "XOR") {
        if (is_input1 && result[0] == 'z') {
            validator.incorrect_outputs.insert(result);
        }
        if (is_input1 && (validator.wire_operations[result][0].operation != "AND" ||
                          validator.wire_operations[result][1].operation != "XOR")) {
            validator.incorrect_outputs.insert(result);
        }
        if (!is_input1 && result[0] != 'z') {
            validator.incorrect_outputs.insert(result);
        }
    }
    else if (operation == "AND") {
        if (validator.wire_operations[result].empty() ||
            validator.wire_operations[result][0].operation != "OR") {
            validator.incorrect_outputs.insert(result);
        }
    }
    else if (operation == "OR") {
        if (is_input1 || is_input2) {
            validator.incorrect_outputs.insert(result);
        }
        if (!validator.wire_operations.contains(result) ||
            validator.wire_operations[result].size() != 2 ||
            validator.wire_operations[result][0].operation != "AND" ||
            validator.wire_operations[result][1].operation != "XOR") {
            validator.incorrect_outputs.insert(result);
        }
    }
}

void remove_special_cases(CircuitValidator& validator, const std::vector<Operation>& operations) {
    for (const auto& op : operations) {
        if (op.gate1 == "x00" || op.gate2 == "x00" ||
            op.gate1 == "y00" || op.gate2 == "y00") {
            validator.incorrect_outputs.erase(op.gate_res);
        }
    }
}

int main() {
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day24\input.txt)";
    std::string content = utilities::readFromFile(path);
    auto parts = utilities::split<std::string>(content, std::string("\n\n"));
    auto rules = utilities::split<std::string>(parts[1], std::string("\n"));

    // Convert rules to operations
    std::vector<Operation> operations;
    for (auto& rule : rules) {
        operations.push_back(extract(rule));
    }

    // Initialize and run validator
    CircuitValidator validator;
    map_wire_operations(validator, operations);

    for (const auto& op : operations) {
        validate_operation(validator, op);
    }

    remove_special_cases(validator, operations);

    // Print results
    for (const auto& output : validator.incorrect_outputs) {
        std::cout << output << ',';
    }
    std::cout << '\n';

    return 0;
}