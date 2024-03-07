#include <iostream>
int main(){
    float f = 1.0;
    short c = *(short*)&f; 
    std::cout<<f<<" "<<c<<std::endl;
    return 0;
}