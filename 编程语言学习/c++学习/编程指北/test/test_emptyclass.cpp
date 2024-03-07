#include <iostream>
class Empty {};
class Derived:public Empty{
};
int main() {
    Empty e1;
    Empty e2;
    Derived e3;
    std::cout << "Size of Empty class: " << sizeof(Empty) <<sizeof(e3)<< std::endl;
}