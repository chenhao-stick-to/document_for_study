/*
 * @lc app=leetcode.cn id=27 lang=cpp
 *
 * [27] 移除元素
 */

// @lc code=start
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int size = 0;
        int ksize=nums.size();
        for(int i = 0;i < ksize;++i){
            if(val == nums[i]){
                nums.erase(nums.begin()+i);
                continue;
            }
            size++;
        }
        return size;
    }
};
// @lc code=end

