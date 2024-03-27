/*
 * @lc app=leetcode.cn id=283 lang=cpp
 *
 * [283] 移动零
 */

// @lc code=start
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int j = 0;
        while (nums[j] != 0) { 
                j++;
                if (j == nums.size()) return;
            }
        for (int i = j;;) {
           while(nums[j] == 0)  {
                j++;
                if (j == nums.size()) return;
            }
           nums[i] = nums[j];
           nums[j] = 0;
           while(nums[++i] != 0) {}
        }
    }
};
// @lc code=end

