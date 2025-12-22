#include "solution.hpp"

#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common.hpp"

#define DAY "11"

namespace {

const static std::string START  = "you";
const static std::string START2 = "svr";
const static std::string END    = "out";
using Node                      = std::string;
using Graph                     = std::unordered_map<Node, std::vector<Node>>;
struct PathState {
    std::string currentNode;
    std::unordered_set<Node> visited;
};

const static std::vector<Node> NEED_TO_VISIT = {"fft", "dac"};

bool hasCycle(const Graph& g, const Node& start, const Node& end) {
    std::stack<std::pair<Node, int>> stack;

    std::unordered_set<Node> visited;
    std::unordered_set<Node> path;

    stack.push({start, 0});
    visited.insert(start);
    path.insert(start);

    while (!stack.empty()) {
        auto& [currNode, neighborIndex] = stack.top();

        auto it = g.find(currNode);
        if (it != g.end() && neighborIndex < it->second.size()) {
            const Node& neighbor = it->second[neighborIndex];
            neighborIndex++;

            if (path.count(neighbor))
                return true;

            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                path.insert(neighbor);
                stack.push({neighbor, 0});
            }
        } else {
            path.erase(currNode);
            stack.pop();
        }
    }

    return false;
}

std::unordered_set<Node> computeReachableToTarget(const Graph& graph, const Node& end) {
    std::unordered_map<Node, std::vector<Node>> reverseGraph;
    for (const auto& [node, neighbors] : graph) {
        for (const auto& neighbor : neighbors)
            reverseGraph[neighbor].push_back(node);

        reverseGraph.try_emplace(node, std::vector<Node>{});
    }

    std::unordered_set<Node> reachable;
    std::queue<Node> q;
    q.push(end);
    reachable.insert(end);

    while (!q.empty()) {
        const Node current = q.front();
        q.pop();

        auto it = reverseGraph.find(current);
        if (it != reverseGraph.end()) {
            for (const Node& predecessor : it->second) {
                if (reachable.find(predecessor) == reachable.end()) {
                    reachable.insert(predecessor);
                    q.push(predecessor);
                }
            }
        }
    }

    return reachable;
}

uint64_t countPathsFrontier(const Graph& graph, const Node& start, const Node& end,
                            const std::vector<Node>& needToVisit = {}) {
    auto reachableToTarget = computeReachableToTarget(graph, end);
    if (reachableToTarget.find(start) == reachableToTarget.end())
        return 0;

    int totalPaths = 0;
    std::vector<PathState> currentFrontier;
    currentFrontier.push_back({start, {start}});

    while (!currentFrontier.empty()) {
        std::vector<PathState> nextFrontier;
        {
            std::vector<PathState> localNextFrontier;
            int localPaths = 0;

            for (size_t i = 0; i < currentFrontier.size(); ++i) {
                const auto& state = currentFrontier[i];
                if (state.currentNode == end) {
                    bool allVisited = true;
                    for (const auto& node : needToVisit) {
                        if (state.visited.find(node) == state.visited.end()) {
                            allVisited = false;
                            break;
                        }
                    }
                    if (allVisited) {
                        ++localPaths;
                    }
                    continue;
                }

                auto it = graph.find(state.currentNode);
                if (it != graph.end()) {
                    for (const auto& neighbor : it->second) {
                        if (reachableToTarget.find(neighbor) == reachableToTarget.end())
                            continue;
                        if (state.visited.find(neighbor) == state.visited.end()) {
                            PathState newState{neighbor, state.visited};
                            newState.visited.insert(neighbor);
                            localNextFrontier.push_back(std::move(newState));
                        }
                    }
                }
            }

            {
                totalPaths += localPaths;
                nextFrontier.insert(nextFrontier.end(), std::make_move_iterator(localNextFrontier.begin()),
                                    std::make_move_iterator(localNextFrontier.end()));
            }
        }

        currentFrontier = std::move(nextFrontier);
    }

    return totalPaths;
}

