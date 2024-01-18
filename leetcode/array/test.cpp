#include <vector>
#include <iostream>
using namespace std;
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
                if(i + loop != n-1-loop | i == 0){//表示 i到最大值不进行赋值操作（左闭右开）/i==0刚开始需要进行赋值操作
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
int main(){
    Solution s;
    vector<vector<int>> tmp = s.generateMatrix(3);
    for(int i=0; i<tmp[0].size();i++){
        for(int j=0; j<tmp[0].size();j++){
            cout<<tmp[i][j]<<" ";
        }
        cout<<endl;
    }
}