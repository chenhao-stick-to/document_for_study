/*
 * @lc app=leetcode.cn id=206 lang=cpp
 *
 * [206] 反转链表
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
    // ListNode* reverseList(ListNode* head) {//双指针法
    //     ListNode* current = head;
    //     ListNode* prev = nullptr;
    //     while(current != nullptr) {
    //         ListNode* tmp = current->next;
    //         current->next = prev;
    //         prev = current;
    //         current = tmp;
    //     }
    //     return prev;

    // }
    ListNode* reverseList(ListNode* head) {//双指针法，正向递归和反向递归；正向递归和双指针类似，这里写一个反向递归
        if(head == nullptr) return nullptr;
        if(head->next == nullptr) return head;
        ListNode* last = reverseList(head->next);//精髓，递归到最深层保留翻转后的头指针
        head->next->next = head;
        head->next = nullptr;
        return last;
    }
};
// @lc code=end

