#include <memory>
#include <iostream>
using namespace std;
class B;

class A {
public:
    std::shared_ptr<A> b_ptr;
    int a;
    ~A() { std::cout << "A deleted\n"; }
};

class B {
public:
    std::shared_ptr<A> a_ptr = std::make_shared<A>() ;
    ~B() { std::cout << "B deleted\n"; }
};

int main() {
    std::shared_ptr<A> a(new A());
    std::shared_ptr<A> a1(new A());
    std::shared_ptr<B> b(new B());
    
    cout<<a.use_count()<<b.use_count()<<a1.use_count()<<endl;
    a->b_ptr = b->a_ptr;
    a1->b_ptr = a;
    b->a_ptr = a1;
    cout<<a.use_count()<<b.use_count()<<a1.use_count()<<endl;
    
    return 0;
}