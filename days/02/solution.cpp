#include "solution.hpp"

#include <iostream>

#include "common.hpp"

#define DAY "02"

namespace {

bool isInvalid1(uint64_t id) {
    std::string strId = std::to_string(id);
    int len           = strId.size();
    if (len % 2) {
        return false;
    }

    int pow = std::pow(10, len / 2);
    return id / pow == id % pow;
}

bool isInvalid2(uint64_t id) {
    std::string strId = std::to_string(id);
    int len           = strId.size();
    for (int i = 1; i < len; ++i) {
        std::string substr = strId.substr(0, i);
        std::string s;
        while (s.size() < strId.size())
            s += substr;
        if (s == strId)
            return true;
    }

    return false;
}

uint64_t part1() {
    auto ids = aoc::split(aoc::read_file(aoc::get_input_path(DAY)), ',');
    std::vector<std::pair<uint64_t, uint64_t>> ranges;
    for (const auto& r : ids) {
        auto a = aoc::split(r, '-');
        ranges.emplace_back(aoc::to_num<uint64_t>(a.front()), aoc::to_num<uint64_t>(a.back()));
    }

    uint64_t ans = 0;
    for (const auto& r : ranges) {
        for (uint64_t i = r.first; i < r.second; ++i) {
            ans += i * isInvalid1(i);
        }
    }

    std::cout << "  Part 1: " << ans << std::endl;
    return ans;
}

uint64_t part2() {
    auto ids = aoc::split(aoc::read_file(aoc::get_input_path(DAY)), ',');
    std::vector<std::pair<uint64_t, uint64_t>> ranges;
    for (const auto& r : ids) {
        auto a = aoc::split(r, '-');
        ranges.emplace_back(aoc::to_num<uint64_t>(a.front()), aoc::to_num<uint64_t>(a.back()));
    }

    uint64_t ans = 0;
    for (const auto& r : ranges) {
        for (uint64_t i = r.first; i < r.second; ++i) {
            ans += i * isInvalid2(i);
        }
    }

    std::cout << "  Part 2: " << ans << std::endl;
    return ans;
}

}  // namespace

namespace day02 {

void solve() {
    aoc::write_answers<uint64_t, uint64_t>(DAY, part1(), part2());
}

}  // namespace day02
