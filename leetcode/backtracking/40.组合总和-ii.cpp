/*
 * @lc app=leetcode.cn id=40 lang=cpp
 *
 * [40] 组合总和 II
 */

// @lc code=start
class Solution {
    int targetsum = 0;
    vector<int> candidates_;
    vector <vector<int>> result;
    vector <int> path;
    void backtracking(int sum, int index) {
        if (sum >= targetsum) {
            if (sum == targetsum) 
                result.push_back(path);
            return;
        }
        for (int i = index; i < candidates_.size();) {
            sum += candidates_[i];
            path.push_back(candidates_[i]);
            backtracking(sum,i+1);
            path.pop_back();
            sum -= candidates_[i];
            i++;
            while (i != candidates_.size() && candidates_[i-1] == candidates_[i]) {
                i++;//这里做去重逻辑
            }
        }
        return;
    }
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
       sort(candidates.begin(),candidates.end());//排序方便做去重逻辑
       candidates_  = candidates;
       targetsum = target;
       backtracking(0,0);
       return result;
    }
};
// @lc code=end

