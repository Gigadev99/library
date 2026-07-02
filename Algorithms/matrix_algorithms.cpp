#pragma once
#include <vector>
#include <span>
using std::vector, std::span, std::plus, std::minus, std::multiplies;

void element_wise_to(const auto& A, const auto& B, auto& C, auto f) {
    int n = A.rows();
    int m = A.cols();
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < m; j++) 
            C[i, j] = f(A[i, j], B[i, j]);
}
void element_wise(auto& A, const auto& B, auto f) { element_wise_to(A, B, A, f); }

void matadd(auto& A, const auto& B) { element_wise(A, B, plus{}); }
void matsub(auto& A, const auto& B) { element_wise(A, B, minus{}); }
void hadamard(auto& A, const auto& B) { element_wise(A, B, multiplies{}); }
void hadamard_to(const auto& A, const auto& B, auto& C) { element_wise_to(A, B, C, multiplies{}); }

template<int N, int M, int P, typename T>
void matmul(T (&A)[N][M], T (&B)[M][P], T (&C)[N][P]) {
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < M; k++) { 
            for (int j = 0; j < P; j++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

