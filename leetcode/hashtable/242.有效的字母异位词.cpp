/*
 * @lc app=leetcode.cn id=242 lang=cpp
 *
 * [242] 有效的字母异位词
 */

// @lc code=start
class Solution {
public:
    bool isAnagram(string s, string t) {
        std::map<char,int> maps;//unicode可采用宽字符和字符串
        std::map<char,int> mapt;//注意map是key有序，可顺序比较。
        for (int i = 0; i < s.size(); i++) {
            maps[s[i]]++;
        }
        for (int i = 0; i < t.size(); i++) {
            mapt[t[i]]++;
        }
        auto iters = maps.begin();
        auto itert = mapt.begin();
        while (iters != maps.end() && itert != mapt.end()) {
            if (iters->first == itert->first && iters->second == itert->second) {
                iters++;
                itert++;
                continue;
            }
            return false;
        }
        if (iters == maps.end() && itert == mapt.end()) {
            return true;
        }
        return false;
    }
};
// @lc code=end

