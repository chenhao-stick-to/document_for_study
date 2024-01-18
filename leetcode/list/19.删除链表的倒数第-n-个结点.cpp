/*
 * @lc app=leetcode.cn id=19 lang=cpp
 *
 * [19] 删除链表的倒数第 N 个结点
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
    ListNode* removeNthFromEnd(ListNode* head, int n) {//使用固定窗口法、
        if (head == nullptr) return nullptr;
        ListNode* headptr = new ListNode();
        headptr->next = head;
        ListNode* prev = headptr;
        ListNode* tail = headptr;
        int index = 0;
        while(tail->next != nullptr) {//确定prev，tail初始值
            index++;
            tail = tail->next;
            if(index == n)
                break;
        }
        if(index < n){
            return head;//没有这么多节点
        }
        while(tail->next != nullptr){
            tail=tail->next;
            prev = prev->next;
        }
        ListNode* tmp = prev->next;
        prev->next = prev->next->next;
        delete tmp;
        return headptr->next;
    }
};
// @lc code=end

