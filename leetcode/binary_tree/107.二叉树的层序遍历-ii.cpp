/*
 * @lc app=leetcode.cn id=107 lang=cpp
 *
 * [107] 二叉树的层序遍历 II
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
    vector<vector<int>> levelOrderBottom(TreeNode* root) {//直接进行翻转。
        vector <int> level_result;
        vector <vector<int>> result;
        queue <TreeNode*> tree_queue;
        if (!root) return result;
        tree_queue.push(root);
        while (!tree_queue.empty()) {//保证每一次循环处理queue的所有元素即可。固定size记录一层的元素数量
            int queue_size = tree_queue.size();//固定大小
            for (int i = 0; i < queue_size; ++i) {
                TreeNode* temp = tree_queue.front();
                tree_queue.pop();
                level_result.emplace_back(temp->val);
                if (temp->left) tree_queue.push(temp->left);
                if (temp->right) tree_queue.push(temp->right);
            }
            result.push_back(level_result);
            level_result.clear();
        }
        std::reverse(result.begin(),result.end());
        return result;
    }
};
// @lc code=end

