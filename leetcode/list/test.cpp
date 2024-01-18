#include <vector>
#include <iostream>
using namespace std;
struct List_Node{
        List_Node(){}
        List_Node(int val){this->val = val;}
        List_Node* prev = nullptr;
        List_Node* next = nullptr;
        int val;
        int index;
};
class MyLinkedList {
public:
    List_Node* my_list= new List_Node();//创建头节点
    MyLinkedList() {
        my_list->prev = nullptr;
        my_list->next = nullptr;
        my_list->val = 0;
        my_list->index = -1;
    }
    
    int get(int index) {
        List_Node* current = my_list->next;
        while(current!=nullptr){
            if(current->index == index){
                return current->val;
            }
            current = current->next;
        }
        return -1;
    }
    
    void addAtHead(int val) {
        List_Node* tmp = new List_Node(val);
        if(my_list->next == nullptr){
            tmp->index = 0;
            my_list->next = tmp;
            tmp->prev = my_list;
        } else {
            tmp->next = my_list->next;
            my_list->next->prev = tmp;
            tmp->prev = my_list;
            my_list->next = tmp;
            List_Node* current = my_list->next;
            int index = 0;
            while(current!=nullptr){
                current->index = index++;
                current = current->next;
            }
        }
    }
    
    void addAtTail(int val) {
        List_Node* tail = my_list;
        while(tail->next != nullptr) {
            tail = tail->next;
        }
        List_Node* tmp = new List_Node(val);
        tmp->index = tail->index + 1;
        tmp->prev = tail;
        tail->next = tmp;
    }
    
    void addAtIndex(int index, int val) {
        int length = 0;
        List_Node* current = my_list->next;
        while(current != nullptr){
            if(current->index == index) {
                    List_Node* tmp = new List_Node(val);
                    tmp->index = current->index;
                    List_Node* prev_node = current->prev;
                    prev_node->next = tmp;
                    tmp->prev = prev_node;
                    current->prev = tmp;
                    tmp->next = current;
                    while(current != nullptr){
                        current->index += 1;
                        current = current->next;
                    }
                    break;
                }
            current = current->next;
        }
        current = my_list->next;
        while(current != nullptr){
            current = current->next;
            length++;
        }
        if(length == index)
            addAtTail(val);
    }
    
