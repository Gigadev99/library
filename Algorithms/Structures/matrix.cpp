#pragma once
#include <vector> 
using std::vector;


template<typename T>
struct Matrix {
    int r, c;
    vector<T> v; 
    Matrix(int n, int m) : r(n), c(m), v(n * m) {}
    Matrix(const auto& nums, int n, int m) : r(n), c(m), v(begin(nums), end(nums)) {}
    
    T& operator[](int i, int j) { return v[i * c + j]; }
    const T& operator[](int i, int j) const { return v[i * c + j]; }
    T* data() { return v.data(); }
    int rows() const { return r; }
    int cols() const { return c; }
};

template<typename T>
struct MatrixView {
    T* ptr;
    int r, c, stride; 
    // Constructor for a view of a matrix
    MatrixView(T* d, int n, int m) : ptr(d), r(n), c(m), stride(m) {}
    // Constructor for a view of a submatrix
    MatrixView(T* d, int n, int m, int s) : ptr(d), r(n), c(m), stride(s) {}

    T& operator[](int i, int j) { return ptr[i * stride + j]; }
    const T& operator[](int i, int j) const { return ptr[i * stride + j]; }
    T* data() { return ptr; }
    int rows() const { return r; }
    int cols() const { return c; }
};
 