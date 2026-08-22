#pragma once
#include <cassert>
#include <vector>  
#include <array> 
#include <span>
#include <iostream>
#include "../utils.cpp" 
using std::array;
template<typename M> concept MatType = requires(M A) { A.rows(); A.cols(); A[0, 0]; }; 
template<typename M> concept OwningMatType = MatType<M> && requires(M A) { A.storage; };

template<typename T, int N = dynamic, int M = dynamic>
struct Matrix { 
    int r, c; // rows, cols
    vector<T> storage;
    Matrix() : r(0), c(0) {}
    Matrix(int n, int m, T val = {})  : r(n), c(m),  storage(n * m, val) {}
    Matrix(T* nums, int n, int m) : r(n), c(m),  storage(nums, nums + n * m) { assert(nums != nullptr); }
    Matrix(init_list<init_list<T>> rows) : r(rows.size()), c(rows.begin()->size()), storage(r * c) { 
        int i = 0; 
        for (const auto& row : rows) { assert(row.size() == c); 
            for (const auto& val : row) storage[i++] = val; 
        }
    }
    void resize(int n, int m)        { r = n; c = m; storage.resize(n * m); } 
    T&       operator[](int i, int j)       { return storage[i * c + j]; }
    const T& operator[](int i, int j) const { return storage[i * c + j]; }
    span<T>  operator[](int i)              { return span<T>(&(*this)[i, 0], c); }
    span<const T> operator[](int i)   const { return span<const T>(&(*this)[i, 0], c); }
    int rows() const { return r; } 
    int cols() const { return c; }
    using value_type = T;
};

template<typename T>
struct MatrixView {
    T* ptr; int r, c;  // rows, cols
    T& operator[](int i, int j) const { return ptr[i * c + j]; } 
    span<T>   operator[](int i) const { return span<T>(&(*this)[i, 0], c); }
    MatrixView() : ptr(nullptr), r(0), c(0) {}
    MatrixView(T* data, int n, int m) : ptr(data), r(n), c(m) {}
    template<int N, int M> MatrixView(T (&arr)[N][M]) : ptr(&arr[0][0]), r(N), c(M) {}
    int rows() const { return r; }
    int cols() const { return c; }
    using value_type = T;
};
template<typename T, int N, int M> 
auto matview(const Matrix<T, N, M>& A) { return MatrixView<T>{&A[0, 0], A.rows(), A.cols()}; }

template<typename T, int N, int M> requires (N != dynamic && M != dynamic)
struct Matrix<T, N, M> {
    array<array<T, M>, N> storage; 
    Matrix() = default; 
    Matrix(T val) { for (auto& row : storage) row.fill(val); }
    Matrix(int n, int m, T val = {}) { assert(n == N && m == M); for (auto& row : storage) row.fill(val); }
    Matrix(init_list<array<T, M>> rows) { assert(rows.size() == N);
        int i = 0; for (const auto& row : rows) storage[i++] = row;
    }
    T&       operator[](int i, int j)       { return storage[i][j]; }
    const T& operator[](int i, int j) const { return storage[i][j]; }
    span<T>  operator[](int i)              { return span<T>(&(*this)[i, 0], M); } 
    span<const T> operator[](int i)   const { return span<const T>(&(*this)[i, 0], M); }
    int rows() const { return N; }
    int cols() const { return M; }
    using value_type = T;
};

// Matrix transforms. Dont mark them const. Dont nest like rotation(rotation(A)) (dangling reference!).

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

// Left/CCW rotation
template<MatType M>
struct RotateMatrix {
    M& matrix; 
    auto& operator[](int i, int j) { return matrix[j, matrix.cols() - i - 1]; }
    int rows() const { return matrix.cols(); }
    int cols() const { return matrix.rows(); }
    using value_type = M::value_type;
};
// Left/CCW rotation
auto rotation(MatType auto& matrix) { return RotateMatrix{matrix}; }

template<MatType M>
struct TransposeMatrix {
    M& matrix;
    auto& operator[](int i, int j) { return matrix[j, i]; }
    int rows() const { return matrix.cols(); }
    int cols() const { return matrix.rows(); }
    using value_type = M::value_type;
};
auto transpose(MatType auto& matrix) { return TransposeMatrix{matrix}; }

template<MatType M, bool Horizontal = true>
struct ReflectMatrix {
    M& matrix;
    auto& operator[](int i, int j) { 
        if constexpr (Horizontal) return matrix[i, matrix.cols() - j - 1]; 
        else return matrix[matrix.rows() - i - 1, j]; 
    }
    int rows() const { return matrix.rows(); }
    int cols() const { return matrix.cols(); }
    using value_type = M::value_type;
};
auto reflection_hor(MatType auto& matrix) { return ReflectMatrix{matrix, true }; }
auto reflection_ver(MatType auto& matrix) { return ReflectMatrix{matrix, false}; }

bool equal(MatType auto&& A, MatType auto&& B) {
    if (A.rows() != B.rows() || A.cols() != B.cols()) return false;
    for (int i = 0; i < A.rows(); i++) 
        for (int j = 0; j < A.cols(); j++) 
            if (A[i, j] != B[i, j]) return false;
    return true;
}
void matprint(MatType auto&& A, string name = "") { 
    std::cout << name << (!name.empty() ? ":\n" : "");
    for (int i = 0; i < A.rows(); i++) {
        for (int j = 0; j < A.cols(); j++) 
            std::cout << A[i][j] << ' ';
        std::cout << '\n';
    }
}
#define MATPRINT(A) matprint(A, #A)

// Polymorphic wrapper that can hold any matrix type
template<typename T>
struct AnyMatrix {
    void* matrix;
    int (*rows_func)(void*);
    int (*cols_func)(void*);
    T& (*access)(void*, int, int);
    template<MatType M>
    AnyMatrix(M& A) : matrix(&A) {
        rows_func = [](void* m) { return static_cast<M*>(m)->rows(); };
        cols_func = [](void* m) { return static_cast<M*>(m)->cols(); };
        access = [](void* m, int i, int j) { return (*static_cast<M*>(m))[i, j]; };
    }
    int rows() const { return rows_func(matrix); }
    int cols() const { return cols_func(matrix); }
    T& operator[](int i, int j) { return access(matrix, i, j); }
    const T& operator[](int i, int j) const { return access(matrix, i, j); }
    using value_type = T;
};

int main() {
    vector<int> v = {1,2,3,4,5,6};
    Matrix<int> M(2,3);
    M.storage = move(v); 
    int arr[3][3] = {};
    MatrixView m2(arr);
    auto m3 = matview(M);
    vector<vector<int>> vec = {{1,1,1},{2,2,2}};
    Matrix<int,3,3> M1 = {{3,3,3},{4,4,4},{5,5,5}};
    matprint(M1);
    Gridify g{vec};
    matprint(g);
}
*/
// Experimental
template<MatType M>
struct MatrixRow {
    M& matrix;
    int i;
    auto& operator[](int j) { return matrix[i, j]; }
    struct iterator {
        MatrixRow& row;
        int j;
        decltype(auto) operator*() const { return row[j]; }
        iterator& operator++() { j++; return *this; }
        bool operator==(const iterator& other) const { return j == other.j; }

    };
    iterator begin() { return {matrix, i, 0}; }
    iterator end()   { return {matrix, i, matrix.cols()}; }
};

template<MatType M>
struct MatrixIterator {
    M& matrix;
    int i;
    auto operator*() { return MatrixRow<M>{matrix, i}; }
    MatrixIterator& operator++() {
        ++i;
        return *this;
    }
    bool operator!=(const MatrixIterator& other) const { return i != other.i; }
};