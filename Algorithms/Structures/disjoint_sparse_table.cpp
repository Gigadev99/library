#include <vector>
#include <span>
#include <bit>
#include "matrix.cpp"
#include <iostream>
using namespace std;

// Builds a Disjoint sparse table from input nums, into table. table dimensions must be (ceil(log2(n)), n).
void build_dst(const auto& nums, auto& table, auto f) {
    int n = size(nums);
    int ceil_log2_n = bit_width((unsigned int)n);
    for (int i = 0; i < ceil_log2_n; i++) {
        int k = ceil_log2_n - i;
        int w = (1 << k);
        for (int start = 0; start < n; start += w) {
            int mid = min(start + w/2 - 1, n-1);
            int end = min(start + w - 1, n-1);
            for (int j = mid; j >= start; j--) {
                table[i, j] = (j < mid) ? f(nums[j], table[i, j + 1]) : nums[j];
            }
            for (int j = mid + 1; j <= end; j++) {
                table[i, j] = (j > mid + 1) ? f(table[i, j - 1], nums[j]) : nums[j];
            }
        }
    }
}

// Queries the Disjoint Sparse Table, from a to b. Must pass the original nums array as well, since the table does not store all values.
auto query_dst(unsigned int a, unsigned int b, const auto& nums, const auto& table, auto f) {
    if (a == b) return nums[a]; //  a^b == 0, so i = bit_width(n) - 0 = bit_width(n), which is out of bounds for the table.
    unsigned int n = size(nums);
    int i = bit_width(n) - bit_width(a^b); // first differing bit index
    int r = f(table[i, a], table[i, b]);
    return r;
}

template<typename T, typename F, typename Table = Matrix<T>>
struct DisjointSparseTable {
    unsigned int n;
    int k;
    F f;
    Table table;
    span<const T> nums;
    DisjointSparseTable(F func = F{}) : f(func) {}
    void build(const auto& nums) {
        n = size(nums);
        this->nums = nums;
        k = bit_width(n); 
        table = Table(k, n);
        build_dst(nums, table, f);
    }
    
    T query(unsigned int a, unsigned int b) {
        return query_dst(a, b, nums, table, f);
    }
};



/*
#include <random>
int brute_sum(const vector<int>& nums, int a, int b) {
    int result = nums[a];
    for (int i = a+1; i <= b; i++) result += nums[i];
    return result;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    mt19937 rng(42);
    int n;
    cin >> n;
    vector<int> nums(n);
    for (auto& x : nums) x = rng() % 100 + 1;
    DisjointSparseTable<int, plus<int>> dst; dst.build(nums);
    cout << "n=" << n << " nums:";
    for (int x : nums) cout << " " << x;
    cout << endl;
    
    for (int a = 0; a < n; a++) {
        for (int b = a; b < n; b++) {
            cout << "query a=" << a << " b=" << b
                    << " brute=" << brute_sum(nums, a, b) << '\n'
                    << "dst=" << dst.query(a, b) << '\n';
            if (brute_sum(nums, a, b) != dst.query(a, b)) {
                cout << "Error: mismatch for query(" << a << ", " << b << ")\n";
                return 1;
            }
        }
    }
    cout << "All queries passed successfully!" << endl;
    
}
*/