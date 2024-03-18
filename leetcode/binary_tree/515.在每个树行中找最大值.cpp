/*
 * @lc app=leetcode.cn id=515 lang=cpp
 *
 * [515] 在每个树行中找最大值
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
    vector<int> largestValues(TreeNode* root) {
        vector <int> result;
        queue <TreeNode*> tree_queue;
        if (!root) return result;
        tree_queue.push(root);
        while (!tree_queue.empty()) {//保证每一次循环处理queue的所有元素即可。固定size记录一层的元素数量
            int queue_size = tree_queue.size();//固定大小
            int max = -pow(2,31);
            for (int i = 0; i < queue_size; ++i) {
                TreeNode* temp = tree_queue.front();
                tree_queue.pop();
                max = max > temp->val ? max : temp->val;
                if (temp->left) tree_queue.push(temp->left);
                if (temp->right) tree_queue.push(temp->right);
            }
            result.push_back(max);
        }
        return result;
    }
};
// @lc code=end

