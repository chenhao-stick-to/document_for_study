#include <iostream>
#include <memory>
using namespace std;
class MyClass {
public:
    MyClass() { std::cout << "MyClass 构造函数"<<endl; }
    ~MyClass() { std::cout << "MyClass 析构函数"<<endl; }
    void do_something() { std::cout << "MyClass::do_something() 被调用"<<endl; }
};

int main() {
    {
        std::shared_ptr<MyClass> ptr1 = std::make_shared<MyClass>();
        {
            std::shared_ptr<MyClass> ptr2 = ptr1; // 这里共享 MyClass 对象的所有权
            ptr1->do_something();
            ptr2->do_something();
            std::cout << "ptr1 and ptr2 use_count " << ptr1.use_count() << " "<<ptr2.use_count()<<std::endl;
        } // 这里 ptr2 被销毁，但是 MyClass 对象不会被删除，因为 ptr1 仍然拥有它的所有权
        std::cout << "ptr1 use_count: " << ptr1.use_count() << std::endl;
    } // 这里 ptr1 被销毁，同时 MyClass 对象也会被删除，因为它是最后一个拥有对象所有权的 shared_ptr

    return 0;
}
