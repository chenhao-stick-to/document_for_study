/*
 * @lc app=leetcode.cn id=106 lang=cpp
 *
 * [106] 从中序与后序遍历序列构造二叉树
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
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        if (inorder.size() == 0) return nullptr;
        int root_val = postorder.back();
        TreeNode* root = new TreeNode(root_val);
        postorder.pop_back();
        int i = 0;
        for (; i < inorder.size(); ++i) {
            if (inorder[i] == root->val)
                break;
        }
        vector <int> left_in(inorder.begin(),inorder.begin()+i);
        vector <int> right_in(inorder.begin()+i+1,inorder.end());
        vector <int> left_post(postorder.begin(),postorder.begin()+left_in.size());
        vector <int> right_post(postorder.begin()+left_in.size(),postorder.end());
        root->left = buildTree(left_in,left_post);
        root->right = buildTree(right_in,right_post);
        return root;
    }
};
// @lc code=end

