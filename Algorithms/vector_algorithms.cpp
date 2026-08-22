#pragma once
#include <vector>
#include <cassert>
#include <array>
#include <ranges>
#include <concepts> 
using std::vector, std::array, std::plus, std::minus, std::multiplies;
template <class T> concept VectorLike = requires(T a) { a[0]; a.size(); }; 

void elem_wise(VectorLike auto& a, const VectorLike auto& b, auto f) { 
    for (int i = 0; i < size(a); i++) a[i] = f(a[i], b[i]);
}

void operator+=(VectorLike auto& a, const VectorLike auto& b) { elem_wise(a, b, plus{}); }
void operator-=(VectorLike auto& a, const VectorLike auto& b) { elem_wise(a, b, minus{}); }
void operator*=(VectorLike auto& a, const VectorLike auto& b) { elem_wise(a, b, multiplies{}); } 

auto operator+(const VectorLike auto& a, const VectorLike auto& b) { 
    vector result(a.begin(), a.end()); 
    result += b; 
    return result; 
}
auto operator-(const VectorLike auto& a, const VectorLike auto& b) { 
    vector result(a.begin(), a.end()); 
    result -= b; 
    return result; 
}
auto operator*(const VectorLike auto& a, const VectorLike auto& b) { 
    vector result(a.begin(), a.end()); 
    result *= b; 
    return result; 
}
template<typename T, size_t N>
array<T, N> operator+(const array<T, N>& a, const array<T, N>& b) { auto result = a; result += b; return result; }
template<typename T, size_t N>
array<T, N> operator-(const array<T, N>& a, const array<T, N>& b) { auto result = a; result -= b; return result; }
template<typename T, size_t N>
array<T, N> operator*(const array<T, N>& a, const array<T, N>& b) { auto result = a; result *= b; return result; }

auto dot(const VectorLike auto& a, const VectorLike auto& b) { assert(size(a) == size(b) && size(a) > 0);
    auto sum = a[0] * b[0];
    for (int i = 1; i < size(a); i++) sum += a[i] * b[i];
    return sum;
}

int main() {
    array<int, 3> a = {1, 2, 3};
    array<int, 3> b = {4, 5, 6};
    auto result = dot(a, b);
    auto added = a + b;
}