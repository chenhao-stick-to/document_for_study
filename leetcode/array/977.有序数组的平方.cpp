/*
 * @lc app=leetcode.cn id=977 lang=cpp
 *
 * [977] 有序数组的平方
 */

class Solution {//如果限定空间复杂度O（1）.如何解决？
public:
    vector<int> sortedSquares(vector<int>& nums) {
        int i = 0;
        int j = nums.size()-1;
        int ksize = j;
        vector<int> nums_result=nums;
        if(j < 0) {
            return nums_result;
        }
        while (i <= j) {
            if (abs(nums[i]) <= abs(nums[j])) {
                nums_result[ksize--] = pow(nums[j--],2);
            } else {
                nums_result[ksize--]=pow(nums[i++],2);
            }
        }
        return nums_result;
    }
};
// @lc code=end

