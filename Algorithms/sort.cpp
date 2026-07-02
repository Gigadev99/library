#include <span>
#include <vector>
#include <ranges>
using namespace std;

// MergeSort implementation, takes std::span as arguments
void mergesort(auto A, auto temp) {
    int n = A.size();
    if (n <= 1) return;
    int mid = n / 2;  
    auto left = A.subspan(0, mid);
    auto right = A.subspan(mid, n);
    mergesort(left, temp);
    mergesort(right, temp);
    ranges::merge_to(left, right, temp);
    ranges::copy_to(temp, A);
}

// Generic MergeSort, works on any contiguous container
void mergesort(auto& A) {
    int n = size(A);
    using T = ranges::range_value_t<decltype(A)>;
    vector<T> temp(n);
    mergesort(span(A), span(temp));
}
