#include <iostream>
#include "../utilities.h"


int NR_ROBOTS = 25;


/*
+---+---+---+
| 7 | 8 | 9 |
+---+---+---+
| 4 | 5 | 6 |
+---+---+---+
| 1 | 2 | 3 |
+---+---+---+
    | 0 | A |
    +---+---+
 */
std::unordered_map<char, Vector2> numeric_keypad = {
        {'A', {3, 2}},
        {'0', {3, 1}},
        {'1', {2, 0}},
        {'2', {2, 1}},
        {'3', {2, 2}},
        {'4', {1, 0}},
        {'5', {1, 1}},
        {'6', {1, 2}},
        {'7', {0, 0}},
        {'8', {0, 1}},
        {'9', {0, 2}}
};

/*
    +---+---+
    | ^ | A |
+---+---+---+
| < | v | > |
+---+---+---+
 */
std::unordered_map<char, Vector2> directional_keypad = {
        {'A', {0, 2}},
        {'<', {1, 0}},
        {'^', {0, 1}},
        {'>', {1, 2}},
        {'v', {1, 1}}
};


struct Movement {
    std::vector<char> sequence;
    Vector2 position{};
    bool allowed{};
};


struct GenerateSeqInput {
    Vector2 current;
    Vector2 target;
    bool order;
    Vector2 bad_position;

    bool operator==(const GenerateSeqInput &other) const {
        return current == other.current && target == other.target && order == other.order &&
               bad_position == other.bad_position;
    }
};

// Custom hash function for GenerateSeqInput
struct InputHash {
    std::size_t operator()(const GenerateSeqInput &input) const {
        const auto h1 = std::hash<int>{}(input.current.x);
        const auto h2 = std::hash<int>{}(input.current.y);
        const auto h3 = std::hash<int>{}(input.target.x);
        const auto h4 = std::hash<int>{}(input.target.y);
        const auto h5 = std::hash<bool>{}(input.order);
        const auto h6 = std::hash<int>{}(input.bad_position.x);
        const auto h7 = std::hash<int>{}(input.bad_position.y);

        // Combine hash values using bit manipulation
        return h1 ^ h2 ^ h3 ^ h4 ^ h5 ^ h6 ^ h7 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
    }
};

typedef std::unordered_map<GenerateSeqInput, Movement, InputHash> GenerateSeqMemo;


void move_up_down(std::vector<char> &sequence, const Vector2 diff) {
    if (diff.x < 0) {
        for (int i = 0; i < std::abs(diff.x); i++) {
            sequence.push_back('^');
        }
    }

    if (diff.x > 0) {
        for (int i = 0; i < std::abs(diff.x); i++) {
            sequence.push_back('v');
        }
    }
}


void move_sideways(std::vector<char> &sequence, const Vector2 diff) {
    if (diff.y < 0) {
        for (int i = 0; i < std::abs(diff.y); i++) {
            sequence.push_back('<');
        }
    }

    if (diff.y > 0) {
        for (int i = 0; i < std::abs(diff.y); i++) {
            sequence.push_back('>');
        }
    }
}


bool verify_position(const Vector2 &position, const Vector2 &bad_position, const std::vector<char> &sequence) {
    auto current = position;
    for (auto &c: sequence) {
        switch (c) {
            case '^':
                current.x--;
                break;
            case 'v':
                current.x++;
                break;
            case '<':
                current.y--;
                break;
            case '>':
                current.y++;
                break;
            default:
                break;
        }
        if (current == bad_position) {
            return false;
        }
    }
    return true;
}


Movement generate_sequence(const Vector2 current, const Vector2 target, bool order, const Vector2 bad_position,
                           GenerateSeqMemo &memo) {
    GenerateSeqInput input = {current, target, order, bad_position};
    if (memo.contains(input)) {
        return memo[input];
    }

    std::vector<char> sequence;

    auto diff = target - current;

    if (order) {
        move_up_down(sequence, diff);
        move_sideways(sequence, diff);
    } else {
        move_sideways(sequence, diff);
        move_up_down(sequence, diff);
    }

    sequence.push_back('A');

    Movement res = {sequence, target, verify_position(current, bad_position, sequence)};
    memo[input] = res;
    return res;
}


