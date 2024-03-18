/*
 * @lc app=leetcode.cn id=102 lang=cpp
 *
 * [102] 二叉树的层序遍历
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
// class Solution {//非递归版本
// public:
//     vector<vector<int>> levelOrder(TreeNode* root) {//不一定是完全二叉树的层序遍历
//         vector <int> level_result;
//         vector <vector<int>> result;
//         queue <TreeNode*> tree_queue;
//         if (!root) return result;
//         tree_queue.push(root);
//         while (!tree_queue.empty()) {//保证每一次循环处理queue的所有元素即可。固定size记录一层的元素数量
//             int queue_size = tree_queue.size();//固定大小
//             for (int i = 0; i < queue_size; ++i) {
//                 TreeNode* temp = tree_queue.front();
//                 tree_queue.pop();
//                 level_result.emplace_back(temp->val);
//                 if (temp->left) tree_queue.push(temp->left);
//                 if (temp->right) tree_queue.push(temp->right);
//             }
//             result.push_back(level_result);
//             level_result.clear();
//         }
//         return result;
//     }
// };
class Solution {//递归版本的实现
public:
    void order(TreeNode* cur, vector<vector<int>>& result, int depth)
    {
        if (cur == nullptr) return;
        if (result.size() == depth) result.push_back(vector<int>());//防止深度访问时，下面索引无效。
        result[depth].push_back(cur->val);
        order(cur->left, result, depth + 1);
        order(cur->right, result, depth + 1);
    }
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        int depth = 0;
        order(root, result, depth);
        return result;
    }
};
// @lc code=end

