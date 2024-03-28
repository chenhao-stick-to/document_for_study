/*
 * @lc app=leetcode.cn id=42 lang=cpp
 *
 * [42] 接雨水
 */

// @lc code=start
class Solution {//s三种方法，双指针，单调栈，动态规划
public:
    int trap(vector<int>& height) {
        int left = 0, right = height.size()-1;
        int result = 0;
        auto maxElementIter = std::max_element(height.begin(), height.end());
        vector <int> max_index;
        for (int i = 0; i < height.size(); ++i) {
            if (*maxElementIter == height[i]) max_index.emplace_back(i);
        }
        int low_index = max_index[0];
        int high_index = max_index[max_index.size()-1];
        int temp;
        for (int i = 0; i  < low_index; ) {
            temp = i;
            while(height[++i] < height[temp]) {}
            if (i > low_index) break;
            result += (i-temp-1)*height[temp];
            for (int k = temp + 1; k < i; k++) {
                result -= height[k];
            }
        }
        for (int j = height.size()-1; j > high_index;) {
            temp = j;
            while(height[--j] < height[temp]) {}
            if (j < high_index) break;
            result += (temp-j-1)*height[temp];
            for (int k = temp - 1; k > j; k--) {
                result -= height[k];
            }
        }
        if ( max_index.size() >= 2) {
            result += (high_index-low_index-1)*height[low_index];
            for (int i = low_index+1; i < high_index; i++) {
                result -= height[i];
            }
        }
        return result;
    }
};
// @lc code=end

