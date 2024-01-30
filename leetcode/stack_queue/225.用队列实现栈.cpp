/*
 * @lc app=leetcode.cn id=225 lang=cpp
 *
 * [225] 用队列实现栈
 */

// @lc code=start
class MyStack {//可以使用一个或两个队列实现，这里只使用一个队列
public:
    queue <int> queue_temp;
    MyStack() {
    }
    
    void push(int x) {
        queue_temp.push(x);
    }
    
    int pop() {
        int i = 0;
        while (i < queue_temp.size()) {
            int temp = queue_temp.front();
            queue_temp.pop();
            if (i == queue_temp.size())
                return temp;
            queue_temp.push(temp);
            i++;
        }
        return 0;
    }
    
    int top() {
        return queue_temp.back();
    }
    
    bool empty() {
        return queue_temp.empty();
    }
};

/**
 * Your MyStack object will be instantiated and called as such:
 * MyStack* obj = new MyStack();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->top();
 * bool param_4 = obj->empty();
 */
// @lc code=end

