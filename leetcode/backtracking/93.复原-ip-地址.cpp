/*
 * @lc app=leetcode.cn id=93 lang=cpp
 *
 * [93] 复原 IP 地址
 */

// @lc code=start
class Solution {
public:
    string path;
    vector <vector<string>> result;
    string s_;
    int point = 0;
    bool judge_trueip(string& s) {
        if (s.size() == 1) return true;
        if (s.size() > 1 && s[0] == "0") return false;
        if (stoi(s) >= 0 && stoi(s) <= 255) return true;
        return false;
    }
    void backtracking(int index) {
        if (point == 4) {
            result.push_back(path);
            return;
        }
        for (int i = index; i < s_.size(); ++i) {
            string temp;
            if (point != 3)
                temp = s_.substr(index,i-index+1);
            else temp = s_.substr(index);
            if (judge_trueip(temp)) {
                point++;
                if (point == 4)
                    path += temp+".";
                else path += temp+".";
                backtracking(i + 1);
                if (point == 4) {
                    path = path.substr(0,path.size()-temp.size());
                    point--;
                    return;
                }
                else path = path.substr(0,path.size()-1-temp.size());
                point--;
            } else return;
        }
    }
    vector<vector<string>> partition(string s) {
        s_ = s;
        backtracking(0);
        return result;
    }
};
// @lc code=end

