/*
 * @lc app=leetcode.cn id=77 lang=cpp
 *
 * [77] 组合
 */

// @lc code=start
class Solution {//使用经典的回溯的模板。返回值和参数确定；终止条件，遍历过程和处理。
private:
    vector <vector<int>> result;
    vector <int> path;
    void backtracking(int n, int k, int startIndex) {//参数以及返回值确定
        if (path.size() == k){//终止条件
            result.push_back(path);
            return;
        }

        // for(int i = startIndex; i <= n; i++) {//遍历和处理过程；可以剪枝优化。
        //     path.push_back(i);
        //     backtracking(n,k,i+1);
        //     path.pop_back();
        // }

        for(int i = startIndex; i <= n-(k-path.size())+1; i++) {//可以剪枝优化。考虑剩余元素加上path.size()个数不足k个。
            path.push_back(i);
            backtracking(n,k,i+1);
            path.pop_back();
        }
        return;
    }
public:
    vector<vector<int>> combine(int n, int k) {
        backtracking(n,k,1);
        return result;
    }
};
// @lc code=end

