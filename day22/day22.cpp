#include <iostream>
#include <array>
#include "../utilities.h"


std::unordered_map<uint64_t, uint64_t> cache{};

struct Sequence {
    int a, b, c, d;

    bool operator==(const Sequence &other) const {
        return a == other.a && b == other.b && c == other.c && d == other.d;
    }
};

typedef std::unordered_map<Sequence, unsigned int> SequenceMap;

struct SequenceHash {
    size_t operator()(const Sequence &s) const {
        return std::hash<int>()(s.a) ^ std::hash<int>()(s.b) ^ std::hash<int>()(s.c) ^ std::hash<int>()(s.d);
    }
};

namespace std {
    template<>
    struct hash<Sequence> {
        size_t operator()(const Sequence &s) const {
            return hash<uint64_t>()(s.a) ^ hash<uint64_t>()(s.b) ^ hash<uint64_t>()(s.c) ^ hash<uint64_t>()(s.d);
        }
    };
}

uint64_t mix(uint64_t a, uint64_t b) {
    // bitwise xor
    return a ^ b;
}

uint64_t prune(uint64_t a) {
    return a % 16777216;
}

uint64_t step1(uint64_t a) {
    return prune(mix(a * 64, a));
}

uint64_t step2(uint64_t a) {
    return prune(mix(std::floor(static_cast<double>(a) / 32.0), a));
}

uint64_t step3(uint64_t a) {
    return prune(mix(a * 2048, a));
}

uint64_t next_secret(uint64_t a) {
    if (cache.contains(a)) {
        return cache[a];
    }
    cache[a] = step3(step2(step1(a)));
    return cache[a];
}

uint64_t get_nth_secret(uint64_t a, int n) {
    for (int i = 0; i < n; i++) {
        a = next_secret(a);
    }
    return a;
}

void generate_all(uint64_t start, SequenceMap& visited) {
    uint64_t current, prev;
    current = start;
    std::array<int, 4> values{};

    for (int i = 1; i < 2000; i++) {
        prev = current;
        current = next_secret(current);
        if (i > 3) {
            values[3] = current % 10 - prev % 10;
            Sequence s{values[0], values[1], values[2], values[3]};
            if (!visited.contains(s)) {
                visited[s] = static_cast<unsigned int>(current % 10);
            }
            // shift values
            for (int j = 1; j < 4; j++) {
                values[j - 1] = values[j];
            }
        } else {
            values[i - 1] = current % 10 - prev % 10;
        }
    }
}


int main() {
    // read from input.txt
    std::filesystem::path path = R"(D:\Projects\adventOfCode24\day22\input.txt)";
    std::string content = utilities::readFromFile(path);
    auto result = utilities::split(content, "\n");

    std::vector<SequenceMap> visited{};
    uint64_t sum = 0;
    for (auto &r: result) {
        sum += get_nth_secret(r[0], 2000);
        SequenceMap v{};
        generate_all(r[0], v);
        visited.push_back(v);
    }

    std::cout << sum << std::endl;

    uint64_t max_sum = 0;
    Sequence max_seq{};
    std::unordered_set<Sequence, SequenceHash> tried{};

    for (auto &v: visited) {
        for (auto &s: v) {
            if (tried.contains(s.first)) {
                continue;
            }

            uint64_t current_sum = 0;
            for (auto &v2: visited) {
                if (v2.contains(s.first)) {
                    current_sum += v2[s.first];
                }
            }

            if (current_sum > max_sum) {
                max_sum = current_sum;
                max_seq = s.first;
            }

            tried.insert(s.first);
        }
    }

    std::cout << max_sum << std::endl;
    std::cout << max_seq.a << " " << max_seq.b << " " << max_seq.c << " " << max_seq.d << std::endl;

    return 0;
}
