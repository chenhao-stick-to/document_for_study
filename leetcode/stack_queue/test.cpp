#include <vector>
#include <iostream>
#include <cstring>
#include <stack>
#include <queue>
using namespace std;
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector <int> result;
        //双优先队列实现，最终版
        priority_queue <int> heap_s;//保留队列
        priority_queue <int> heap_d;//删除队列
        for (int i  = 0; i <= nums.size()-k; i++) {
            if ( i == 0) {//构建初始大顶堆
                for (int j = i; j < i + k; ++j)
                    heap_s.push(nums[j]);
                result.push_back(heap_s.top());
            } else {
                heap_s.push(nums[i+k-1]);//添加元素
                heap_d.push(nums[i-1]);//删除元素
                if (heap_s.top() == heap_d.top()) {//相等则需要删除该元素
                    while (!heap_d.empty() && heap_s.top() == heap_d.top()) {//不等，则找到heap_s保留的最大元素,注意为空情况，无法访问top().
                        heap_s.pop();
                        heap_d.pop();
                    }
                }
                result.push_back(heap_s.top());
            }
        }
        return result;
    }
};
int main(){
    Solution s;
    vector <int> temp = {9,10,9,-7,-4,-8,2,-6};
    vector <int> result;
    result=s.maxSlidingWindow(temp,5);
    for(int i = 0; i< result.size(); ++i) {
        cout<<result[i]<<" ";
    }
    return 0;
}