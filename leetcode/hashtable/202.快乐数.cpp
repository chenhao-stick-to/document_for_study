/*
 * @lc app=leetcode.cn id=202 lang=cpp
 *
 * [202] 快乐数
 */

// @lc code=start
class Solution {
public:
    vector <int> exists;//考虑不用递归，使用循环，以及使用unorder_set；而不是数组。
    bool isHappy(int n) {
        int temp = n;
        vector <int> bits; 
        for (int i = 9; i >= 0; i--) {
            if (pow(10,i) > temp){
                bits.push_back(0);
                continue;
            }
            bits.push_back(temp/pow(10,i));
            temp = temp - (int)(temp/pow(10,i))*pow(10,i);
        }
        temp = 0;
        for (int j = 0; j < bits.size(); j++) {
            temp += bits[j]*bits[j];
        }
        if (temp == 1) {
            return true;
        } 
        else {
            if (find(exists.begin(),exists.end(),n) != exists.end())
                return false;
            exists.push_back(n);
            return isHappy(temp); 
        }
    }
};
// @lc code=end

