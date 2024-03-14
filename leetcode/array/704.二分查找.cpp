/*
 * @lc app=leetcode.cn id=704 lang=cpp
 *
 * [704] 二分查找
 */

// @lc code=start
class Solution//这里仅适用于左闭右闭区间，左闭右开区间有一定的调整
{
public:
    int search(vector<int> &nums, int target)
    {
        int left = 0;
        int right = nums.size()-1;
        while(left <= right){
            int middle = (left+right)/2;//开改成右移
            if(nums[middle] == target){
                return middle;
            }else if(nums[middle]>target){
                right = middle-1;
            }else{
                left = middle+1;
            }
        }
        return -1;
    }
};
// @lc code=end
