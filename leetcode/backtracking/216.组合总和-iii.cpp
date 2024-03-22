/*
 * @lc app=leetcode.cn id=216 lang=cpp
 *
 * [216] 组合总和 III
 */

// @lc code=start
class Solution {
    vector <vector<int>> result;
    vector <int> path;
    void backtracking(int k, int n, int startIndex) {//可以再定义一个sum变量，一起进行回溯，避免计算
        if (path.size() == k) {
            if (accumulate(path.begin(),path.end(),0) == n)
                result.push_back(path);
            return;
        }
        for (int i = startIndex; i <= 9-(k-path.size())+1; ++i) {
            path.push_back(i);
            backtracking(k,n,i+1);
            path.pop_back();
        }
        return ;
    }
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        if (k > n) return result;
        backtracking(k,n,1);
        return result;
    }
};
// @lc code=end

