/*
 * @lc app=leetcode.cn id=15 lang=cpp
 *
 * [15] 三数之和
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        // multimap <int,vector<int>> hash_nums;
        // vector <vector<int>> result; 
        // unordered_set <int> flag_nums;//保证索引不重复
        // for (int i = 0; i < nums.size(); i++) {
        //     vector <int> temp;
        //     for (int j = i + 1; j < nums.size(); j++) {
        //         temp.push_back(i);
        //         temp.push_back(j);
        //         hash_nums.insert(make_pair(-(nums[i]+nums[j]),temp));
        //     }
        // }
        // for (int i = 0; i < nums.size(); i++) {
        //     for (auto it = hash_nums.find(nums[i]); it != hash_nums.end() && it->first == nums[i]; it++) {
        //         if (i != it->second[0] && i != it->second[1]) {
        //             if (((flag_nums.find(i) == flag_nums.end()) || (flag_nums.find((it->second)[0]) == flag_nums.end()) || (flag_nums.find((it->second)[1]) == flag_nums.end())) ) {//三个不全在已标记数据里面，索引和值一起
        //                 if (flag_nums.find(i) == flag_nums.end()) {
        //                     flag_nums.insert(i);
        //                 } 
        //                 if (flag_nums.find((it->second)[0]) == flag_nums.end()) {
        //                     flag_nums.insert((it->second)[0]);
        //                 } 
        //                 if (flag_nums.find((it->second)[1]) == flag_nums.end()) {
        //                     flag_nums.insert((it->second)[1]);
        //                 }
        //                 vector <int> temp;
        //                 temp.push_back(nums[i]);
        //                 temp.push_back(nums[(it->second)[0]]);
        //                 temp.push_back(nums[(it->second)[1]]);
        //                 result.push_back(temp);
        //             }
        //         }
        //     }
        // }
        // return result;
        vector<vector<int>> result;//!这道题二刷需要再做，一刷没做出来
        sort(nums.begin(), nums.end());
        // 找出a + b + c = 0
        // a = nums[i], b = nums[left], c = nums[right]
        for (int i = 0; i < nums.size(); i++) {
            // 排序之后如果第一个元素已经大于零，那么无论如何组合都不可能凑成三元组，直接返回结果就可以了
            if (nums[i] > 0) {
                return result;
            }
            // 错误去重a方法，将会漏掉-1,-1,2 这种情况
            /*
            if (nums[i] == nums[i + 1]) {
                continue;
            }
            */
            // 正确去重a方法
            if (i > 0 && nums[i] == nums[i - 1]) {//防止和以前一样的组合出现;注意和前面比而不是后面
                continue;
            }
            int left = i + 1;
            int right = nums.size() - 1;//滑动双指针
            while (right > left) {
                // 去重复逻辑如果放在这里，0，0，0 的情况，可能直接导致 right<=left 了，从而漏掉了 0,0,0 这种三元组
                /*
                while (right > left && nums[right] == nums[right - 1]) right--;
                while (right > left && nums[left] == nums[left + 1]) left++;
                */
                if (nums[i] + nums[left] + nums[right] > 0) right--;
                else if (nums[i] + nums[left] + nums[right] < 0) left++;
                else {
                    result.push_back(vector<int>{nums[i], nums[left], nums[right]});//先加是为了防止漏掉[0,0,0......]的情况,
                    // 去重逻辑应该放在找到一个三元组之后，对b 和 c去重
                    while (right > left && nums[right] == nums[right - 1]) right--;
                    while (right > left && nums[left] == nums[left + 1]) left++;

                    // 找到答案时，双指针同时收缩
                    right--;
                    left++;
                }
            }

        }
        return result;
    }
};
// @lc code=end

