/*
 * @lc app=leetcode.cn id=104 lang=cpp
 *
 * [104] 二叉树的最大深度
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
    int maxDepth(TreeNode* root) {//不用层序遍历，直接递归
        if (root == nullptr) return 0;
        int max_left = 1;
        int max_right = 1;
        if (root->left) 
            max_left += maxDepth(root->left);
        if (root->right)
            max_right += maxDepth(root->right);
        
        return max_left > max_right ? max_left : max_right;
    }
};
// @lc code=end

