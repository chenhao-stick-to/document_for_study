#include <iostream>
#include <stdio.h>
class MyClass {
public:
    MyClass() {  }
    MyClass(const MyClass& other) { std::cout << "Copy constructor called" << std::endl; }
};

MyClass foo() {
    return MyClass();
}

int main() {
    MyClass obj = foo();
    MyClass obj1(obj);
    return 0;
}