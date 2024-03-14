/*
 * @lc app=leetcode.cn id=203 lang=cpp
 *
 * [203] 移除链表元素
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {//创建头节点，一视同仁。注意释放头节点，一视同仁！
        ListNode* dummy = new ListNode(0); 
        dummy->next = head;
        ListNode* current = dummy; 
        while(current->next != NULL) {
            if (current->next->val == val){
                ListNode* tmp = current->next;
                current->next = tmp->next;
                delete tmp;
            } else {
                current = current->next;
            }
        }
        return dummy->next;
    }
};
// @lc code=end

