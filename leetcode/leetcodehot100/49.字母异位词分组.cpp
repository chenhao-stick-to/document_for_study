/*
 * @lc app=leetcode.cn id=49 lang=cpp
 *
 * [49] 字母异位词分组
 */

// @lc code=start
class Solution {//1思考字符串用整数来唯一标识（质数法）；2 思考使用位，一个字符串转为位来唯一标识，根据字符串范围来确定每个字母用多少位。//3.使用字符串排序后一致作为键值
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        map<vector<int>,vector<int>> umap_vec;
        vector<vector<string>> result;
        for (int i = 0; i < strs.size(); ++i) {
            string temp = strs[i];
            vector<int> temp_vec(26,0);
            for (int j = 0; j < temp.size(); j++) {
                temp_vec[(int)(temp[j]-'a')] += 1;
            }
            if (umap_vec.find(temp_vec) == umap_vec.end()) {
                umap_vec[temp_vec] = vector<int>{i};
            } else umap_vec[temp_vec].push_back(i);
        }
        for (auto iter = umap_vec.begin(); iter != umap_vec.end(); ++iter) {
            vector<string> temp;
            for (int i = 0; i < iter->second.size(); ++i) {
                temp.emplace_back(strs[(iter->second)[i]]);
            }
            result.emplace_back(temp);
        }
        return result;
    }
};
// @lc code=end

