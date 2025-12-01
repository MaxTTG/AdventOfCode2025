#include "solution.hpp"

#include <iostream>

#include "common.hpp"

#define DAY "01"

namespace {

int part1() {
    auto lines = aoc::read_lines(aoc::get_input_path(DAY));

    int pass = 0;
    int pos  = 50;
    for (const auto &line : lines) {
        if (line[0] == 'R') {
            pos += aoc::to_num<int>(line.substr(1));
        } else {  // 'L'
            pos -= aoc::to_num<int>(line.substr(1)) + 100;
        }

        pos %= 100;
        pass += pos == 0;
    }

    std::cout << "  Part 1: " << pass << std::endl;
    return pass;
}

int part2() {
    auto lines = aoc::read_lines(aoc::get_input_path(DAY));

    int pass = 0;
    int pos  = 50;

    for (const auto &line : lines) {
        int d = aoc::to_num<int>(line.substr(1));

        if (line[0] == 'R') {
            int first_hit = (100 - pos) % 100;
            if (first_hit == 0)
                first_hit = 100;

            if (d >= first_hit)
                pass += 1 + (d - first_hit) / 100;

            pos = (pos + d) % 100;

        } else {  // 'L'
            int first_hit = (pos == 0) ? 100 : pos;

            if (d >= first_hit)
                pass += 1 + (d - first_hit) / 100;

            pos = (pos - d) % 100;
            if (pos < 0)
                pos += 100;
        }
    }

    std::cout << "  Part 2: " << pass << std::endl;
    return pass;
}

}  // namespace

namespace day01 {

void solve() {
    aoc::write_answers(DAY, part1(), part2());
}

}  // namespace day01
