#include <iostream>
using namespace std;
class Base{
    public:
    virtual void func1(){
        cout<<"Base"<<endl;
    }
};
class Derive:public Base{
    public:
    void func1() override{
        cout<<"Base1"<<endl;
    }
};
int main(){
    Base tmp;
    tmp.func1();
    Derive tmp1;
    tmp1.func1();
}