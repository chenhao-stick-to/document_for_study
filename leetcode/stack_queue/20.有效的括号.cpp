/*
 * @lc app=leetcode.cn id=20 lang=cpp
 *
 * [20] 有效的括号
 */

// @lc code=start
class Solution {
public:
    bool isValid(string s) {
        stack <char> stack_s;
        for (int i = 0; i < s.size(); ++i) {
            if (s[i] == '(' || s[i] == '{' || s[i] == '[') {
                stack_s.push(s[i]);
            } else {
                if (stack_s.empty()) 
                    return false;
                if (s[i] == ')') {
                    int temp = stack_s.top();
                    if (temp != '(') 
                        return false;
                    stack_s.pop();
                }
                if (s[i] == '}') {
                    int temp = stack_s.top();
                    if (temp != '{') 
                        return false;
                    stack_s.pop();
                }
                if (s[i] == ']') {
                    int temp = stack_s.top();
                    if (temp != '[') 
                        return false;
                    stack_s.pop();
                }
            }   
        }
        if (stack_s.empty()) 
            return true;
        return false;
    }
};
// @lc code=end

