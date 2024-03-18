/*
 * @lc app=leetcode.cn id=1 lang=cpp
 *
 * [1] 两数之和
 */

// @lc code=start
class Solution {//这里不考虑元素重复的情况，所以考虑使用unordered的情况。查询只有O(1)。另外注意在map初始化的时候就进行数据的判断，没有再把数据加入，省掉一个循环遍历的时间。
public:
    vector<int> twoSum(vector<int>& nums, int target) {//使用map。不适用map复杂度是n方，使用则是nlogn.
        multimap <int,int> hash_map;//key是nums的值，value是对应的索引。
        vector <int> result;
        for (int i = 0; i < nums.size(); i++) {
            // hash_map[nums[i]] = i;
            hash_map.insert({nums[i],i});
        }
        for (auto it = hash_map.begin(); it !=hash_map.end(); ++it) {//n的复杂度
            if (hash_map.find(target-it->first) != hash_map.end() && (hash_map.find(target-it->first))->second != it->second) {//logn的复杂度
                result.push_back(it->second);
                result.push_back((hash_map.find(target-it->first))->second);
                return result;
            }
            }
            return result;
        }
};
// @lc code=end

