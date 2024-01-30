/*
 * @lc app=leetcode.cn id=347 lang=cpp
 *
 * [347] 前 K 个高频元素
 */

// @lc code=start
class Solution {//考虑时间复杂度需要小于nlogn，进行<次数，数>的次数排序不可行。/堆的实现。
public:
    struct Compare {
    bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) const {
        return a.first> b.first;//小顶堆
    }
    };
    vector<int> topKFrequent(vector<int>& nums, int k) {
        vector <int> result;
        unordered_map <int,int> hash_fre;//
        // priority_queue <int> k_direc;//记录次数的堆
       std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, Compare>  k_direc_min;
    //记录次数的堆，小顶堆
        for (int i = 0; i < nums.size(); i++) {
            hash_fre[nums[i]]++;
        }
        //大顶堆的实现，但是呢大顶堆也需要nlogn的时间复杂度；为啥不考虑小顶堆，从小往大排序，最大为k个值。每次弹出最小的，最后就是最大的k个值。
        // for(auto it = hash_fre.begin(); it != hash_fre.end(); it++) {
        //     k_direc.push(it->second);
        // }
        // int i = 0;
        // while (i < k-1) {
        //     k_direc.pop();
        //     i++;
        // }
        // int k_num = k_direc.top();
        // k_direc.pop();
        // for(auto it = hash_fre.begin(); it != hash_fre.end(); it++) {
        //     if (it->second >= k_num) 
        //         result.push_back(it->first);
        // }

        //小顶堆的实现
        for(auto it = hash_fre.begin(); it != hash_fre.end(); it++) {
            if (k_direc_min.size() != k) {
                k_direc_min.push(std::make_pair(it->second,it->first));
            } else {
                if (k_direc_min.top().first < it->second) {
                    k_direc_min.pop();
                    k_direc_min.push(std::make_pair(it->second,it->first));
                }
            }
            
        }
        while (!k_direc_min.empty()) {
            result.push_back(k_direc_min.top().second);
            k_direc_min.pop();
        }
        return result;
    }
};
// @lc code=end

