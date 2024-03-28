/*
 * @lc app=leetcode.cn id=11 lang=cpp
 *
 * [11] 盛最多水的容器
 */

// @lc code=start
class Solution {
public:
    int maxArea(vector<int>& height) {
        int i = 0,j = height.size()-1;
        int max = (j-i)*std::min(height[j],height[i]);
        while (i < j) {
            if (height[j]  > height[i]) {
                int temp = i;
                while (height[++i] < height[temp]) {}
                if (i < j && max < (j-i)*std::min(height[j],height[i])) {
                    max = (j-i)*std::min(height[j],height[i]);
                }
            } else {
                int temp = j;
                while (height[--j] < height[temp]) {}
                if (i < j && max < (j-i)*std::min(height[j],height[i])) {
                    max = (j-i)*std::min(height[j],height[i]);
                }
            }
        }
        return max;
    }
};
// @lc code=end

