/*
 * @lc app=leetcode.cn id=15 lang=cpp
 *
 * [15] 三数之和
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        multimap <int,vector<int>> hash_nums;
        vector <vector<int>> result; 
        unordered_set <int> flag_nums;//保证索引不重复
        for (int i = 0; i < nums.size(); i++) {
            vector <int> temp;
            for (int j = i + 1; j < nums.size(); j++) {
                temp.push_back(i);
                temp.push_back(j);
                hash_nums.insert(make_pair(-(nums[i]+nums[j]),temp));
            }
        }
        for (int i = 0; i < nums.size(); i++) {
            for (auto it = hash_nums.find(nums[i]); it != hash_nums.end() && it->first == nums[i]; it++) {
                if (i != it->second[0] && i != it->second[1]) {
                    if (((flag_nums.find(i) == flag_nums.end()) || (flag_nums.find((it->second)[0]) == flag_nums.end()) || (flag_nums.find((it->second)[1]) == flag_nums.end())) ) {//三个不全在已标记数据里面，索引和值一起
                        if (flag_nums.find(i) == flag_nums.end()) {
                            flag_nums.insert(i);
                        } 
                        if (flag_nums.find((it->second)[0]) == flag_nums.end()) {
                            flag_nums.insert((it->second)[0]);
                        } 
                        if (flag_nums.find((it->second)[1]) == flag_nums.end()) {
                            flag_nums.insert((it->second)[1]);
                        }
                        vector <int> temp;
                        temp.push_back(nums[i]);
                        temp.push_back(nums[(it->second)[0]]);
                        temp.push_back(nums[(it->second)[1]]);
                        result.push_back(temp);
                    }
                }
            }
        }
        return result;
        
    }
};
// @lc code=end

