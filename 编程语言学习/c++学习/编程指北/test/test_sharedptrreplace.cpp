#include <memory>
#include <iostream>
class B;

class A {
public:
    std::shared_ptr<B> b_ptr;
    ~A() { std::cout << "A deleted\n"; }
};

class B {
public:
    std::weak_ptr<A> a_ptr;
    ~B() { std::cout << "B deleted\n"; }
};

int main() {
    {
        std::shared_ptr<A> a(new A());
        std::shared_ptr<B> b(new B());
        a->b_ptr = b;
        b->a_ptr = a;
    }
    
    return 0;
}