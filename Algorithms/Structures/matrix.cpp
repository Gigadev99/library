#pragma once
#include <cassert>
#include <vector>  
#include <array> 
#include <span>
#include "../utils.cpp" 
template<typename M> concept MatType = requires(M A) { A.rows(); A.cols(); A[0, 0]; }; 
template<typename M> concept OwningMatType = MatType<M> && requires(M A) { A.storage; };

template<typename T, int N = dynamic, int M = dynamic>
struct Matrix { 
    int r, c; // rows, cols
    vector<T> storage;
    Matrix() : r(0), c(0) {}
    Matrix(int n, int m, T val = {})  : r(n), c(m),  storage(n * m, val) {}
    Matrix(auto&& nums, int n, int m) : r(n), c(m),  storage(begin(nums),begin(nums)+n*m) { assert(size(nums) >= n*m); }
    void resize(int n, int m) { r = n; c = m;        storage.resize(n * m); } 
    T&       operator[](int i, int j)       { return storage[i * c + j]; }
    const T& operator[](int i, int j) const { return storage[i * c + j]; }
    span<T> operator[](int i)               { return span<T>(&(*this)[i, 0], c); }
    span<const T> operator[](int i)   const { return span<const T>(&(*this)[i, 0], c); }
    int rows() const { return r; } 
    int cols() const { return c; }
    using value_type = T;
};

template<typename T>
struct MatrixView {
    T* ptr; int r, c;  // rows, cols
    T&   operator[](int i, int j) const { return ptr[i * c + j]; } 
    auto operator[](int i) const { return span(&(*this)[i, 0], c); }
    MatrixView() : ptr(nullptr), r(0), c(0) {}
    MatrixView(T* data, int n, int m) : ptr(data), r(n), c(m) {}
    MatrixView(auto&& nums, int n, int m) : ptr(nums.data()), r(n), c(m) { assert(size(nums) >= n*m); }
    MatrixView(OwningMatType auto& A) : ptr(&A[0, 0]), r(A.rows()), c(A.cols()) {}
    template<int N, int M> MatrixView(T (&arr)[N][M]) : ptr(&arr[0][0]), r(N), c(M) {}
    int rows() const { return r; }
    int cols() const { return c; }
    using value_type = T;
};
template<OwningMatType M>
MatrixView(M&) -> MatrixView<typename M::value_type>;

template<typename T, int N, int M> requires (N != dynamic && M != dynamic)
struct Matrix<T, N, M> {
    T storage[N][M]; 
    Matrix() = default; 
    Matrix(T val) { storage.fill(val); }
    Matrix(int n, int m, T val = {}) { assert(n == N && m == M); storage.fill(val); }
    Matrix(T (&arr)[N][M]) : storage{arr} {}
    T&       operator[](int i, int j)       { return storage[i][j]; }
    const T& operator[](int i, int j) const { return storage[i][j]; }
    auto operator[](int i)       { return span(&(*this)[i, 0], M); } 
    auto operator[](int i) const { return span(&(*this)[i, 0], M); }
    int rows() const { return N; }
    int cols() const { return M; }
    using value_type = T;
};

template<MatType M>
struct SubMatrix {
    M& matrix;  int start_i, start_j, r, c; 
    auto& operator[](int i, int j) { return matrix[start_i + i, start_j + j]; }
    int rows() const { return r; }
    int cols() const { return c; }
    using value_type = M::value_type;
};
auto submatrix(MatType auto& matrix, int i, int j, int rows, int cols) { assert(i + rows <= matrix.rows() && j + cols <= matrix.cols());
    return SubMatrix{matrix, i, j, rows, cols}; 
} 


/*
int main() {
    vector<int> v = {1,2,3,4,5,6};
    Matrix<int> M(2,3);
    M.storage = move(v);
    MatrixView m0(M);
    int arr[3][3] = {};
    MatrixView m2(arr);
    vector<vector<int>> vec = {{1,1,1},{2,2,2}};
    for (auto row : vec) for (auto cell : row) std::cout << cell << ' ';
}*/