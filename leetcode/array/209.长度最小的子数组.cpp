/*
 * @lc app=leetcode.cn id=209 lang=cpp
 *
 * [209] 长度最小的子数组
 */

// @lc code=start
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int low = 0;
        int high = 0;
        int sum = 0;
        int min_count = 0;
        for (int i = 0; i < nums.size(); i++){//找到第一个满足的长度子数组
            if (sum >= target)
                break;
            sum += nums[i];
            high = i;
        }
        if (sum < target){//不满足条件
            return 0;
        }
        min_count = high + 1; 
        int count = min_count;
        while (high < nums.size()){//终止条件,窗口值大于等于target移动low，否则移动high。
            if (sum >= target){
                sum -= nums[low++];
                count--;
                if (sum >= target && min_count > count)
                    min_count = count;
            } else {
                if (high + 1 == nums.size()){
                    return min_count;
                } else {
                    sum += nums[++high];
                    count++;
                }
            }
        }
        return min_count;
    }
};
// @lc code=end