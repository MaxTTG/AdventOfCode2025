#include "solution.hpp"

#include <iostream>
#include <set>
#include <map>
#include <iterator>

#include "common.hpp"

#define DAY "07"

namespace {

constexpr char START    = 'S';
constexpr char SPLITTER = '^';

int part1() {
    auto lines = aoc::read_lines(aoc::get_input_path(DAY));
    std::set<int> beams;
    int ans = 0;
    auto start_pos = std::find(lines[0].begin(), lines[0].end(), START);
    beams.insert(start_pos - lines[0].begin());

    for (int i = 0; i < lines.size(); i += 2) {
        auto line = lines[i];
        std::set<int> to_remove;
        std::set<int> to_insert;
        for (const auto& b : beams) {
            if (line[b] == SPLITTER) {
                to_insert.insert(b + 1);
                to_insert.insert(b - 1);
                to_remove.insert(b);
                ++ans;
            }
        }
        for (const auto& r : to_remove) beams.erase(r);
        beams.insert(to_insert.begin(), to_insert.end());
    }

    std::cout << "  Part 1: " << ans << std::endl;
    return ans;
}

uint64_t part2() {
    auto lines = aoc::read_lines(aoc::get_input_path(DAY));

    std::map<int, uint64_t> beams;

    auto start_pos = std::find(lines[0].begin(), lines[0].end(), START);
    int start_col    = start_pos - lines[0].begin();
    beams[start_col] = 1;

    for (int i = 2; i < lines.size(); i += 2) {
        const std::string& line = lines[i];
        std::map<int, uint64_t> next_beams;

        for (const auto& [col, count] : beams) {
            if (col < 0 || col >= (int)line.size()) {
                next_beams[col] += count;
                continue;
            }

            if (line[col] == SPLITTER) {
                next_beams[col - 1] += count;
                next_beams[col + 1] += count;
            } else {
                next_beams[col] += count;
            }
        }

        beams = std::move(next_beams);
    }

    uint64_t ans = 0;
    for (const auto& [col, count] : beams) 
        ans += count;

    std::cout << "  Part 2: " << ans << std::endl;
    return ans;
}

}  // namespace

namespace day07 {

void solve() {
    aoc::write_answers<int, uint64_t>(DAY, part1(), part2());
}

}  // namespace day07
