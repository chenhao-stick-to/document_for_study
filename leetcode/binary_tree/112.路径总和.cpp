/*
 * @lc app=leetcode.cn id=112 lang=cpp
 *
 * [112] 路径总和
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
    bool hasPathSum(TreeNode* root, int targetSum) {
        if (!root) return false;
        stack <int> tree_node;
        stack <TreeNode*> tree_stack;
        tree_stack.push(root);
        tree_node.push(root->val);
        while(!tree_stack.empty()) {
            TreeNode* curr = tree_stack.top();
            tree_stack.pop();
            int temp = tree_node.top();
            tree_node.pop();
            if(!curr->left && !curr->right) {//叶子节点
                if (targetSum == temp) return true;
            }
            if (curr->left) {
                temp += curr->left->val;
                tree_node.push(temp);
                tree_stack.push(curr->left);
                temp -= curr->left->val;//注意这个值的回溯
            }
            if (curr->right) {
                temp += curr->right->val;
                tree_node.push(temp);
                tree_stack.push(curr->right);
            }
        }
        return false;
    }
};
// @lc code=end

