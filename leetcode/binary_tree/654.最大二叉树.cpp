/*
 * @lc app=leetcode.cn id=654 lang=cpp
 *
 * [654] 最大二叉树
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
    TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
        if (nums.size() == 0) return nullptr;
        auto max_it = std::max_element(nums.begin(),nums.end());//不用判断有没有找到，必找到
        TreeNode* root = new TreeNode(*max_it);
        vector <int> left_nums(nums.begin(),max_it);
        vector <int> right_nums(max_it+1,nums.end());
        root->left = constructMaximumBinaryTree(left_nums);
        root->right = constructMaximumBinaryTree(right_nums);
        return root;
    }
};
// @lc code=end

