/*
 * @lc app=leetcode.cn id=530 lang=cpp
 *
 * [530] 二叉搜索树的最小绝对差
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
class Solution {//可以使用中序遍历得到数组，再比较，这里使用其他方法。
public:
    // void searchBST(TreeNode* root,vector<int>& result) {
    //     if (!root) {
    //         return;
    //     }
    //     if (root->left) {
    //         searchBST(root->left,result);
    //     }
    //     result.push_back(root->val);
    //     if (root->right) {
    //         searchBST(root->right,result);
    //     }
    //     return;
    // }

    // int getMinimumDifference(TreeNode* root) {//递归法
    //     vector<int> result;
    //     searchBST(root,result); 
    //     int min = 100001;
    //     for(int i = 0; i < result.size()-1; i++) {
    //         min = min < (result[i+1]-result[i]) ? min : (result[i+1]-result[i]);
    //     }
    //     return min;
    // }

    int getMinimumDifference(TreeNode* root) {//迭代法,pre存储BST curr节点上一个值！避免最后再计算。
        stack <TreeNode*> tree_stack;
        TreeNode* curr = root;
        TreeNode* pre = nullptr;
        int result = 100001;
        while (!tree_stack.empty() || curr) {
            if (curr) {
                tree_stack.push(curr);
                curr = curr->left;
            } else {
                curr = tree_stack.top();
                tree_stack.pop();
                if (pre) {
                    result = result < (curr->val-pre->val) ? result : (curr->val-pre->val);
                }
                pre = curr;//没有左节点，curr节点，所以该节点设置为该子树的pre
                curr = curr->right;
            }
        }
        return result;
    }
};
// @lc code=end

