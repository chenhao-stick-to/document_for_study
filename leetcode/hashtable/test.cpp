#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;
// class Solution {
// public:
//     vector <int> exists;
//     bool isHappy(int n) {
//         int temp = n;
//         vector <int> bits; 
//         for (int i = 9; i >= 0; i--) {
//             if (pow(10,i) > temp){
//                 bits.push_back(0);
//                 continue;
//             }
//             bits.push_back(temp/pow(10,i));
//             temp = temp - (int)(temp/pow(10,i))*pow(10,i);
//         }
//         temp = 0;
//         for (int j = 0; j < bits.size(); j++) {
//             temp += bits[j]*bits[j];
//         }
//         if (temp == 1) {
//             return true;
//         } 
//         else {
//             if (find(exists.begin(),exists.end(),n) != exists.end())
//                 return false;
//             exists.push_back(n);
//             return isHappy(temp); 
//         }
//     }
// };
// int main() {
//     Solution s;
//     cout<<s.isHappy(2);
// }
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        vector <vector<int>> result;
        if (nums.size() < 4) {
            return result;
        }
        sort(nums.begin(),nums.end());
        for (int i = 0; i < nums.size()-3; i++) {
            if (i > 0 && nums[i] != nums[i-1] || i == 0) {//确保不进行a的重复元素包含
                if(nums[i] >= target && nums[i] > 0) break;
                for (int j = i+1; j < nums.size()-2; j++){
                    // if (nums[j] > target - nums[i]) {//首元素淘汰，加快计算,由于负数，所以不可以添加此条件
                    //     break;
                    // }
                    if(nums[j] >= target-nums[i] && nums[j] > 0) break;
                    if (j > i+1 && nums[j] == nums[j-1]) continue;//进行元素b的重复去除
                    int left = j+1;
                    int right = nums.size()-1;
                    while (left < right) {
                        if (nums[left] + nums[right] + nums[j] > target-nums[i]) right--;
                        else if (nums[left] + nums[right] + nums[j] < target-nums[i]) left++;
                        else {
                            result.push_back(vector<int> {nums[i],nums[j],nums[left],nums[right]});
                            while (left < right && nums[right] == nums[--right]){}//对c不重复包含
                            while (left < right && nums[left] == nums[++left]){}//对d不重复包含
                            if(nums[left-1] >= target-nums[i] && nums[left-1] > 0) break;
                        }
                    }
                }
            }
        }
        return result;
    }
};
int main() {
    Solution s;
    vector <int> temp = {0,0,0,1000000000,1000000000,1000000000,1000000000};
    int target = 1000000000;
    vector<vector<int>> result = s.fourSum(temp,target); 

}