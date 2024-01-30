/*
 * @lc app=leetcode.cn id=1047 lang=cpp
 *
 * [1047] 删除字符串中的所有相邻重复项
 */

// @lc code=start
class Solution {
public:
    string removeDuplicates(string s) {
        stack <int> stack_s;
        for (int i = 0; i < s.size(); i++) {
            if (stack_s.empty()){
                stack_s.push(s[i]);
            } else {
                int temp = stack_s.top();
                if (temp == s[i]) {
                    stack_s.pop();
                } else {
                    stack_s.push(s[i]);
                }
            }
        }
        string result;
        while(!stack_s.empty()) {
            result += stack_s.top();
            stack_s.pop();
        }
        std::reverse(result.begin(),result.end());
        return result;
    }
};
// @lc code=end

