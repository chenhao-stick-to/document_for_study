/*
 * @lc app=leetcode.cn id=151 lang=cpp
 *
 * [151] 反转字符串中的单词
 */

// @lc code=start
class Solution {//再做，空间复杂度o(n)主要是看思想，首先是字符串反转，去除多于空格，再单词翻转
public:
    string reverseWords(string s) {
        int j = s.size()-1;
        for (int i = 0; i <= j; i++,j--) {//字符串翻转
            char temp;
            temp = s[i];
            s[i] = s[j];
            s[j] = temp;
        }
        int index = 0;
        j = 0;
        while (j < s.size()) {//去除多于空格
            if (s[j] != " ") {
                s[index++] = s[j++];
            } else {
                if (j == 0){
                    while (s[j] == " ") {
                        
                    }
                } else {

                }
            }
            
        }

    }
};
// @lc code=end