    void deleteAtIndex(int index) {   
        List_Node* current = my_list->next;
        while(current != nullptr) {
            if(index == current->index){
                if(current->next == nullptr){//只有一个元素
                    current->prev->next = nullptr;
                    delete current;
                    break;
                } else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                    List_Node* tmp = current->next;
                    delete current;
                    while(tmp != nullptr) {
                        tmp->index -= 1;
                        tmp = tmp->next;
                        }
                    break;
                }
            }
            current = current->next;
        }
    }
};
int main(){
    MyLinkedList* myLinkedList = new MyLinkedList();
    myLinkedList->addAtHead(38);
    myLinkedList->addAtTail(66);
    myLinkedList->addAtTail(61);
    myLinkedList->addAtTail(76);
    myLinkedList->addAtTail(26);
    myLinkedList->addAtTail(37);
    myLinkedList->addAtTail(8);
    myLinkedList->deleteAtIndex(5);
    myLinkedList->addAtHead(4);
    myLinkedList->addAtHead(45);
    myLinkedList->get(4);
    myLinkedList->addAtTail(85);
    myLinkedList->addAtHead(37);
    myLinkedList->get(5);
    myLinkedList->addAtTail(93);
    myLinkedList->addAtIndex(10,23);
    myLinkedList->addAtTail(21);
    myLinkedList->addAtHead(52);
    myLinkedList->addAtHead(15);
    myLinkedList->addAtHead(47);
    myLinkedList->get(12);
    myLinkedList->addAtIndex(6,24);
    myLinkedList->addAtHead(64);
    myLinkedList->get(4);
    myLinkedList->addAtHead(31);
    myLinkedList->deleteAtIndex(6);
    myLinkedList->addAtHead(40);
    myLinkedList->addAtTail(17);
    myLinkedList->addAtTail(15);
    myLinkedList->addAtIndex(19,2);
    myLinkedList->addAtTail(11);
    myLinkedList->addAtHead(86);
    myLinkedList->get(17);
    myLinkedList->addAtTail(55);
    myLinkedList->deleteAtIndex(15);
    myLinkedList->addAtIndex(14,95);
    myLinkedList->deleteAtIndex(22);
    myLinkedList->addAtHead(66);
    myLinkedList->addAtTail(95);
    myLinkedList->addAtHead(8);
    myLinkedList->addAtHead(47);
    myLinkedList->addAtTail(23);
    myLinkedList->addAtTail(39);
    myLinkedList->get(30);
    myLinkedList->get(27);
    myLinkedList->addAtHead(0);
    myLinkedList->addAtTail(99);
    myLinkedList->addAtTail(45);
    myLinkedList->addAtTail(4);
    myLinkedList->addAtIndex(9,11);
    myLinkedList->get(6);
    myLinkedList->addAtHead(81);
    myLinkedList->addAtIndex(18,32);
     myLinkedList->addAtHead(20);
    myLinkedList->addAtTail(13);
    myLinkedList->addAtTail(42);
    myLinkedList->addAtIndex(37,91);
    myLinkedList->deleteAtIndex(36);
    myLinkedList->addAtIndex(10,37);
    myLinkedList->addAtHead(96);
    myLinkedList->addAtHead(57);
    myLinkedList->deleteAtIndex(20);
    myLinkedList->addAtTail(89);
    myLinkedList->deleteAtIndex(18);
    myLinkedList->addAtIndex(41,5);
    myLinkedList->addAtTail(23);
    myLinkedList->addAtHead(75);
    myLinkedList->get(7);
    myLinkedList->addAtIndex(25,51);
    myLinkedList->addAtTail(48);
    myLinkedList->addAtHead(46);
    myLinkedList->addAtHead(29);
    myLinkedList->addAtHead(85);
    myLinkedList->addAtHead(82);
    myLinkedList->addAtHead(6);
    myLinkedList->addAtHead(38);
    myLinkedList->deleteAtIndex(14);
    myLinkedList->get(1);
    myLinkedList->get(12);
    myLinkedList->addAtHead(42);
    myLinkedList->get(42);
    myLinkedList->addAtTail(83);
    myLinkedList->addAtTail(13);
    myLinkedList->addAtIndex(14,30);
    myLinkedList->addAtIndex(17,34);
    myLinkedList->addAtHead(36);
    myLinkedList->addAtTail(58);
    myLinkedList->addAtTail(2);
    myLinkedList->get(38);
    myLinkedList->addAtIndex(33,59);
    myLinkedList->addAtHead(37);
    myLinkedList->deleteAtIndex(15);
    myLinkedList->addAtTail(64);
    myLinkedList->get(56);
    List_Node * tmp = myLinkedList->my_list->next;
    while(tmp != nullptr){
        cout<<tmp->index <<" "<<tmp->val<<endl;
        tmp=tmp->next;
    }
    cout<<endl;
    myLinkedList->addAtHead(0);
    tmp = myLinkedList->my_list->next;
    while(tmp != nullptr){
        cout<<tmp->index <<" "<<tmp->val<<endl;
        tmp=tmp->next;
    }
    cout<<endl;
    myLinkedList->get(40);
    tmp = myLinkedList->my_list->next;
    while(tmp != nullptr){
        cout<<tmp->index <<" "<<tmp->val<<endl;
        tmp=tmp->next;
    }
    cout<<endl;
    myLinkedList->addAtHead(92);
    tmp = myLinkedList->my_list->next;
    while(tmp != nullptr){
        cout<<tmp->index <<" "<<tmp->val<<endl;
        tmp=tmp->next;
    }
    cout<<endl;
    myLinkedList->deleteAtIndex(63);
    tmp = myLinkedList->my_list->next;
    while(tmp != nullptr){
        cout<<tmp->index <<" "<<tmp->val<<endl;
        tmp=tmp->next;
    }
    cout<<endl;
    myLinkedList->get(35);
    tmp = myLinkedList->my_list->next;
    while(tmp != nullptr){
        cout<<tmp->index <<" "<<tmp->val<<endl;
        tmp=tmp->next;
    }
    cout<<endl;
    myLinkedList->addAtTail(62);
    tmp = myLinkedList->my_list->next;
    while(tmp != nullptr){
        cout<<tmp->index <<" "<<tmp->val<<endl;
        tmp=tmp->next;
    }
    cout<<endl;
    myLinkedList->addAtTail(32);
    

    tmp = myLinkedList->my_list->next;
    while(tmp != nullptr){
        cout<<tmp->index <<" "<<tmp->val<<endl;
        tmp=tmp->next;
    }
    cout<<endl;
    // myLinkedList->addAtTail(3);
    // tmp = myLinkedList->my_list->next;
    // while(tmp != nullptr){
    //     cout<<tmp->index <<" "<<tmp->val<<endl;
    //     tmp=tmp->next;
    // }
    // cout<<endl;
    // myLinkedList->addAtIndex(1, 2); 
    // tmp = myLinkedList->my_list->next;
    // while(tmp != nullptr){
    //     cout<<tmp->index <<" "<<tmp->val<<endl;
    //     tmp=tmp->next;
    // }
    // cout<<endl;
    // int x = myLinkedList->get(1); 
    // myLinkedList->deleteAtIndex(0);
    // tmp = myLinkedList->my_list->next;
    // while(tmp != nullptr){
    //     cout<<tmp->index <<" "<<tmp->val<<endl;
    //     tmp=tmp->next;
    // }
    // cout<<endl;
    // x = myLinkedList->get(1); 
    return 0;
}