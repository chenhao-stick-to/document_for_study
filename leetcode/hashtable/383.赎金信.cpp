/*
 * @lc app=leetcode.cn id=383 lang=cpp
 *
 * [383] 赎金信
 */

// @lc code=start
class Solution {
public:
    bool canConstruct(string ransomNote, string magazine) {
        unordered_map <int,int> hash_ran;
        unordered_map <int,int> hash_mag;
        for (int i = 0; i < ransomNote.size(); i++) {
            if(hash_ran.find(ransomNote[i]) != hash_ran.end()) {
                hash_ran[ransomNote[i]]++;
            } else {
                hash_ran.insert(std::make_pair(ransomNote[i],1));
            }
        }
        for (int i = 0; i < magazine.size(); i++) {
            if(hash_mag.find(magazine[i]) != hash_mag.end()) {
                hash_mag[magazine[i]]++;
            } else {
                hash_mag.insert(std::make_pair(magazine[i],1));
            }
        }
        for (auto it1 = hash_ran.begin(); it1 != hash_ran.end(); it1++) {
            auto it2 = hash_mag.find(it1->first);
            if (it2 == hash_mag.end() || it1->second > it2->second) {
                return false;
            } 
        }
        return true;
    }
};
// @lc code=end

