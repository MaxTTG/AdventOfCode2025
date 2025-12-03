#include "solution.hpp"

#include <iostream>
#include <vector>

#include "common.hpp"

#define DAY "03"

namespace {

uint64_t largestKDigitNumber(const std::string& line, int k) {
    if (k <= 0)
        return 0;

    if (static_cast<int>(line.size()) < k)
        throw std::invalid_argument("String too short for k digits");

    int toRemove = static_cast<int>(line.size()) - k;
    std::vector<char> stack;
    stack.reserve(line.size());

    for (char c : line) {
        // stack not empty AND can remove AND cur digit > stack top
        while (!stack.empty() && toRemove > 0 && c > stack.back()) {
            stack.pop_back();
            --toRemove;
        }
        stack.push_back(c);
    }

    stack.resize(k);

    uint64_t result = 0;
    for (char c : stack)
        result = result * 10 + (c - '0');

    return result;
}

int part1() {
    auto lines = aoc::read_lines(aoc::get_input_path(DAY));

    uint64_t ans = 0;
    for (const auto& line : lines)
        ans += largestKDigitNumber(line, 2);

    std::cout << "  Part 1: " << ans << std::endl;
    return ans;
}

uint64_t part2() {
    auto lines = aoc::read_lines(aoc::get_input_path(DAY));

    uint64_t ans = 0;
    for (const auto& line : lines)
        ans += largestKDigitNumber(line, 12);

    std::cout << "  Part 2: " << ans << std::endl;
    return ans;
}

}  // namespace

namespace day03 {

void solve() {
    aoc::write_answers<int, uint64_t>(DAY, part1(), part2());
}

}  // namespace day00
