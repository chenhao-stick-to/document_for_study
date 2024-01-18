#include <vector>
#include <iostream>
using namespace std;
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
  };
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        if(head == nullptr | head->next == nullptr) 
            return head;
        ListNode* headptr = new ListNode();
        headptr->next = head;
        ListNode* prev = headptr;
        ListNode* current1 = head;
        ListNode* current2 = head->next;
        ListNode* curr =head;
        while(current1 != nullptr && current2 != nullptr) {
            ListNode* tmp = current2->next; 
            prev->next= current2;
            current2->next = current1;
            prev = current1;
            prev->next = tmp;
            if(tmp == nullptr || tmp->next == nullptr)
                break;
            else {
                current1 = tmp;
                current2 = tmp->next;
            }
        }
        return headptr->next;
    }
};
int main(){
    Solution s;
    ListNode* head = new ListNode(1);
   ListNode* head1 = new ListNode(2);
   ListNode* head2 = new ListNode(3);
   ListNode* head3 = new ListNode(4);
   head->next=head1;
   head1->next = head2;
   head2->next = head3;
   ListNode* result = s.swapPairs(head);
   ListNode* curr =result;
   while(curr!=nullptr){
    cout<<curr->val<<"  ";
    curr=curr->next;
   }
}