/*
 * @lc app=leetcode.cn id=11 lang=cpp
 *
 * [11] 盛最多水的容器
 */

// @lc code=start
class Solution {
public:
    int maxArea(vector<int>& height) {
        int max = 0;
        int i = 0,j = 1;
        for (;j < height.size(); j++) {
            for (int h = i; h < j; h++) {
                if ( max < (j-h)*std::min(height[j],height[h])) {
                    i = h;
                    max = (j-h)*std::min(height[j],height[h]);  
                }
            }
        }
        return max;
    }
};
// @lc code=end

