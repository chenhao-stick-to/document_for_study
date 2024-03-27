/*
 * @lc app=leetcode.cn id=1 lang=cpp
 *
 * [1] 两数之和
 */

// @lc code=start
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        // multimap<int,int> map_vec;//重复键使用multimap
        // vector<int> result;
        // for (int i = 0; i < nums.size(); ++i) {
        //     map_vec.insert(make_pair(nums[i],i));
        //     if (map_vec.find(target-nums[i]) != map_vec.end() && map_vec.find(target-nums[i])->second != i) {//去除自身影响
        //         result.emplace_back(i);
        //         result.emplace_back(map_vec.find(target-nums[i])->second);
        //         break;
        //     }
        // }
        // return result;

        unordered_map<int,int> map_vec;//重复键使用multimap
        vector<int> result;
        for (int i = 0; i < nums.size(); ++i) {
            if (map_vec.find(target-nums[i]) != map_vec.end()) {
                result.emplace_back(i);
                result.emplace_back(map_vec.find(target-nums[i])->second);
                break;
            }
            map_vec[nums[i]] = i;
        }
        return result;
    }
};
// @lc code=end

