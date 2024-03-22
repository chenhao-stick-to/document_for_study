/*
 * @lc app=leetcode.cn id=131 lang=cpp
 *
 * [131] 分割回文串
 */

// @lc code=start
class Solution {
public:
    vector <string> path;
    vector <vector<string>> result;
    string s_;
    bool judge_true(string& s) {
        if (s.size() == 1) return true;
        if (s.size() > 1 && s[0] == "0") return false;
        if ()
    }
    void backtracking(int index) {
        if (index == s_.size() ) {
            result.push_back(path);
            return;
        }
        for (int i = index; i < s_.size(); ++i) {
            string temp = s_.substr(index,i-index+1);
            if (judge_huiwen(temp)) {
                path.push_back(temp);
                backtracking(i + 1);
                path.pop_back();
            }
        }
    }
    vector<vector<string>> partition(string s) {
        s_ = s;
        backtracking(0);
        return result;
    }
};
// @lc code=end

