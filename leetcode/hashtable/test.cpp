#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;
class Solution {
public:
    vector <int> exists;
    bool isHappy(int n) {
        int temp = n;
        vector <int> bits; 
        for (int i = 9; i >= 0; i--) {
            if (pow(10,i) > temp){
                bits.push_back(0);
                continue;
            }
            bits.push_back(temp/pow(10,i));
            temp = temp - (int)(temp/pow(10,i))*pow(10,i);
        }
        temp = 0;
        for (int j = 0; j < bits.size(); j++) {
            temp += bits[j]*bits[j];
        }
        if (temp == 1) {
            return true;
        } 
        else {
            if (find(exists.begin(),exists.end(),n) != exists.end())
                return false;
            exists.push_back(n);
            return isHappy(temp); 
        }
    }
};
int main() {
    Solution s;
    cout<<s.isHappy(2);
}