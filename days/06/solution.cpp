#include "solution.hpp"

#include <iostream>
#include <map>
#include <numeric>
#include <functional>

#include "common.hpp"

#define DAY "06"

namespace  {

std::vector<std::pair<std::vector<uint64_t>, std::function<uint64_t(uint64_t, uint64_t)>>> parse_columns_and_ops(
    const std::vector<std::string>& lines) {
    if (lines.empty())
        return {};

    std::vector<std::string> op_tokens = aoc::split2(lines.back());
    size_t num_cols                    = op_tokens.size();

    std::vector<std::vector<uint64_t>> matrix;
    for (size_t i = 0; i < lines.size() - 1; ++i) {
        auto tokens = aoc::split2(lines[i]);
        std::vector<uint64_t> row;
        row.reserve(num_cols);
        for (const auto& tok : tokens) {
            row.push_back(std::stoi(tok));
        }
        matrix.push_back(row);
    }

    std::vector<std::vector<uint64_t>> columns(num_cols);
    for (size_t r = 0; r < matrix.size(); ++r)
        for (size_t c = 0; c < num_cols; ++c)
            if (c < matrix[r].size()) 
                columns[c].push_back(matrix[r][c]);

    std::vector<std::function<uint64_t(uint64_t, uint64_t)>> ops;
    for (const auto& op_str : op_tokens) {
        if (op_str == "+")
            ops.push_back(std::plus<uint64_t>{});
        else if (op_str == "*") 
            ops.push_back(std::multiplies<uint64_t>{});
        else 
            throw std::runtime_error("Unknown operation: " + op_str);
    }

    std::vector<std::pair<std::vector<uint64_t>, std::function<uint64_t(uint64_t, uint64_t)>>> result;
    for (size_t i = 0; i < num_cols; ++i)
        result.emplace_back(std::move(columns[i]), ops[i]);

    return result;
}

std::vector<std::string> transpose(const std::vector<std::string>& matrix) {
    if (matrix.empty())
        return {};

    size_t rows = matrix.size();
    size_t cols = matrix[0].size();

    for (const auto& row : matrix) {
        if (row.size() != cols) {
            throw std::invalid_argument("All strings must have the same length");
        }
    }

    std::vector<std::string> transposed(cols, std::string(rows, ' '));

    for (size_t i = 0; i < rows; ++i) 
        for (size_t j = 0; j < cols; ++j) 
            transposed[j][i] = matrix[i][j];

    return transposed;
}

std::vector<std::pair<std::vector<uint64_t>, std::function<uint64_t(uint64_t, uint64_t)>>> parse_columns_and_ops2(
    const std::vector<std::string>& lines) {
    std::vector<std::pair<std::vector<uint64_t>, std::function<uint64_t(uint64_t, uint64_t)>>> result;

    std::vector<std::string> op_tokens = aoc::split2(lines.back());
    size_t num_cols                    = op_tokens.size();
    std::vector<std::function<uint64_t(uint64_t, uint64_t)>> ops;
    for (const auto& op_str : op_tokens) {
        if (op_str == "+")
            ops.push_back(std::plus<uint64_t>{});
        else if (op_str == "*")
            ops.push_back(std::multiplies<uint64_t>{});
        else
            throw std::runtime_error("Unknown operation: " + op_str);
    }

    auto new_lines  = transpose({lines.begin(), lines.end() - 1});
    std::vector<uint64_t> nums;
    int cur_op_i = 0;
    for (int i = 0; i < new_lines.size(); ++i) {
        std::string str = aoc::trim(new_lines[i]);
        if (!str.empty()) {
            nums.push_back(aoc::to_num<uint64_t>(str));
        } else {
            result.push_back({nums, ops[cur_op_i++]});
            nums.clear();
        }
    }

    result.push_back({nums, ops[cur_op_i++]});

    return result;
}

uint64_t part1() {
    auto lines = aoc::read_lines(aoc::get_input_path(DAY));

    auto cols = parse_columns_and_ops(lines);
    uint64_t ans = 0;
    for (const auto& col : cols) 
        ans += std::accumulate(col.first.begin() + 1, col.first.end(), *col.first.begin(), col.second);

    std::cout << "  Part 1: " << ans << std::endl;
    return ans;
}

uint64_t part2() {
    auto lines   = aoc::read_lines(aoc::get_input_path(DAY));
    auto cols    = parse_columns_and_ops2(lines);
    uint64_t ans = 0;
    for (const auto& col : cols)
        ans += std::accumulate(col.first.begin() + 1, col.first.end(), *col.first.begin(), col.second);

    std::cout << "  Part 2: " << ans << std::endl;
    return ans;
}

}  // namespace

namespace day06 {

void solve() {
    aoc::write_answers<uint64_t, uint64_t>(DAY, part1(), part2());
}

}  // namespace day00
