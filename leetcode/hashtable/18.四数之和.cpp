/*
 * @lc app=leetcode.cn id=18 lang=cpp
 *
 * [18] 四数之和
 */

// @lc code=start
class Solution {//!折磨大题，以~再做，边界问题和用例适配问题。
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        vector <vector<int>> result;
        if (nums.size() < 4) {
            return result;
        }
        sort(nums.begin(),nums.end());
        for (int i = 0; i < nums.size()-3; i++) {
            if (i > 0 && nums[i] != nums[i-1] || i == 0) {//确保不进行a的重复元素包含
                if((nums[i] >= target && nums[i] > 0) || (nums[i] <= target && nums[nums.size()-1] < 0)) break;//尽快退出
                for (int j = i+1; j < nums.size()-2; j++){
                    // if (nums[j] > target - nums[i]) {//首元素淘汰，加快计算,由于负数，所以不可以添加此条件
                    //     break;
                    // }
                    if((nums[j] >= target-nums[i] && nums[j] > 0) || (nums[j] <= target-nums[i] && nums[nums.size()-1] < 0)) break;//尽快退出
                    if (j > i+1 && nums[j] == nums[j-1]) continue;//进行元素b的重复去除
                    int left = j+1;
                    int right = nums.size()-1;
                    while (left < right) {
                        if (nums[left] + nums[right] + nums[j] > target-nums[i]) right--;
                        else if (nums[left] + nums[right] + nums[j] < target-nums[i]) left++;
                        else {
                            result.push_back(vector<int> {nums[i],nums[j],nums[left],nums[right]});
                            while (left < right && nums[right] == nums[--right]){}//对c不重复包含
                            while (left < right && nums[left] == nums[++left]){}//对d不重复包含
                            if(nums[left-1] >= target-nums[i]-nums[j] && nums[left-1] > 0) break;//尽快退出
                        }
                    }
                }
            }
        }
        return result;
    }
};
// @lc code=end

