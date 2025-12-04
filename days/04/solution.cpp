#include "solution.hpp"

#include <iostream>
#include <queue>

#include "common.hpp"

#define DAY "04"

namespace {

const char EMPTY    = '.';
const char ROLL     = '@';
const int MAX_ROLLS = 4;

int part1() {
    auto lines  = aoc::read_lines(aoc::get_input_path(DAY));
    const int H = lines.size();
    const int W = lines[0].size();

    std::vector<std::vector<int>> neighborCount(H, std::vector<int>(W, 0));

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            if (lines[i][j] != ROLL)
                continue;

            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0)
                        continue;
                    int ni = i + dx, nj = j + dy;
                    if (ni >= 0 && ni < H && nj >= 0 && nj < W)
                        ++neighborCount[ni][nj];
                }
            }
        }
    }

    int ans = 0;
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            ans += lines[i][j] == ROLL && neighborCount[i][j] < MAX_ROLLS;

    std::cout << "  Part 1: " << ans << std::endl;
    return ans;
}

int part2() {
    auto lines  = aoc::read_lines(aoc::get_input_path(DAY));
    const int H = lines.size();
    const int W = lines[0].size();

    std::vector<std::vector<char>> grid(H, std::vector<char>(W));
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            grid[i][j] = lines[i][j];

    auto countNeighbors = [&](int x, int y) {
        int cnt = 0;
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0)
                    continue;
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < H && ny >= 0 && ny < W && grid[nx][ny] == ROLL)
                    ++cnt;
            }
        }
        return cnt;
    };

    std::queue<std::pair<int, int>> q;
    std::vector<std::vector<bool>> inQueue(H, std::vector<bool>(W, false));
    std::vector<std::vector<int>> neighborCount(H, std::vector<int>(W));

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            if (grid[i][j] == ROLL) {
                neighborCount[i][j] = countNeighbors(i, j);
                if (neighborCount[i][j] < MAX_ROLLS) {
                    q.emplace(i, j);
                    inQueue[i][j] = true;
                }
            }
        }
    }

    int ans = 0;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        inQueue[x][y] = false;

        if (grid[x][y] != ROLL)
            continue;

        if (neighborCount[x][y] >= MAX_ROLLS)
            continue;

        grid[x][y] = EMPTY;
        ++ans;

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0)
                    continue;
                int nx = x + dx, ny = y + dy;
                if (nx < 0 || nx >= H || ny < 0 || ny >= W || grid[nx][ny] != ROLL)
                    continue;

                --neighborCount[nx][ny];

                if (neighborCount[nx][ny] < MAX_ROLLS && !inQueue[nx][ny]) {
                    q.emplace(nx, ny);
                    inQueue[nx][ny] = true;
                }
            }
        }
    }

    std::cout << "  Part 2: " << ans << std::endl;
    return ans;
}

}  // namespace

namespace day04 {

void solve() {
    aoc::write_answers(DAY, part1(), part2());
}

}  // namespace day04
