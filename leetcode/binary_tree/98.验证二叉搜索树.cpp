/*
 * @lc app=leetcode.cn id=98 lang=cpp
 *
 * [98] 验证二叉搜索树
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
    void flag_BST(TreeNode* root,vector<int>& result) { 
        if (!root) return;
        if (root->left) {
           flag_BST(root->left,result);
        }
        result.emplace_back(root->val);
        if (root->right) {
           flag_BST(root->right,result);
        }
        return;
    }
    bool isValidBST(TreeNode* root) {
        vector<int> result;
        flag_BST(root,result);
        bool flag =true;
        if (!root || result.size() == 1) return flag;
        for (int i = 0; i < result.size()-1; ++i) {
            if (result[i+1] <= result[i]) {
                flag = false;
                break;
            }
        }
        return flag;
    }
};
// @lc code=end

