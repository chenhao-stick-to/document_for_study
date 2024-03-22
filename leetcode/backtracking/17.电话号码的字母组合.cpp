/*
 * @lc app=leetcode.cn id=17 lang=cpp
 *
 * [17] 电话号码的字母组合
 */

// @lc code=start
class Solution {
public:
    vector<string> result;
    string path = "";//保存一次完整路径
    string digits_;
    int str_size = 0;
    unordered_map <int,string> map_dict;
    void backtracking(int index) {
        if (path.size() == str_size) {
            if (str_size) 
                result.push_back(path);
            return;
        }
        for (int i = 0; i < map_dict[int(digits_[index]-'0')].size(); i++) {
            path.push_back(map_dict[int(digits_[index]-'0')][i]);
            backtracking(index+1);
            path.pop_back();
        }
        return;
    }
public:
    vector<string> letterCombinations(string digits) {
        str_size = digits.size();
        digits_ = digits;
        vector <string> temp = {"abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"};
        for (int i = 2; i <= 9; i++) {
            map_dict[i] = temp[i-2];
        }
        backtracking(0);
        return result;
    }
};
// @lc code=end

