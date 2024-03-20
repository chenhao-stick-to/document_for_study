/*
 * @lc app=leetcode.cn id=513 lang=cpp
 *
 * [513] 找树左下角的值
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
class Solution {//可首先确定树高，再前序遍历确定最左边的!
public:
    int findBottomLeftValue(TreeNode* root) {
        if (!root) return 0;
        queue <TreeNode*> tree_queue;
        tree_queue.push(root);
        int temp = 0;
        while(!tree_queue.empty()) {
            int size = tree_queue.size();
            for (int i = 0; i < size; i++) {
                TreeNode* curr = tree_queue.front();
                tree_queue.pop();
                if (i == 0) {
                    temp = curr->val;
                }
                if (curr->left) tree_queue.push(curr->left);
                if (curr->right) tree_queue.push(curr->right);
               
            }
        }
        return temp;
    }
};
// @lc code=end

