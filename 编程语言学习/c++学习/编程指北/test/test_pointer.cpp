#include <iostream>
#include <cstring>
using namespace std;
void change_string(string* temp) {
    temp = new string("chenhao");
}
int main(){
    int *a = new int(10);
    int c = 20;
    int &b = c;
    cout<<a <<" "<<*a<<" "<<b<<" "<<&b;
    delete a;
    b = 12;    // 对已经
    string d = "haochen";
    change_string(&d);
    cout<<d;
    return 0;
}