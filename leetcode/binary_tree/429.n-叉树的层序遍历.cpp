/*
 * @lc app=leetcode.cn id=429 lang=cpp
 *
 * [429] N 叉树的层序遍历
 */

// @lc code=start
/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> children;

    Node() {}

    Node(int _val) {
        val = _val;
    }

    Node(int _val, vector<Node*> _children) {
        val = _val;
        children = _children;
    }
};
*/

class Solution {
public:
    vector<vector<int>> levelOrder(Node* root) {
        queue <Node*> tree_queue;
        vector <vector<int>> result;
        if (!root) return result;
        tree_queue.push(root);
        while (!tree_queue.empty()) {
            vector <int> level_result;
            int size = tree_queue.size();
            Node* temp = nullptr;
            for (int i = 0; i < size; i++) {
                temp = tree_queue.front();
                tree_queue.pop();
                level_result.push_back(temp->val);
                for (auto t : temp->children) {
                    if (t) 
                        tree_queue.push(t);
                       

                }
            }
            result.push_back(level_result);
        } 
        return result;
    }
};
// @lc code=end

