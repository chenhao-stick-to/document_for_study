/*
 * @lc app=leetcode.cn id=94 lang=cpp
 *
 * [94] 二叉树的中序遍历
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
   //递归
    // void traversal(TreeNode* curr, vector <int>& result){//不用返回实际值，参数就是结果数组以及当前结点
    //     if (curr == nullptr) return ; //边界条件
    //     traversal(curr->left,result);
    //     result.push_back(curr->val);
    //     traversal(curr->right,result);
    // }
    // vector<int> inorderTraversal(TreeNode* root) {
    //     vector <int> result;
    //     traversal(root,result);
    //     return result;
    // }

    //非递归法，用栈
    //第一种写法
    // vector<int> inorderTraversal(TreeNode* root) {
    //     vector <int> result;
    //     stack <TreeNode*> stack_tree;
    //     if (root == nullptr) return result;
    //     stack_tree.push(root);
    //     int stack_tree_size = 1;
    //     while (!stack_tree.empty()) {
    //         TreeNode* curr = stack_tree.top();
    //         if (curr->left && stack_tree_size <= stack_tree.size()) {
    //             stack_tree_size = stack_tree.size();
    //             stack_tree.push(curr->left);
    //         } else {
    //             stack_tree_size = stack_tree.size();
    //             result.push_back(curr->val);
    //             stack_tree.pop();
    //             if (curr->right) {
    //                 stack_tree.push(curr->right);
    //             }
    //         }
            
    //     }
    //     return result;
    // }

    //第二种写法,最好学习这种
    // vector<int> inorderTraversal(TreeNode* root) {
    //     vector <int> result;
    //     stack <TreeNode*> stack_tree;
    //     TreeNode* curr = root;
    //     while (!stack_tree.empty() || curr) {
    //         if (curr) {
    //             stack_tree.push(curr);
    //             curr = curr->left;
    //         } else {
    //             curr = stack_tree.top();
    //             result.push_back(curr->val);
    //             stack_tree.pop();
    //             curr = curr->right;
    //         }
            
    //     }
    //     return result;
    // }

    //第三种方法，统一迭代的方法。//特殊方法是将处理元素和访问元素不做区分，给未处理元素加上标志
    vector<int> inorderTraversal(TreeNode* root) {
        vector <int> result;
        stack <TreeNode*> stack_tree;
        if (root != nullptr) stack_tree.push(root);
        TreeNode* curr = root;
        while (!stack_tree.empty()) {
            curr = stack_tree.top();
            if (curr) {//当前结点是否为空，标志着不同处理方法，不为空则标志着压栈操作
                stack_tree.pop();
                if (curr->right) stack_tree.push(curr->right);

                stack_tree.push(curr);
                stack_tree.push(nullptr);//标志该元素还未被处理,只有当前结点需要加入nullptr标志

                if (curr->left) stack_tree.push(curr->left);

            } else {//为空则标志着处理操作，压入元素到result
                stack_tree.pop();
                curr = stack_tree.top();
                stack_tree.pop();
                result.push_back(curr->val);
            }
        }
        return result;
    }
};
// @lc code=end

