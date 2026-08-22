#pragma once
// Just general utilities that I use often
#include <concepts>
#include <ranges>
#include <vector> 
#include <span>
#include <bit>
#include <string> 
#include <functional>
#include <type_traits>
namespace ranges = std::ranges;
using std::vector, std::string, std::integral, std::floating_point, std::swap, std::span, 
      std::countr_zero, std::countl_zero, std::countr_one, std::countl_one, std::popcount, std::has_single_bit, std::bit_ceil, std::bit_floor, std::bit_width,
      std::plus, std::minus, std::multiplies, std::divides, std::modulus, 
      std::is_same_v, std::remove_cvref_t, ranges::contiguous_range, ranges::range_value_t,
      std::is_lvalue_reference_v, std::is_rvalue_reference_v;

template<typename T> using init_list = std::initializer_list<T>;
template<typename T> concept Number = integral<T> || floating_point<T>;
struct none{};
constexpr int dynamic = -1; 
template<typename T> concept Type2D = requires(T t) { t[0][0];  };
// Convert a 2d array or vector, or similar nested type
// to a 2D interface supporting [i,j] and rows(), cols()
template<Type2D G>
struct Gridify {
    G& grid;
    auto& operator[](int i) { return grid[i]; }
    auto& operator[](int i, int j) { return grid[i][j]; }
    Gridify(G& g) : grid(g) {}
    int rows() const { return size(grid); }
    int cols() const { return size(grid[0]); }
    using value_type = remove_cvref_t<decltype(grid[0][0])>;
};

/*
#include <iostream>
#include <print>
int main() {
    vector<vector<int>> vv = {{1,2,3},{4,5,6},{7,8,9}};
    Gridify g{vv};   // CTAD from the ctor, deduces G = vector<vector<int>>
    std::cout << g[1,1] << std::endl;  // Output: 5
    std::println("{}", vv);
}
*/