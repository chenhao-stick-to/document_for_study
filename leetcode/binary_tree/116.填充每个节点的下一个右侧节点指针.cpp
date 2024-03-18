/*
 * @lc app=leetcode.cn id=116 lang=cpp
 *
 * [116] 填充每个节点的下一个右侧节点指针
 */

// @lc code=start
/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node() : val(0), left(NULL), right(NULL), next(NULL) {}

    Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}

    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};
*/

class Solution {
public:
    Node* connect(Node* root) {
        queue <Node*> tree_queue;
        if (!root) return root;
        tree_queue.push(root);
        while (!tree_queue.empty()) {//保证每一次循环处理queue的所有元素即可。固定size记录一层的元素数量
            int queue_size = tree_queue.size();//固定大小
            Node* temp_pre = tree_queue.front();
            tree_queue.pop();
            if (temp_pre->left) tree_queue.push(temp_pre->left);
            if (temp_pre->right) tree_queue.push(temp_pre->right);
            if (queue_size == 1) {
                temp_pre->next = nullptr;
                continue;
            }
            for (int i = 0; i < queue_size-1; ++i) {
                Node* temp = tree_queue.front();
                tree_queue.pop();
                temp_pre->next = temp;
                temp_pre = temp;
                if (temp->left) tree_queue.push(temp->left);
                if (temp->right) tree_queue.push(temp->right);
                if (i == queue_size-2) {
                    temp->next = nullptr;
                }
            }
        }
        return root;
    }
};
// @lc code=end

