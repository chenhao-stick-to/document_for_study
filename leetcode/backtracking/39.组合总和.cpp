/*
 * @lc app=leetcode.cn id=39 lang=cpp
 *
 * [39] 组合总和
 */

// @lc code=start
class Solution {
public:
    int targetsum = 0;
    vector<int> candidates_;
    vector <vector<int>> result;
    vector <int> path;
    void backtracking(int sum, int num) {
        if (sum >= targetsum) {
            if (sum == targetsum) 
                result.push_back(path);
            return;
        }
        for (int i = 0; i < candidates_.size(); ++i) {
            if (candidates_[i] < num) continue;//有序去除多余的重复元素
            sum += candidates_[i];
            path.push_back(candidates_[i]);
            backtracking(sum,candidates_[i]);
            path.pop_back();
            sum -= candidates_[i];
        }
    }
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
       sort(candidates.begin(),candidates.end()); 
       candidates_  = candidates;
       targetsum = target;
       int sum = 0;
       backtracking(sum,candidates_[0]);
       return result;
    }
};
// @lc code=end