int part1() {
    auto lines = aoc::read_lines(aoc::get_input_path(DAY));
    Graph k2o;
    for (const auto& line : lines) {
        auto s = aoc::split2(line);
        auto k = s[0].substr(0, s[0].size() - 1);
        for (int i = 1; i < s.size(); ++i)
            k2o[k].push_back(s[i]);
    }

    if (hasCycle(k2o, START, END)) {
        std::cout << "CYCLED!" << std::endl;
        return 0;
    }

    auto ans = countPathsFrontier(k2o, START, END);

    std::cout << "  Part 1: " << ans << std::endl;
    return ans;
}

bool isReachable(const Graph& graph, const Node& start, const Node& target) {
    if (start == target)
        return true;

    std::unordered_set<Node> visited;
    std::stack<Node> stack;
    stack.push(start);

    while (!stack.empty()) {
        Node current = stack.top();
        stack.pop();

        if (current == target)
            return true;
        if (visited.count(current))
            continue;
        visited.insert(current);

        auto it = graph.find(current);
        if (it != graph.end()) {
            for (const Node& neighbor : it->second) {
                if (!visited.count(neighbor)) {
                    stack.push(neighbor);
                }
            }
        }
    }
    return false;
}

long long countPathsDAG(const Graph& graph, const Node& source, const Node& target) {
    if (source == target)
        return 1;

    // 1. Getting a subgraph that is reachable from the source
    std::unordered_set<Node> reachableFromSource;
    {
        std::queue<Node> q;
        q.push(source);
        reachableFromSource.insert(source);
        while (!q.empty()) {
            Node u = q.front();
            q.pop();
            auto it = graph.find(u);
            if (it != graph.end()) {
                for (const Node& v : it->second) {
                    if (!reachableFromSource.count(v)) {
                        reachableFromSource.insert(v);
                        q.push(v);
                    }
                }
            }
        }
    }

    if (!reachableFromSource.count(target))
        return 0;

    // 2. Building an inverse graph for reverse reachability from target
    std::unordered_set<Node> canReachTarget;
    {
        std::unordered_map<Node, std::vector<Node>> revGraph;
        for (const auto& [u, neighbors] : graph) {
            if (!reachableFromSource.count(u))
                continue;
            for (const Node& v : neighbors) {
                if (reachableFromSource.count(v)) {
                    revGraph[v].push_back(u);
                }
            }
        }

        std::queue<Node> q;
        q.push(target);
        canReachTarget.insert(target);
        while (!q.empty()) {
            Node v = q.front();
            q.pop();
            auto it = revGraph.find(v);
            if (it != revGraph.end()) {
                for (const Node& u : it->second) {
                    if (!canReachTarget.count(u)) {
                        canReachTarget.insert(u);
                        q.push(u);
                    }
                }
            }
        }
    }

    if (!canReachTarget.count(source))
        return 0;

    // 3. We collect all the vertices lying on the paths from source to target
    std::vector<Node> relevantNodes;
    for (const Node& u : reachableFromSource) {
        if (canReachTarget.count(u)) {
            relevantNodes.push_back(u);
        }
    }

    // 4. Topological sorting of relevant vertices
    std::unordered_map<Node, int> inDegree;
    std::unordered_map<Node, std::vector<Node>> subGraph;

    for (const Node& u : relevantNodes) {
        inDegree[u] = 0;
        subGraph[u] = {};
    }

    for (const Node& u : relevantNodes) {
        auto it = graph.find(u);
        if (it != graph.end()) {
            for (const Node& v : it->second) {
                if (canReachTarget.count(v) && reachableFromSource.count(v)) {
                    subGraph[u].push_back(v);
                    inDegree[v]++;
                }
            }
        }
    }

    std::queue<Node> q;
    std::unordered_map<Node, long long> pathCount;
    for (const Node& u : relevantNodes) {
        if (inDegree[u] == 0) {
            q.push(u);
            pathCount[u] = (u == source) ? 1 : 0;
        }
    }

    while (!q.empty()) {
        Node u = q.front();
        q.pop();
        for (const Node& v : subGraph[u]) {
            pathCount[v] += pathCount[u];
            if (--inDegree[v] == 0) {
                q.push(v);
            }
        }
    }

    return pathCount.count(target) ? pathCount[target] : 0;
}

