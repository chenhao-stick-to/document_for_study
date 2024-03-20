/*
 * @lc app=leetcode.cn id=257 lang=cpp
 *
 * [257] 二叉树的所有路径
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
class Solution {//使用前序遍历！由于路径是先访问节点.
public:
    // void oridinary_access(TreeNode* root,vector<string>& result,vector<int>& path) {
    //     path.push_back(root->val);
    //     if (!root->left && !root->right) {//到了叶子节点
    //         string temp;
    //         for (int i = 0; i < path.size()-1; i++) {
    //             temp += to_string(path[i]) + "->";
    //         }
    //         temp += to_string(path[path.size()-1]);
    //         result.push_back(temp);
    //     }

    //     if (root->left) {
    //         oridinary_access(root->left,result,path);
    //         path.pop_back();
    //     }
    //     if (root->right) {
    //         oridinary_access(root->right,result,path);
    //         path.pop_back();
    //     }
    // }
    // vector<string> binaryTreePaths(TreeNode* root) {
    //     vector<string> result; //记载所有路径
    //     if (!root) return result;
    //     vector<int> path;//使用回溯算法，用到的
    //     oridinary_access(root,result,path);
    //     return result;
    // }

    //回溯，递归简单写法
    // void oridinary_access(TreeNode* root,vector<string>& result,string path) {//注意不适用string&,注意差别，回溯的精髓
    //     path += to_string(root->val);
    //     if (!root->left && !root->right) {//到了叶子节点
    //         result.push_back(path);
    //         return;
    //     }

    //     if (root->left) {
    //         oridinary_access(root->left,result,path+"->");
    //     }
    //     if (root->right) {
    //         oridinary_access(root->right,result,path+"->");
    //     }
    // }
    // vector<string> binaryTreePaths(TreeNode* root) {
    //     vector<string> result; //记载所有路径
    //     if (!root) return result;
    //     string path;//使用回溯算法，用到的
    //     oridinary_access(root,result,path);
    //     return result;
    // }

    vector<string> binaryTreePaths(TreeNode* root) {//使用栈来模拟
        vector<string> result; //记载所有路径
        if (!root) return result; 

        stack <TreeNode*> tree_stack;
        stack <string> tree_path;//保留每个节点根节点到该节点的对应的路径，这个和上面的节点对应，只是保留对应节点的路径
        tree_stack.push(root);
        tree_path.push(to_string(root->val));

        while(!tree_stack.empty()) {
            TreeNode* curr =  tree_stack.top();
            tree_stack.pop();
            string path = tree_path.top();
            tree_path.pop();
            if (!curr->left && !curr->right) {//属于叶子节点
                result.push_back(path);
            }
            if (curr->left) {
                tree_path.push(path + "->"+to_string(curr->left->val));
                tree_stack.push(curr->left);
            }
            if (curr->right) {
                tree_path.push(path + "->" +to_string(curr->right->val));
                tree_stack.push(curr->right);
            }
        }
        return result;
    }
};
// @lc code=end

