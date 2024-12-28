#include <iostream>
#include "../utilities.h"
#include <unordered_set>


uint64_t get_combo(int operand, uint64_t register_a, uint64_t register_b, uint64_t register_c) {
    if (operand < 4) {
        return operand;
    }
    switch (operand) {
        case 4:
            return register_a;
        case 5:
            return register_b;
        case 6:
            return register_c;
        default:
            std::cout << "case 7 not implemented" << std::endl;
            return 0;
    }
}


std::vector<uint64_t> run_program(uint64_t register_a, uint64_t register_b, uint64_t register_c,
                                  const std::vector<uint64_t> &program, bool break_early = false) {
    std::vector<uint64_t> result;
    uint64_t res = 0;
    for (int i = 0; i < program.size() - 1; i += 2) {
        auto instruction = program[i];
        auto literal_operand = program[i + 1];
        auto combo_operand = get_combo(program[i + 1], register_a, register_b, register_c);
        switch (instruction) {
            case 0: // A / (2^combo)
                register_a = register_a >> combo_operand;
                break;
            case 1: // bitwise xor B and literal
                register_b = register_b ^ literal_operand;
                break;
            case 2: // B = combo % 8
                register_b = combo_operand % 8ULL;
                break;
            case 3: // nothing if A is 0, otherwise i = literal - 2
                if (register_a == 0) {
                    break;
                }
                if (literal_operand >= program.size()) {
                    // out of bounds
                    i = program.size();
                    std::cout << "Out of bounds" << std::endl;
                    break;
                }
                i = literal_operand - 2;
                break;
            case 4: // bitwise xor between b and c
                register_b = register_b ^ register_c;
                break;
            case 5: // combo % 8
                res = combo_operand % 8ULL;
                result.push_back(res);
                if (break_early && res != program[result.size() - 1]) {
                    return result;
                }
                break;
            case 6: // same as 0 but stored in b
                register_b = register_a >> combo_operand;
                break;
            case 7: // same as 0 but stored in c
                register_c = register_a >> combo_operand;
                break;
            default:
                break;
        }
    }

    return result;
}



void dfs(std::vector<uint64_t> &program, int index, uint64_t register_a) {
    if (index < 0) {
        return;
    }

    auto target = program[index];
    for (uint64_t i = 0; i < 1024; i++) {
        auto out = run_program(register_a << 3 | i, 0, 0, program);
        bool equal = true;
        for (int j = 0; j < out.size(); j++) {
            auto idx = program.size() - out.size() + j;
            if (out[j] != program[idx]) {
                equal = false;
            }
        }
        if (equal) {
            register_a = register_a << 3 | i;
            std::cout << "For ";
            utilities::printRow(out);
            std::cout << "Register A: " << register_a << std::endl;
            return dfs(program, index - 1, register_a);
        }
    }
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(/home/mihai/mihai_projs/adventOfCode24/day17/input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::split<std::string>(content, std::string("\n"));

    auto register_a = std::stoll(result[0].substr(12, result[0].size() - 12));
    auto register_b = std::stoll(result[1].substr(12, result[1].size() - 12));
    auto register_c = std::stoll(result[2].substr(12, result[2].size() - 12));
    auto program_str = result[4].substr(9, result[4].size() - 9);
    auto program = utilities::str_row_to_int(program_str, ",");

    // std::cout << "Register A: " << register_a << std::endl;
    // std::cout << "Register B: " << register_b << std::endl;
    // std::cout << "Register C: " << register_c << std::endl;
    utilities::printRow(program);

    auto res = run_program(register_a, register_b, register_c, program);
    utilities::printRow(res);

    dfs(program, program.size() - 1, 0);

    return 0;
}
