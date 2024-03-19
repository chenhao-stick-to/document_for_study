/*
 * @lc app=leetcode.cn id=257 lang=cpp
 *
 * [257] 二叉树的所有路径
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
    void access(TreeNode* root,vector<string>& result) {
        if (!root) return;
        for (int i = 0; i < result.size(); ++i) {
            result[i] += to_string(root->val)+"->";
        }
        if (!root->left && root->right) {
            access(root->right,result);
        } else if (root->left && !root->right) {
            access(root->left,result);
        } else if(root->left && root->right){
            access()
        }
    }
    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> result; 
        access(root,result);
        return result;
    }
};
// @lc code=end

