/*
 * @lc app=leetcode.cn id=454 lang=cpp
 *
 * [454] 四数相加 II
 */

// @lc code=start
class Solution {
public:
    int fourSumCount(vector<int>& nums1, vector<int>& nums2, vector<int>& nums3, vector<int>& nums4) {
            std::unordered_map <int,int> hash1_2;
            std::unordered_map <int,int> hash3_4;
            for (int i = 0; i < nums1.size(); i++) {
                for (int j = 0; j < nums2.size(); j++) {
                    hash1_2[nums1[i]+nums2[j]]++;
                }
            }
            for (int i = 0; i < nums3.size(); i++) {
                for (int j = 0; j < nums4.size(); j++) {
                    hash3_4[-(nums3[i]+nums4[j])]++;
                }
            }
            int result = 0;
            for (auto it = hash1_2.begin(); it != hash1_2.end();it++) {
                if (hash3_4.find(it->first) != hash3_4.end()) {
                    result += it->second * (hash3_4.find(it->first))->second;
                }
            }
            return result;
    }
};
// @lc code=end

