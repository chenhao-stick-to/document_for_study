/*
 * @lc app=leetcode.cn id=101 lang=cpp
 *
 * [101] 对称二叉树
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
class Solution {//!其实本质可以考虑递归的方法以及遍历的方法，使用栈，队列进行处理
public:
    TreeNode* acess(TreeNode* root,vector<int>& temp) {//翻转二叉树
        if (!root) {
            temp.push_back(101);
            return nullptr;
        }
        temp.push_back(root->val);
        root->left = acess(root->left, temp);
        root->right = acess(root->right, temp);
        return root;
    }

    TreeNode* invert(TreeNode* root) {//翻转二叉树
        if (root == nullptr) return nullptr;
        root->left = invert(root->left);
        root->right = invert(root->right);
        swap(root->left,root->right);
        return root;
    }
    bool isSymmetric(TreeNode* root) {//思考翻转二叉树，翻转二叉树和原来相同就是对称二叉树
        vector <int> direc1;//记录翻转前的记录值
        vector <int> direc2;//记录翻转后的记录值
        acess(root,direc1);
        root = invert(root);//进行翻转二叉树
        acess(root,direc2);
        bool flag = true;
        for(int i = 0; i < direc1.size(); ++i){
            if(direc1[i] != direc2[i]) {
                flag = false;
                break;
            }
        }
        return flag;
    }
};
// @lc code=end

