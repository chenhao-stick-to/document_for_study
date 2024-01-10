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
        int ksize = nums.size() - 1;
        if (ksize < 0){
            return 0;
        }
        // for (int i = 0;i < ksize;++i) {
        //     if(val == nums[i]){
        //         nums.erase(nums.begin()+i);
        //         continue;
        //     }
        //     size++;
        // }
        while (size < ksize) {
            if (val != nums[size] && nums[ksize] == val) {
                size++;
                ksize--;
            } else if (val == nums[size] && nums[ksize] == val) {
                ksize--;
            } else if (val != nums[size] && nums[ksize] != val) {
                size++;
            } else {
                int temp = nums[size];
                nums[size] = nums[ksize];
                nums[ksize] = temp;
                ksize--;
                size++;
            }
        }
        if (nums[size] == val)
            size -= 1;
        return size + 1;
    }
};