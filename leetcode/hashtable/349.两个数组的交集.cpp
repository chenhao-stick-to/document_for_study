/*
 * @lc app=leetcode.cn id=349 lang=cpp
 *
 * [349] 两个数组的交集
 */

// @lc code=start
class Solution {//使用
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        vector<bool> hash_vector(1001,false);
        vector <int> result;
        for (int i = 0; i < nums1.size(); ++i) {
            hash_vector[nums1[i]] = true;
        }
        for (int i = 0; i < nums2.size(); ++i) {
            if (hash_vector[nums2[i]] == true) {
                result.push_back(nums2[i]);
                hash_vector[nums2[i]] = false;
            }
        }
        return result;
    }
};
// @lc code=end

