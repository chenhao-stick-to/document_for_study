/*
 * @lc app=leetcode.cn id=707 lang=cpp
 *
 * [707] 设计链表
 */

// @lc code=start
struct List_Node{//再一种方法
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
                    current->prev->next = nullptr;//注意边界条件,一定要考虑指针的有效性。删除指针时，一定要把所有链接该指针的都置为nullptr
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

/**
 * Your MyLinkedList object will be instantiated and called as such:
 * MyLinkedList* obj = new MyLinkedList();
 * int param_1 = obj->get(index);
 * obj->addAtHead(val);
 * obj->addAtTail(val);
 * obj->addAtIndex(index,val);
 * obj->deleteAtIndex(index);
 */
// @lc code=end

