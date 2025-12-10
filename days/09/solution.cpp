#include "solution.hpp"

#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "common.hpp"

#define DAY "09"

namespace {

struct Tile {
    int x, y;
    bool operator==(const Tile& other) const { return x == other.x && y == other.y; }
    bool operator<(const Tile& other) const { return x == other.x ? y < other.y : x < other.x; }
};

struct TileHash {
    std::size_t operator()(const Tile& t) const {
        return static_cast<std::size_t>(t.x) * 73856093u ^ static_cast<std::size_t>(t.y) * 19349663u;
    }
};

int part1() {
    auto lines = aoc::read_lines(aoc::get_input_path(DAY));
    std::vector<Tile> tiles;
    for (const auto& line : lines) {
        if (line.empty())
            continue;
        auto coords = aoc::split(line, ',');
        tiles.push_back({aoc::to_num<int>(coords[0]), aoc::to_num<int>(coords[1])});
    }

    int64_t maxSize = 0;
    for (int i = 0; i < tiles.size(); ++i) {
        for (int j = i + 1; j < tiles.size(); ++j) {
            int64_t newSquare = (1ULL + std::abs(tiles[i].x - tiles[j].x)) * (1ULL + std::abs(tiles[i].y - tiles[j].y));
            maxSize           = std::max(maxSize, newSquare);
        }
    }

    std::cout << "  Part 1: " << maxSize << std::endl;
    return maxSize;
}

int64_t part2() {
    auto lines = aoc::read_lines(aoc::get_input_path(DAY));
    std::vector<Tile> poly;
    for (const auto& line : lines) {
        if (line.empty())
            continue;
        auto parts = aoc::split(line, ',');
        poly.push_back({aoc::to_num<int>(parts[0]), aoc::to_num<int>(parts[1])});
    }

    std::set<int> xSet, ySet;
    for (const auto& p : poly) {
        xSet.insert(p.x);
        ySet.insert(p.y);
    }

    std::vector<int> xAxes(xSet.begin(), xSet.end());
    std::vector<int> yAxes(ySet.begin(), ySet.end());
    std::sort(xAxes.begin(), xAxes.end());
    std::sort(yAxes.begin(), yAxes.end());

    int gridH = static_cast<int>(yAxes.size()) * 2 + 1;
    int gridW = static_cast<int>(xAxes.size()) * 2 + 1;
    std::vector<std::vector<bool>> sqashedGrid(gridH, std::vector<bool>(gridW, false));

    std::unordered_map<Tile, Tile, TileHash> coordToGridPos;

    int n = static_cast<int>(poly.size());
    for (int i = 0; i < n; ++i) {
        Tile p1 = poly[i];
        Tile p2 = poly[(i + 1) % n];

        int idx1_x = static_cast<int>(std::distance(xAxes.begin(), std::find(xAxes.begin(), xAxes.end(), p1.x)));
        int idx1_y = static_cast<int>(std::distance(yAxes.begin(), std::find(yAxes.begin(), yAxes.end(), p1.y)));
        int idx2_x = static_cast<int>(std::distance(xAxes.begin(), std::find(xAxes.begin(), xAxes.end(), p2.x)));
        int idx2_y = static_cast<int>(std::distance(yAxes.begin(), std::find(yAxes.begin(), yAxes.end(), p2.y)));

        Tile gridP1{idx1_x * 2 + 1, idx1_y * 2 + 1};
        Tile gridP2{idx2_x * 2 + 1, idx2_y * 2 + 1};

        coordToGridPos[p1] = gridP1;

        int xStart = std::min(gridP1.x, gridP2.x);
        int xEnd   = std::max(gridP1.x, gridP2.x);
        int yStart = std::min(gridP1.y, gridP2.y);
        int yEnd   = std::max(gridP1.y, gridP2.y);

        for (int y = yStart; y <= yEnd; ++y)
            for (int x = xStart; x <= xEnd; ++x)
                sqashedGrid[y][x] = true;
    }

    std::unordered_set<Tile, TileHash> externalPoints;
    std::queue<Tile> q;
    q.push({0, 0});

    const std::vector<std::pair<int, int>> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    while (!q.empty()) {
        Tile cur = q.front();
        q.pop();

        if (externalPoints.count(cur))
            continue;
        externalPoints.insert(cur);

        for (auto [dx, dy] : dirs) {
            Tile next{cur.x + dx, cur.y + dy};
            if (next.x < 0 || next.x >= gridW || next.y < 0 || next.y >= gridH)
                continue;
            if (sqashedGrid[next.y][next.x])
                continue;
            if (externalPoints.count(next))
                continue;
            q.push(next);
        }
    }

    std::vector<std::vector<std::pair<int, int>>> externalRangesPerRow(gridH);
    for (int y = 0; y < gridH; ++y) {
        int x = 0;
        while (x < gridW) {
            if (!externalPoints.count({x, y})) {
                ++x;
                continue;
            }
            int begin = x;
            while (x < gridW && externalPoints.count({x, y}))
                ++x;
            externalRangesPerRow[y].emplace_back(begin, x - 1);
        }
    }

    std::vector<std::vector<std::pair<int, int>>> externalRangesPerCol(gridW);
    for (int x = 0; x < gridW; ++x) {
        int y = 0;
        while (y < gridH) {
            if (!externalPoints.count({x, y})) {
                ++y;
                continue;
            }
            int begin = y;
            while (y < gridH && externalPoints.count({x, y}))
                ++y;
            externalRangesPerCol[x].emplace_back(begin, y - 1);
        }
    }

    int64_t maxArea = 0;

    for (const auto& c1 : poly) {
        for (const auto& c2 : poly) {
            if (coordToGridPos.find(c1) == coordToGridPos.end() || coordToGridPos.find(c2) == coordToGridPos.end()) {
                continue;
            }

            Tile g1 = coordToGridPos[c1];
            Tile g2 = coordToGridPos[c2];

            int minX = std::min(g1.x, g2.x);
            int maxX = std::max(g1.x, g2.x);
            int minY = std::min(g1.y, g2.y);
            int maxY = std::max(g1.y, g2.y);

            bool touchesExternal = false;

            for (const auto& [rStart, rEnd] : externalRangesPerRow[minY]) {
                if (minX <= rEnd && rStart <= maxX) {
                    touchesExternal = true;
                    break;
                }
            }

            if (!touchesExternal) {
                for (const auto& [rStart, rEnd] : externalRangesPerRow[maxY]) {
                    if (minX <= rEnd && rStart <= maxX) {
                        touchesExternal = true;
                        break;
                    }
                }
            }

            if (!touchesExternal) {
                for (const auto& [cStart, cEnd] : externalRangesPerCol[minX]) {
                    if (minY <= cEnd && cStart <= maxY) {
                        touchesExternal = true;
                        break;
                    }
                }
            }

            if (!touchesExternal) {
                for (const auto& [cStart, cEnd] : externalRangesPerCol[maxX]) {
                    if (minY <= cEnd && cStart <= maxY) {
                        touchesExternal = true;
                        break;
                    }
                }
            }

            if (touchesExternal)
                continue;
            int64_t area =
                (static_cast<int64_t>(std::abs(c2.x - c1.x)) + 1) * (static_cast<int64_t>(std::abs(c2.y - c1.y)) + 1);
            if (area > maxArea)
                maxArea = area;
        }
    }

    std::cout << "  Part 2: " << maxArea << std::endl;
    return maxArea;
}

}  // namespace

namespace day09 {

void solve() {
    aoc::write_answers(DAY, part1(), part2());
}

}  // namespace day09
