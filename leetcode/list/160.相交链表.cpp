/*
 * @lc app=leetcode.cn id=160 lang=cpp
 *
 * [160] 相交链表
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
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        ListNode* current = headA;
        while(current != nullptr){//对headA做标记
            current->val -= 2147483647; //注意int值范围为[-2147483648,2147483647],而链表值的范围是[1,10^5],进行唯一标识，则减去int最大值，肯定<=0.
            current =current->next;
        }
        current = headB;
        while(current != nullptr){//做标记
            if(current->val <= 0)
                break;
            current =current->next;
        }
        ListNode* current1 = headA;
        while(current1 != nullptr){//链表结构headA恢复
            current1->val += 2147483647; 
            current1 = current1->next;
        }
        return current;
    }
};
// @lc code=end

