/*
 * @lc app=leetcode.cn id=150 lang=cpp
 *
 * [150] 逆波兰表达式求值
 */

// @lc code=start
class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        stack <string> stack_s;
        for (int i = 0; i < tokens.size(); ++i) {
            char char_s = tokens[i].c_str()[0];
            int temp1,temp2;
            if (tokens[i].size() == 1) {
                switch(char_s) {
                    case '+':
                        temp1 = stoi(stack_s.top());
                        stack_s.pop();
                        temp2 = stoi(stack_s.top());
                        stack_s.pop();
                        stack_s.push(to_string(temp1+temp2));
                        break;
                    case '-':
                        temp1 = stoi(stack_s.top());
                        stack_s.pop();
                        temp2 = stoi(stack_s.top());
                        stack_s.pop();
                        stack_s.push(to_string(temp2-temp1));
                        break;
                    case '*':
                        temp1 = stoi(stack_s.top());
                        stack_s.pop();
                        temp2 = stoi(stack_s.top());
                        stack_s.pop();
                        stack_s.push(to_string(temp2*temp1));
                        break;
                    case '/':
                        temp1 = stoi(stack_s.top());
                        stack_s.pop();
                        temp2 = stoi(stack_s.top());
                        stack_s.pop();
                        stack_s.push(to_string((int)(temp2/temp1)));
                        break;
                    default:
                        stack_s.push(tokens[i]);
                        break;
                } 
            } else {
                    stack_s.push(tokens[i]);
            }
        }
        return stoi(stack_s.top());
    }
};
// @lc code=end

