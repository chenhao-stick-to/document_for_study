/*
 * @lc app=leetcode.cn id=226 lang=cpp
 *
 * [226] 翻转二叉树
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
    // TreeNode* invertTree(TreeNode* root) {//使用swap函数，递归法
    //     if (root == nullptr) return nullptr;
    //     TreeNode* temp;
    //     temp = invertTree(root->left);
    //     root->left = invertTree(root->right);
    //     root->right = temp;
    //     return root;
    // }
    
    // TreeNode* invertTree(TreeNode* root) {//使用swap函数，前序遍历，普通迭代
    //     if (root == nullptr) return nullptr;
    //     stack <TreeNode*> tree_stack;
    //     tree_stack.push(root);
    //     while(!tree_stack.empty()) {
    //         TreeNode* temp = tree_stack.top();
    //         tree_stack.pop();
    //         swap(temp->left,temp->right);
    //         if (temp->right) tree_stack.push(temp->right);
    //         if (temp->left) tree_stack.push(temp->left);
    //     }
    //     return root;
    // }

    // TreeNode* invertTree(TreeNode* root) {//使用swap函数，中序遍历，普通迭代
    //     if (root == nullptr) return nullptr;
    //     stack <TreeNode*> tree_stack;
    //     TreeNode* curr = root;
    //     while(!tree_stack.empty() || curr) {
    //         if (curr) {
    //             tree_stack.push(curr);
    //             swap(curr->left,curr->right);
    //             curr = curr->left;
    //         } else {
    //             curr = tree_stack.top();
    //             tree_stack.pop();
    //             curr = curr->right;//更新curr
    //         }  
    //     }
    //     return root;
    // }

    // TreeNode* invertTree(TreeNode* root) {//使用swap函数，中序遍历，普通迭代
    //     if (root == nullptr) return nullptr;
    //     stack <TreeNode*> tree_stack;
    //     TreeNode* curr = root;
    //     while(!tree_stack.empty() || curr) {
    //         if (curr) {
    //             tree_stack.push(curr);
    //             swap(curr->left,curr->right);
    //             curr = curr->left;
    //         } else {
    //             curr = tree_stack.top();
    //             tree_stack.pop();
    //             curr = curr->right;//更新curr
    //         }  
    //     }
    //     return root;
    // }

    TreeNode* invertTree(TreeNode* root) {//统一迭代法，中序遍历
        stack <TreeNode*> tree_stack;
        if (root) tree_stack.push(root);
        while(!tree_stack.empty()) {
            TreeNode* curr = tree_stack.top();
            if (curr) {
                tree_stack.pop();
                if(curr->right) tree_stack.push(curr->right);
                tree_stack.push(curr);
                tree_stack.push(nullptr);
                if (curr->left) tree_stack.push(curr->left);
            } else {
                tree_stack.pop();
                curr = tree_stack.top();
                tree_stack.pop();
                swap(curr->left,curr->right);
            }  
        }
        return root;
    }
};
// @lc code=end

