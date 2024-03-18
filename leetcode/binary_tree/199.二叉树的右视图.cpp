/*
 * @lc app=leetcode.cn id=199 lang=cpp
 *
 * [199] 二叉树的右视图
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
    vector<int> rightSideView(TreeNode* root) {
        queue <TreeNode*> tree_queue;
        vector <int> result;
        if (!root) return result;
        tree_queue.push(root);
        while (!tree_queue.empty()) {
            int size = tree_queue.size();
            TreeNode* temp = nullptr;
            for (int i = 0; i < size; i++) {
                temp = tree_queue.front();
                tree_queue.pop();
                if (temp->left) tree_queue.push(temp->left);
                if (temp->right) tree_queue.push(temp->right);
            }
            result.push_back(temp->val);
        } 
        return result;
    }
};
// @lc code=end

