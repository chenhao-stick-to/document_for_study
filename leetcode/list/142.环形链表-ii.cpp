/*
 * @lc app=leetcode.cn id=142 lang=cpp
 *
 * [142] 环形链表 II
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode *detectCycle(ListNode *head) {//这里用到了快慢指针法，请注意这个用法的创新新，二刷必做。
        if(head == nullptr || head->next == nullptr) return nullptr;
        ListNode* fast = head;
        ListNode* slow = head;
        bool flag = true;
        while(fast != slow || flag){//找到相遇结点,第一次必进去
            fast = fast->next->next;
            slow = slow->next;
            flag = false;
            if(fast == nullptr || fast->next == nullptr){//next为null无环
                return nullptr;
            }
        }
        slow = head;
        while(slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }
        return slow;
    }
};
// @lc code=end

