#include <vector>
#include <iostream>
using namespace std;
struct ListNode {
     int val;
     ListNode *next;
     ListNode(int x) : val(x), next(NULL) {}
};
class Solution {
public:
    ListNode *detectCycle(ListNode *head) {//这里用到了快慢指针法，请注意这个用法的创新新，二刷必做。
        if(head == nullptr || head->next == nullptr) return nullptr;
        ListNode* fast = head;
        ListNode* slow = head;
        while(fast != slow || slow == head){//找到相遇结点h
            fast = fast->next->next;
            slow = slow->next;
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
int main(){
    ListNode* head = new ListNode(1);
    ListNode* head1 = new ListNode(2);
    ListNode* head2 = new ListNode(3);
    ListNode* head3 = new ListNode(4);
    head->next = head1;
    head1->next = head2;
    head2->next = head3;
    head3->next = head1;
    Solution s;
    ListNode * tmp = s.detectCycle(head);
    cout<<tmp->val;
    
}