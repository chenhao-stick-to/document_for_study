/*
 * @lc app=leetcode.cn id=637 lang=cpp
 *
 * [637] 二叉树的层平均值
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
    vector<double> averageOfLevels(TreeNode* root) {
        queue <TreeNode*> tree_queue;
        vector <double> result;
        if (!root) return result;
        tree_queue.push(root);
        while (!tree_queue.empty()) {
            int size = tree_queue.size();
            TreeNode* temp = nullptr;
            double level_sum = 0;
            for (int i = 0; i < size; i++) {
                temp = tree_queue.front();
                tree_queue.pop();
                level_sum += temp->val;
                if (temp->left) tree_queue.push(temp->left);
                if (temp->right) tree_queue.push(temp->right);
            }
            result.push_back(level_sum/(1.0*size));
        } 
        return result;
    }
};
// @lc code=end

