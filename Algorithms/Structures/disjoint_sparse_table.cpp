#include <vector>
#include <span>
#include <bit>

using namespace std;

void build_dst(const auto& nums, auto& table, auto f) {
    int n = size(nums);
    int ceil_log2_n = bit_width((unsigned int)n);
    for (int i = 0; i < ceil_log2_n; i++) {
        int k = ceil_log2_n - i;
        int w = (1 << k);
        int mid = w/2;
        for (int start = 0; start < n; start += w) {
            int end = start + w;
            int mid = (start + end)/2;
            for (int j = mid; j >= start; j--) {
                table[i, j] = (j < mid) ? f(nums[j], table[i, j+1]) : nums[j];
            }
            for (int j = mid + 1; j < end && j < n; j++) {
                table[i, j] = (j > mid + 1) ? f(table[i, j-1], nums[j]) : nums[j];
            }
        }
    }
}

auto query_dst(int a, int b, auto& table, auto f) {
    int first_differing_bit = a^b;
}

struct DisjointSparseTable {
    
};