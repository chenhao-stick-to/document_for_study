#include <memory>
#include <iostream>
using namespace std;
class B;

class A {
public:
    std::shared_ptr<B> b_ptr;
    int a;
    ~A() { std::cout << "A deleted\n"; }
};

class B {
public:
    std::weak_ptr<A> a_ptr;
    ~B() { std::cout << "B deleted\n"; }
};

int main() {
    A* temp = new A();
    {
        std::shared_ptr<A> a(temp);
        std::shared_ptr<B> b(new B());
        A* temp1 = a.get();
        delete temp1;
        temp = nullptr;
        cout<<a.use_count()<<b.use_count()<<endl;
        a->b_ptr = b;
        b->a_ptr = a;
        cout<<a.use_count()<<b.use_count()<<endl;
    }
    return 0;
}