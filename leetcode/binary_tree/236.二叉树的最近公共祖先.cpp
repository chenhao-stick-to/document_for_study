/*
 * @lc app=leetcode.cn id=236 lang=cpp
 *
 * [236] 二叉树的最近公共祖先
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {//采用后序遍历。
public:
    // TreeNode* node_father(TreeNode* root, TreeNode* p, TreeNode* q,bool& flag,bool& flag_in) {
    //     if (p == root || q == root) flag_in = true;
    //     if (!root->left && !root->right) {
    //         return nullptr;
    //     }
    //     bool flag1 = flag;
    //     bool flag2 = flag;
    //     TreeNode* left = nullptr;
    //     TreeNode* right = nullptr;
    //     if (root->left) {
    //         left = node_father(root->left,p,q,flag1,flag_in);
    //     }
    //     if (root->right) {
    //         right = node_father(root->right,p,q,flag2,flag_in);
    //     }
    //     flag = flag1 | flag2;
    //     if (flag) {
    //         if (left) return left;
    //         else return right;
    //     } else {
    //         if ((p == root && (q == root->left || q == root->right)) || q == root && (p == root->left || p == root->right) || (p == root && flag_in) || (q == root && flag_in)) {
    //             flag = true;
    //             return root;
    //         } else {
    //             if (p == root || q == root) flag_in = true; 
    //             return nullptr;
    //         }
    //     }

    // }
    // TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    //     bool flag1 = false;
    //     bool flag2 = false;
    //     bool flag_in = false;
    //     TreeNode* left = nullptr;
    //     TreeNode* right = nullptr;
    //     if (root->left) {
    //         left = node_father(root->left,p,q,flag1,flag_in);
    //     }
    //     if (root->right) {
    //         right = node_father(root->right,p,q,flag2,flag_in);
    //     }
    //     if (!flag1 && !flag2) return root;
    //     else if (flag1) return left;
    //     else return right;
    // }

    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (root == p || root == q || !root) return root;
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
        if (root->left) {
            left = lowestCommonAncestor(root->left,p,q);
        }
        if (root->right) {
            right = lowestCommonAncestor(root->right,p,q);
        }
        if (left && right) {
            return root;
        }
        if (!left) {
            return right;
        }
        return left;

        
        
    }
};
// @lc code=end

