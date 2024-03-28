/*
 * @lc app=leetcode.cn id=438 lang=cpp
 *
 * [438] 找到字符串中所有字母异位词
 */

// @lc code=start
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        int slid_size = p.size(); 
        multiset <char> set_p;
        vector <int> result;
        if (s.size() < p.size()) return result;
        for (int i = 0; i < slid_size; ++i) {
            set_p.insert(p[i]);
        }
        int i = 0;
        int flag = 0;
        multiset <char> temp1 = set_p;
        while(i < s.size()-slid_size+1) {
            string temp = s.substr(i,slid_size);
            for (int j = 0; j < slid_size; j++) {
                if (temp1.count(temp[j]) == 0) {
                    break;
                } else {
                    auto iter = temp1.find(temp[j]);
                    temp1.erase(iter);
                }
            }
            if (temp1.size() == 0) {
                result.push_back(i);
            }
            temp1 = set_p;
            i++;
        }
        return result;
    }
};
// @lc code=end

