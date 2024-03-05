#include <bits/stdc++.h>
using namespace std;

// int main(){
//     const int a = 10;
//     const int* p2 = &a;
//     int* p1 =const_cast<int*>(p2);
//     *p1 = 22;
//     *p1 = 30;
//     cout<< a <<" "<<*p1 << " "<< &a << " "<<p1;
//     return 0;
// }

int main(){
    const int a = 10;
    const int* const p2 = &a;
    *p2 = 22;
    cout<< a <<" "<<*p2 ;
    return 0;
}