std::vector<Node> topologicalSortMandatory(const Graph& graph, std::vector<Node> nodes) {
    // a < b <===> from a we can reach b
    std::sort(nodes.begin(), nodes.end());
    nodes.erase(std::unique(nodes.begin(), nodes.end()), nodes.end());

    std::sort(nodes.begin(), nodes.end(), [&graph](const Node& a, const Node& b) {
        if (a == b)
            return false;

        std::unordered_set<Node> visited;
        std::queue<Node> q;
        q.push(a);
        visited.insert(a);

        while (!q.empty()) {
            Node u = q.front();
            q.pop();
            auto it = graph.find(u);
            if (it != graph.end()) {
                for (const Node& v : it->second) {
                    if (v == b)
                        return true;
                    if (!visited.count(v)) {
                        visited.insert(v);
                        q.push(v);
                    }
                }
            }
        }

        visited.clear();
        q.push(b);
        visited.insert(b);
        while (!q.empty()) {
            Node u = q.front();
            q.pop();
            auto it = graph.find(u);
            if (it != graph.end()) {
                for (const Node& v : it->second) {
                    if (v == a)
                        return false;
                    if (!visited.count(v)) {
                        visited.insert(v);
                        q.push(v);
                    }
                }
            }
        }

        return false;
    });

    return nodes;
}

uint64_t countPathsViaMandatory(const Graph& graph, const Node& start, const Node& end, std::vector<Node> needToVisit) {
    needToVisit = topologicalSortMandatory(graph, needToVisit);

    std::vector<Node> waypoints;
    waypoints.reserve(needToVisit.size() + 2);
    waypoints.push_back(start);
    for (const Node& node : needToVisit) {
        waypoints.push_back(node);
    }
    waypoints.push_back(end);

    uint64_t total = 1;
    for (size_t i = 0; i + 1 < waypoints.size(); ++i) {
        uint64_t paths = countPathsDAG(graph, waypoints[i], waypoints[i + 1]);
        if (paths == 0) {
            return 0;
        }
        total *= paths;
    }

    return total;
}

uint64_t part2() {
    auto lines = aoc::read_lines(aoc::get_input_path(DAY));
    Graph k2o;
    for (const auto& line : lines) {
        auto s = aoc::split2(line);
        auto k = s[0].substr(0, s[0].size() - 1);
        for (int i = 1; i < s.size(); ++i)
            k2o[k].push_back(s[i]);
    }

    if (hasCycle(k2o, START2, END)) {
        std::cout << "CYCLED!" << std::endl;
        return 0;
    }

    // auto a1 = countPathsFrontier(k2o, START2, "fft");
    // std::cout << "a1: " << a1 << std::endl;
    // auto a2 = countPathsFrontier(k2o, "fft", "dac"); // endless calculating, a2 = 11222908ULL;
    // std::cout << "a2: " << a2 << std::endl;
    // auto a3 = countPathsFrontier(k2o, "dac", "out");
    // std::cout << "a3: " << a3 << std::endl;
    // auto ans = a2 * a1 * a3;

    auto ans = countPathsViaMandatory(k2o, START2, END, NEED_TO_VISIT);

    std::cout << "  Part 2: " << ans << std::endl;
    return ans;
}

}  // namespace

namespace day11 {

void solve() {
    aoc::write_answers<uint64_t, uint64_t>(DAY, part1(), part2());
}

}  // namespace day11
