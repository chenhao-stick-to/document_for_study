/*
 * @lc app=leetcode.cn id=110 lang=cpp
 *
 * [110] 平衡二叉树
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {//可以使用最大树高，最小树高差值来做。//这里还是遍历的方式来做
public:
    int depth(TreeNode* root,bool& flag) {//考虑使用-1表示高度作为flag，从而去掉flag的变量。
        if (!root) return 0;
        int left_depth = depth(root->left,flag);
        int right_depth = depth(root->right,flag);
        if (abs(left_depth-right_depth) > 1) {
            flag = false;
            return 0;
        } else {
            return max(left_depth,right_depth)+1;
        }
    }
    bool isBalanced(TreeNode* root) {
        bool flag = true;
        int temp = depth(root,flag);
        return flag;
    }
};
// @lc code=end

