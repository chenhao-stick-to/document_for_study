/*
 * @lc app=leetcode.cn id=232 lang=cpp
 *
 * [232] 用栈实现队列
 */

// @lc code=start
class MyQueue {
public:
    stack <int> stack_in;
    stack <int> stack_out;
    MyQueue() {
    }
    
    void push(int x) {
        stack_in.push(x);
    }
    
    int pop() {
        if (!stack_out.empty()) {
            int temp = stack_out.top();
            stack_out.pop();
            return temp;
        } else {
            while (!stack_in.empty()) {
                stack_out.push(stack_in.top());
                stack_in.pop();
            }
            return this->pop();
        }
    }
    
    int peek() {
        if (!stack_out.empty()) {
            return  stack_out.top();
        } else {
            while (!stack_in.empty()) {
                stack_out.push(stack_in.top());
                stack_in.pop();
            }
            return this->peek();
        }
    }
    
    bool empty() {
        if (stack_in.size() == 0 && stack_out.size() == 0) {
            return true;
        } else
            return false;
    }
};

/**
 * Your MyQueue object will be instantiated and called as such:
 * MyQueue* obj = new MyQueue();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->peek();
 * bool param_4 = obj->empty();
 */
// @lc code=end

