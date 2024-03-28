/*
 * @lc app=leetcode.cn id=15 lang=cpp
 *
 * [15] 三数之和
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        std::sort(nums.begin(),nums.end());
        vector <vector<int>> result;
        for (int i = 0; i < nums.size()-2; i++) {//最外层循环
            if (nums[i] > 0) break;//快速退出
            if ( i > 0 && nums[i] == nums[i-1]) {
                continue;
            }
            int low = i+1;
            int high = nums.size()-1;
            while (low < high) {
                if (nums[low] + nums[high] > -nums[i]) {
                    high--;
                } else if (nums[low] + nums[high] < -nums[i]) {
                    low++;
                } else {
                    vector <int> temp={nums[i],nums[low],nums[high]};
                    result.push_back(temp);
                    while (high > low && nums[high] == nums[high-1]) high--;
                    while(low < high && nums[low+1] == nums[low]) low++;
                    high--;
                    low++;
                }
            }
        }
        return result;
    }
};
// @lc code=end

