/*
 * @lc app=leetcode.cn id=1 lang=cpp
 *
 * [1] 两数之和
 */

// @lc code=start
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {//使用map。
        multimap <int,int> hash_map;//key是nums的值，value是对应的索引。
        vector <int> result;
        for (int i = 0; i < nums.size(); i++) {
            hash_map[nums[i]] = i;
        }
        for (auto it = hash_map.begin(); it !=hash_map.end(); ++it) {
            if (it->first > target) {
                continue;
            }
            int index = target;
            while (index > 0) {
                if (hash_map.find(index) != hash_map.end()) {
                    result.push_back(hash_map[index]);
                    break;
                }
                index -= 
        
            }
        
        }
};
// @lc code=end