struct RobotInput {
    Vector2 position{}, target{};
    int robot_nr = 0;

    bool operator==(const RobotInput &other) const {
        return position == other.position && target == other.target && robot_nr == other.robot_nr;
    }
};

struct RobotOutput {
    uint64_t length = 0;
    Vector2 position{};

    bool operator==(const RobotOutput &other) const {
        return length == other.length && position == other.position;
    }
};

struct RobotHash {
    std::size_t operator()(const RobotInput &input) const {
        auto h1 = std::hash<Vector2>{}(input.position);
        auto h2 = std::hash<Vector2>{}(input.target);
        auto h3 = std::hash<int>{}(input.robot_nr);

        return h1 ^ h2 ^ h3 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
    }
};

typedef std::unordered_map<RobotInput, RobotOutput, RobotHash> RobotMemo;


uint64_t me(const std::vector<char> &sequence, GenerateSeqMemo &memo) {
    Vector2 current_pos = directional_keypad['A'];
    uint64_t length = 0;
    for (auto &c: sequence) {
        Movement res;
        res = generate_sequence(current_pos, directional_keypad[c], true, {0, 0}, memo);

        if (!res.allowed) {
            res = generate_sequence(current_pos, directional_keypad[c], false, {0, 0}, memo);
        }

        current_pos = res.position;
        length += res.sequence.size();
    }

    return length;
}


uint64_t robot(const std::vector<char> &sequence, int robot_nr, GenerateSeqMemo &memo, RobotMemo &robot_memo) {
    std::unordered_map<char, Vector2> keypad = directional_keypad;
    Vector2 bad_position = {0, 0};
    if (robot_nr == 0) {
        keypad = numeric_keypad;
        bad_position = {3, 0};
    } else if (robot_nr == NR_ROBOTS) {
        return me(sequence, memo);
    }

    Vector2 current_pos = keypad['A'];
    uint64_t length = 0;
    for (auto &c: sequence) {
        RobotInput input = {current_pos, keypad[c], robot_nr};
        if (robot_memo.contains(input)) {
            auto res = robot_memo[input];
            current_pos = res.position;
            length += res.length;
            continue;
        }

        uint64_t new_length1 = 0, new_length2 = 0;
        auto res1 = generate_sequence(current_pos, keypad[c], true, bad_position, memo);
        if (res1.allowed) {
            new_length1 = robot(res1.sequence, robot_nr + 1, memo, robot_memo);
        }
        auto res2 = generate_sequence(current_pos, keypad[c], false, bad_position, memo);
        if (res2.allowed) {
            new_length2 = robot(res2.sequence, robot_nr + 1, memo, robot_memo);
        }

        RobotOutput output;
        if (new_length1 != 0 && (new_length1 < new_length2 || new_length2 == 0)) {
            current_pos = res1.position;
            length += new_length1;
            output = {new_length1, current_pos};
        } else {
            current_pos = res2.position;
            length += new_length2;
            output = {new_length2, current_pos};
        }

        robot_memo[input] = output;
    }

    return length;
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day21\input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::split<std::string>(content, "\n");


    for (int n = 25; n < 26; n++) {
        NR_ROBOTS = n;

        uint64_t sum = 0;
        for (auto &code: result) {
            if (code.empty()) {
                continue;
            }
            uint64_t nr = std::stoi(code.substr(0, 3));

            std::vector<char> code_char_v;
            for (auto &c: code) {
                code_char_v.push_back(c);
            }

            GenerateSeqMemo memo{};
            RobotMemo robot_memo{};
            auto sequence_l = robot(code_char_v, 0, memo, robot_memo);
//            std::cout << "Code: " << code << " " << sequence_l << ": " << nr * sequence_l << std::endl;

//        utilities::printRow(sequence);
//        break;

            sum += nr * sequence_l;
        }

        std::cout << n << " sum: " << sum << std::endl; // 211930
    }
    return 0;
}
