/*
 * @lc app=leetcode.cn id=404 lang=cpp
 *
 * [404] 左叶子之和
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
//递归模式
    // int compute_left(TreeNode* root,bool flag) {
    //     int sum = 0;
    //     if (!root->left && !root->right) {
    //         int left_sum = flag ? root->val : 0;
    //         return left_sum;
    //     }
    //     if (root->left) sum += compute_left(root->left,true);
    //     if (root->right) sum += compute_left(root->right,false);
    //     return sum;
    // }
    // int sumOfLeftLeaves(TreeNode* root) {
    //     if (!root) return 0;
    //     return compute_left(root,false);
    // }
    
    //层序遍历模式
    int sumOfLeftLeaves(TreeNode* root) {
        if (!root) return 0;
        int sum = 0;
        queue <TreeNode*> tree_queue;
        tree_queue.push(root);
        while(!tree_queue.empty()) {
            int size = tree_queue.size();
            for (int i = 0; i < size; i++) {
                TreeNode* curr = tree_queue.front();
                tree_queue.pop();
                if (curr->left && !curr->left->left && !curr->left->right) {//只能在父节点判断其是否为左叶子节点！
                    sum += curr->left->val;
                }
                if (curr->left) tree_queue.push(curr->left);
                if (curr->right) tree_queue.push(curr->right);
               
            }
        }
        return sum;
    }
};
// @lc code=end

