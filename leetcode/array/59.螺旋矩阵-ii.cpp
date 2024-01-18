/*
 * @lc app=leetcode.cn id=59 lang=cpp
 *
 * [59] 螺旋矩阵 II
 */

// @lc code=start
// class Solution {//采用递归解法，能做但占用的内存空间太大了，速度和空间效果不理想
// public:
//     vector<vector<int>> generateMatrix(int n) {//尝试递归解法在
//         vector<vector<int>> martix_result(n,vector<int>(n));
//         if (n == 1) {
//             vector<int> inside = {1};
//             vector<vector<int>> outside;
//             outside.emplace_back(inside);
//             return outside;
//         }
//         if (n == 2) {
//             vector<int> inside1 = {1,2};
//             vector<int> inside2 = {4,3};
//             vector<vector<int>> outside;
//             outside.emplace_back(inside1);
//             outside.emplace_back(inside2);
//             return outside;
//         }
//         vector<vector<int>> tmp=generateMatrix(n-2);//获取子螺旋矩阵
//         for(int i = 0; i < n-1; i++) {
//             martix_result[0][i] = i+1;
//             martix_result[i][n-1] = n + i;
//             martix_result[n-1][n-i-1] = 2*n - 1 + i;
//             martix_result[n-i-1][0] = 3*n -2 + i;
//         }
//         for(int i = 0; i < n-2; i++) {
//             for(int j = 0; j < n-2; j++) {
//                 martix_result[i+1][j+1] = 4*n - 4 + tmp[i][j];
//             }
//         }
//         return martix_result;
//     }
// };
class Solution {//
public:
    vector<vector<int>> generateMatrix(int n) {//尝试逐层分解法
        vector<vector<int>> martix_result(n,vector<int>(n));
        int loop_max = (n + 1)/2;
        int sum = 0;
        int loop = 0;
        int tmp_n = n;
        while(loop < loop_max) {//循环终止条件
            for(int i = 0; i <= n-1-2*loop; i++) {//左闭又开
                if(i + loop != n-1-loop | i == 0){//表示 i到最大值不进行赋值操作（左闭右开）/i==0表示刚开始需要进行赋值操作（防止最大值为i==0时取得）
                    martix_result[loop][i+loop] = i + 1 + sum;
                    martix_result[i+loop][n-loop-1] = tmp_n + i + sum;
                    martix_result[n-loop-1][n-i-1-loop] = 2*tmp_n - 1 + i + sum;
                    martix_result[n-i-1-loop][loop] = 3*tmp_n -2 + i + sum;
                }
            }
            sum += 4*tmp_n-4;
            tmp_n -= 2;
            loop++;
        }
        return martix_result;
    }
};
// @lc code=end

