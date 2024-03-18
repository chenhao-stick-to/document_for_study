/*
 * @lc app=leetcode.cn id=239 lang=cpp
 *
 * [239] 滑动窗口最大值
 */

// @lc code=start
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {//单调队列的实现！。双优先队列实现也可！
        vector <int> result;
        // if (k > nums.size()) {
        //     return result;
        // }
        // int max = 0;
        // for (int i = 0; i < k; ++i) {
        //     if (nums[i] > max)
        //         max = nums[i];
        // }
        // result.push_back(max);
        // for (int i  = 1; i <= nums.size()-k; i++) {
        //     if (nums[i+k-1] > max)
        //         max = nums[i+k-1];
        //     result.push_back(max);
        // }

        //k的大顶堆
        // if (k > nums.size()) {
        //     return result;
        // }
        // int max = 0;
        // priority_queue <int> heap;
        // for (int i = 0; i < k; ++i) {
        //    heap.push(nums[i]);
        // }
        // result.push_back(heap[0]);
        // auto it = std::find(heap.begin(), heap.end(), nums[0]); // 寻找要移除的元素
        // if (it != heap.end()) {
        //     swap(*it, heap.back()); // 将要移除的元素与堆的最后一个元素交换
        //     heap.pop_back(); // 移除最后一个元素
        // }

        //优先队列实现，单。 
        // int max;
        // priority_queue <int> heap;
        // for (int i  = 0; i <= nums.size()-k; i++) {
        //     if (i == 0 || max == nums[i-1] || nums[i+k-1] >= max) {//最大值发生改变重新构建heap
        //         if (nums[i+k-1] >= max && i > 0){//不需要重新构造heap
        //             heap.push(nums[i+k-1]);
        //         } else {
        //             while (!heap.empty()) {//清空heap
        //                 heap.pop();
        //             }
        //             for (int j = i; j < i + k; ++j)
        //                 heap.push(nums[j]);
        //         }
        //         max = heap.top();
        //     } 
        //     result.push_back(max);
        // }
        // return result;

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
// @lc code=end

