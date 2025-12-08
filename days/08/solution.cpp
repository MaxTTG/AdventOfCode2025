#include "solution.hpp"

#include <iostream>
#include <unordered_map>
#include <vector>

#include "common.hpp"

#define DAY "08"

namespace {

struct Box {
    int x, y, z;
};

double distance(const Box& a, const Box& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double dz = a.z - b.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

// Disjoint Set Union
struct DSU {
    std::vector<int> parent;
    std::vector<int> size;

    DSU(int n) {
        parent.resize(n);
        size.resize(n, 1);
        for (int i = 0; i < n; ++i)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y)
            return;
        if (size[x] < size[y])
            std::swap(x, y);
        parent[y] = x;
        size[x] += size[y];
    }

    std::vector<int> get_component_sizes() {
        std::unordered_map<int, int> root_count;
        for (int i = 0; i < parent.size(); ++i)
            root_count[find(i)] = size[find(i)];
        std::vector<int> sizes;
        for (auto& kv : root_count)
            sizes.push_back(kv.second);
        std::sort(sizes.rbegin(), sizes.rend());
        return sizes;
    }
};

struct Edge {
    int i, j;
    double dist;
    bool operator<(const Edge& other) const { return dist < other.dist; }
};

uint64_t part1() {
    auto lines = aoc::read_lines(aoc::get_input_path(DAY));
    std::vector<Box> boxes;
    boxes.reserve(lines.size());
    for (const auto& line : lines) {
        if (line.empty())
            continue;
        auto coords = aoc::split(line, ',');
        boxes.push_back({aoc::to_num<int>(coords[0]), aoc::to_num<int>(coords[1]), aoc::to_num<int>(coords[2])});
    }

    int n = boxes.size();
    std::vector<Edge> edges;
    edges.reserve(static_cast<size_t>(n) * (n - 1) / 2);

    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            edges.push_back({i, j, distance(boxes[i], boxes[j])});

    std::sort(edges.begin(), edges.end());

    DSU dsu(n);

    int connections = 0;
    size_t index    = 0;
    while (connections < n && index < edges.size()) {
        auto& e = edges[index];
        if (dsu.find(e.i) != dsu.find(e.j))
            dsu.unite(e.i, e.j);
        connections++;
        index++;
    }

    auto sizes = dsu.get_component_sizes();

    uint64_t result = 1;
    for (int i = 0; i < 3; ++i)
        result *= (i < sizes.size() ? result *= sizes[i] : 1);

    std::cout << "  Part 1: " << result << std::endl;
    return result;
}

uint64_t part2() {
    auto lines = aoc::read_lines(aoc::get_input_path(DAY));
    std::vector<Box> boxes;
    for (const auto& line : lines) {
        if (line.empty())
            continue;
        auto coords = aoc::split(line, ',');
        boxes.push_back({aoc::to_num<int>(coords[0]), aoc::to_num<int>(coords[1]), aoc::to_num<int>(coords[2])});
    }

    int n = boxes.size();

    std::vector<Edge> edges;
    edges.reserve(static_cast<size_t>(n) * (n - 1) / 2);

    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            edges.push_back({i, j, distance(boxes[i], boxes[j])});

    std::sort(edges.begin(), edges.end());

    DSU dsu(n);
    int unions_done = 0;
    Edge last_edge{};

    for (const auto& e : edges) {
        if (dsu.find(e.i) != dsu.find(e.j)) {
            dsu.unite(e.i, e.j);
            unions_done++;
            last_edge = e;
            if (unions_done == n - 1)
                break;
        }
    }

    uint64_t result = static_cast<uint64_t>(boxes[last_edge.i].x) * static_cast<uint64_t>(boxes[last_edge.j].x);

    std::cout << "  Part 2: " << result << std::endl;
    return result;
}

}  // namespace

namespace day08 {

void solve() {
    aoc::write_answers<uint64_t, uint64_t>(DAY, part1(), part2());
}

}  // namespace day08
