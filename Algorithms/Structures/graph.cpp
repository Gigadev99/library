#pragma once
#include <vector>
using std::vector;

struct Directed{};
struct Undirected{};

template<typename T, typename EdgeType = Undirected>
struct GraphType {
    int V, E;
    void GraphType(this auto&& self);
    void add_edge(int u, int v);
};

template<typename T, typename EdgeType>
struct Graph : GraphType<Graph<T>, T, EdgeType> {
    vector<int> to, head, next;
    void add_edge(int u, int v) {
        to[u] = v;
        next[next[-1]] = u; 
    }
};