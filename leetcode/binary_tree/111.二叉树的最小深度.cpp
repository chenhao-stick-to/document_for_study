/*
 * @lc app=leetcode.cn id=111 lang=cpp
 *
 * [111] 二叉树的最小深度
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
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (root == nullptr) return 0;
        int min_left = 1;
        int min_right = 1;
        if (root->left) 
            min_left += minDepth(root->left);
        else min_left = pow(10,6)+1;
        if (root->right)
            min_right += minDepth(root->right);
        else min_right = pow(10,6)+1;
        int min_sub = min_left > min_right ? min_right : min_left;
        return min_sub > pow(10,6) ? 1 : min_sub;
    }
};
// @lc code=end

