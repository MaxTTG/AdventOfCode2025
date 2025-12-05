#include "solution.hpp"

#include <algorithm>
#include <iostream>
#include <set>

#include "common.hpp"

#define DAY "05"

namespace {

struct IntervalSet {
private:
    struct Interval {
        int64_t l, r;
        bool operator<(const Interval& other) const { return l < other.l; }
    };

    std::set<Interval> intervals;

public:
    void add(int64_t l, int64_t r) {
        if (l > r)
            return;

        auto it = intervals.lower_bound(Interval{l, l});
        if (it != intervals.begin()) {
            --it;
            if (it->r < l - 1)
                ++it;
        }

        int64_t newL = l, newR = r;

        while (it != intervals.end() && it->l <= r + 1) {
            if (it->r < l - 1) {
                ++it;
                continue;
            }
            newL = std::min(newL, it->l);
            newR = std::max(newR, it->r);
            it   = intervals.erase(it);
        }

        intervals.insert(Interval{newL, newR});
    }

    bool contains(int64_t x) const {
        if (intervals.empty())
            return false;

        auto it = intervals.upper_bound(Interval{x, x});
        if (it == intervals.begin())
            return false;
        --it;

        return (it->l <= x) && (x <= it->r);
    }

    int64_t totalSize() const {
        int64_t size = 0;
        for (const auto& seg : intervals)
            size += seg.r - seg.l + 1;
        return size;
    }

    void print() const {
        for (const auto& seg : intervals) {
            std::cout << "[" << seg.l << ", " << seg.r << "] ";
        }
        std::cout << std::endl;
    }
};

int part1() {
    auto lines = aoc::read_lines(aoc::get_input_path(DAY));
    auto it    = std::find(lines.begin(), lines.end(), "");
    std::vector<std::string> good_str(lines.begin(), it);
    std::vector<std::string> test_str(it + 1, lines.end());
    std::vector<std::pair<int64_t, int64_t>> good;
    std::vector<int64_t> test;
    good.reserve(good_str.size());
    test.reserve(test_str.size());
    for (auto& str : good_str) {
        auto bounds = aoc::split(str, '-');
        auto bl     = bounds.front();
        auto br     = bounds.back();
        good.emplace_back(aoc::to_num<int64_t>(bl), aoc::to_num<int64_t>(br));
    }

    for (auto& str : test_str)
        test.emplace_back(aoc::to_num<int64_t>(str));

    IntervalSet intervalSet;
    for (const auto& g : good)
        intervalSet.add(g.first, g.second);

    int ans = 0;
    for (const auto& t : test)
        ans += intervalSet.contains(t);

    std::cout << "  Part 1: " << ans << std::endl;
    return ans;
}

int64_t part2() {
    auto lines = aoc::read_lines(aoc::get_input_path(DAY));
    auto it    = std::find(lines.begin(), lines.end(), "");
    std::vector<std::string> good_str(lines.begin(), it);
    std::vector<std::pair<int64_t, int64_t>> good;
    good.reserve(good_str.size());
    for (auto& str : good_str) {
        auto bounds = aoc::split(str, '-');
        auto bl     = bounds.front();
        auto br     = bounds.back();
        good.emplace_back(aoc::to_num<int64_t>(bl), aoc::to_num<int64_t>(br));
    }

    IntervalSet intervalSet;
    for (const auto& g : good)
        intervalSet.add(g.first, g.second);

    auto ans = intervalSet.totalSize();
    std::cout << "  Part 2: " << ans << std::endl;
    return ans;
}

}  // namespace

namespace day05 {

void solve() {
    aoc::write_answers(DAY, part1(), part2());
}

}  // namespace day05
