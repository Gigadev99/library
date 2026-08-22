#pragma once
#include <vector>
#include <cassert>
#include <array> 
#include "Structures/vector.cpp"
#include "utils.cpp"
using std::vector, std::array, std::plus, std::minus, std::multiplies;
template <class T> concept VectorLike = contiguous_range<T>;

auto dot(const VectorLike auto& a, const VectorLike auto& b) {
    auto sum = a[0] * b[0];
    for (int i = 0; i < size(a); i++) sum += a[i] * b[i];
    return sum;
}
template<class T> auto dot(const Vec2<T>& a, const Vec2<T>& b) { return a.x * b.x + a.y * b.y; }
template<class T> auto dot(const Vec3<T>& a, const Vec3<T>& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
template<class T> auto dot(const Vec4<T>& a, const Vec4<T>& b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.t * b.t; }

template<class T> 
Vec3<T> cross(const Vec3<T>& a, const Vec3<T>& b) { 
    return {a.y * b.z - a.z * b.y, 
            a.z * b.x - a.x * b.z, 
            a.x * b.y - a.y * b.x}; 
}
template<class T> Vec3<T> cross(const Vec2<T>& a, const Vec2<T>& b) { return {0, 0, a.x * b.y - a.y * b.x}; }


void elem_wise(VectorLike auto& a, const VectorLike auto& b, auto f) { assert(size(a) == size(b));
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
array<T, N> operator+(const array<T, N>& a, const VectorLike auto& b) { auto result = a; result += b; return result; }
template<typename T, size_t N>
array<T, N> operator-(const array<T, N>& a, const VectorLike auto& b) { auto result = a; result -= b; return result; }
template<typename T, size_t N>
array<T, N> operator*(const array<T, N>& a, const VectorLike auto& b) { auto result = a; result *= b; return result; }

/*
int main() {
    array<int, 3> a = {1, 2, 3};
    vector<int> b = {4, 5, 6};
    auto added = a + b;
    auto result = dot(a, b);
}
*/