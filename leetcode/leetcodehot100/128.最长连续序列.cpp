/*
 * @lc app=leetcode.cn id=128 lang=cpp
 *
 * [128] 最长连续序列
 */

// @lc code=start
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {//还可以使用unordered_set;使用o(1)查找节省时间
        // set <int> set_num;
        // for (int i = 0; i < nums.size(); ++i) set_num.insert(nums[i]);
        // int max_re = 0;
        // for (auto iter = set_num.begin(); iter != set_num.end();) {
        //     int temp = *iter;
        //     iter++;
        //     int direc = 1;
        //     while (iter != set_num.end() && temp == *iter-1) {
        //         direc++;
        //         temp = *iter;
        //         iter++;
        //     }
        //     max_re = max_re > direc ? max_re:direc;
        // }
        // return max_re;

        //unordered_set
        unordered_set <int> set_num;//使用map，set时一定要优先思考特点，能使用unoreder就使用！！！
        for (const auto& num:nums) {
            set_num.insert(num);
        }
        int max_re = 0;
        for (const auto& num:nums) {
            if  (!set_num.count(num+1)) {//确保num是连续最大的那个数
            int direc = 1;
            int temp = num;
            while (set_num.count(--temp) > 0) {
                direc++;
            }
            max_re = max_re > direc ? max_re:direc;
            }
        }
        return max_re;
    }
};
// @lc code=end

