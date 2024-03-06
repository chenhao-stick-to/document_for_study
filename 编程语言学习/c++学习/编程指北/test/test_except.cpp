#include <iostream>
class C{
public:
    ~C(){ throw 1;}
};
int main(){
    try{
        C c;
    }
    catch(...){}
    return 0;
}
