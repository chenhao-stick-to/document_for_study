/*
 * @lc app=leetcode.cn id=144 lang=cpp
 *
 * [144] 二叉树的前序遍历
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
    //递归法
    // void traversal(TreeNode* curr, vector <int>& result){//不用返回实际值，参数就是结果数组以及当前结点
    //     if (curr == nullptr) return ; //边界条件
    //     result.push_back(curr->val);
    //     traversal(curr->left,result);
    //     traversal(curr->right,result);
    // }
    // vector<int> preorderTraversal(TreeNode* root) {
    //     vector <int> result;
    //     traversal(root,result);
    //     return result;
    // }

    //非递归法，直接用栈。请注意入栈顺序。
    // vector<int> preorderTraversal(TreeNode* root) {
    //     vector <int> result;
    //     stack <TreeNode*> stack_tree;
    //     if (root == nullptr) return result;
    //     stack_tree.push(root);
    //     while (!stack_tree.empty()) {
    //         TreeNode* temp = stack_tree.top();//中间结点
    //         result.push_back(temp->val);//保存这个结果
    //         stack_tree.pop();
    //         if (temp->right != nullptr) {//栈顶结点有右孩子，则出栈并且入栈右孩子。
    //            stack_tree.push(temp->right);
    //         }
    //         if (temp->left != nullptr) {
    //             stack_tree.push(temp->left);
    //         }
    //     }  
    //     return result;
    // }

    //统一迭代法，
    vector<int> preorderTraversal(TreeNode* root) {//注意对首先处理的元素加入nullptr的标志！
        vector <int> result;
        stack <TreeNode*> stack_tree;
        if (root != nullptr) stack_tree.push(root);
        TreeNode* curr = root;
        while (!stack_tree.empty()) {
            curr = stack_tree.top();
            if (curr) {//当前结点是否为空，标志着不同处理方法，不为空则标志着压栈操作
                stack_tree.pop();
                if (curr->right) stack_tree.push(curr->right);
                if (curr->left) stack_tree.push(curr->left);
                
                stack_tree.push(curr);
                stack_tree.push(nullptr);//标志该元素还未被处理,只有当前结点需要加入nullptr标志

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

