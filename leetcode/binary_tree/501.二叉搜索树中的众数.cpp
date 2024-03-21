/*
 * @lc app=leetcode.cn id=501 lang=cpp
 *
 * [501] 二叉搜索树中的众数
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
    vector<int> findMode(TreeNode* root) {
        stack <TreeNode*> tree_stack;
        TreeNode* curr = root;
        TreeNode* pre = nullptr;
        vector <int> result;
        if (!root) return result;
        int count = 0;
        int max = 0;
        while (!tree_stack.empty() || curr) {
            if (curr) {
                tree_stack.push(curr);
                curr = curr->left;
            } else {
                curr = tree_stack.top();
                tree_stack.pop();
                if (!pre) { // 第一个节点
                    count = 1;
                } else if (pre->val == curr->val) { // 与前一个节点数值相同
                    count++;
                } else { // 与前一个节点数值不同
                    count = 1;
                }
                if (count == max) { // 如果和最大值相同，放进result中
                    result.push_back(curr->val);
                }
                if (count > max) { // 如果计数大于最大值频率
                    max = count;   // 更新最大频率
                    result.clear();     // 很关键的一步，不要忘记清空result，之前result里的元素都失效了
                    result.push_back(curr->val);
                }

                pre = curr;//没有左节点，curr节点，所以该节点设置为该子树的pre
                curr = curr->right;
            }
        }
        return result;
    }
};
// @lc code=end

