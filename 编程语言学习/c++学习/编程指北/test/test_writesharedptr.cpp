#include <iostream>
using namespace std;
template <typename T> //模板用于接收各种类型
class MySimpleSharedPtr{
    public:
        explicit MySimpleSharedPtr(T* ptr = nullptr):ptr_(ptr),count_(ptr?new size_t(1):nullptr) {}//只能接收显示构造，make_shared不存在
        MySimpleSharedPtr (const MySimpleSharedPtr& other):ptr_(other.ptr_),count_(other.count_) {//不需返回引用，为构造函数，初始化对象。
            if (count_)              
                (*count_)++;
            cout<<"copy constructor!"<<endl;
        }

        MySimpleSharedPtr& operator = (const MySimpleSharedPtr& other) {//需要返回整个对象来进行赋值操作
        if (this != &other) {//保证不是自己给自己赋值
            release();//释放掉智能指针指向的原来的对象的对应的引用计数，为0时，释放原来的对象的内存
            ptr_ = other.ptr_;
            count_ = other.count_;
            (*count_)++;//当前对象的引用计数加1，智能指针对象加1.
        }
        cout<<"= constructor"<<endl;
        return *this;
        }

        ~MySimpleSharedPtr() {
            release();
        }
        T& operator *() const{//const禁止修改成员变量。
            return *ptr_;
        }
        T* operator ->() const{
            return ptr_;
        }
        T* get() const{
            return ptr_;
        }
        size_t use_count() {
            return count_?*count_:0;
        }
    private:
        release(){
            if (count_ && (*count_)-- == 0) {
                delete ptr_;
                delete count_;
            }
        }
        T* ptr_;
        size_t* count_;//一定要注意判断count_和ptr_的有效值，虽然理论上没什么问题
};
class A {
    public:
        A(){}
        int a = 10;
};
int main() {
    MySimpleSharedPtr<int> a(new int(1));
    MySimpleSharedPtr<int> b(a);
    cout<<'1'<<endl;
    MySimpleSharedPtr<int> c = b;//调用拷贝构造函数，因为是在声明和初始化
    MySimpleSharedPtr<int> d(new int(3));
    MySimpleSharedPtr<int> e(d);
    d = a;//这个调用赋值运算符函数，因为两个都已经初始化
    cout<<a.use_count()<<" "<<b.use_count()<< " "<<c.use_count()<<" "<<d.use_count()<<" "<<e.use_count()<<endl;
    // MySimpleSharedPtr<A> a(new A());
    // MySimpleSharedPtr<A> b(a);
    // b->a = 11;
    // MySimpleSharedPtr<A> c = b;
    // cout<<(*a).a<<" "<<b->a<<endl;
    // A* temp = c.get();
    // temp->a = 22;
    // cout << (*a).a<<" "<<b->a<<" "<<temp->a<<endl;
    // temp = nullptr;
    // {MySimpleSharedPtr<A> d(c);
    //     temp = d.get();
    //     d->a = 1;
    //     cout<<temp<<" "<<temp->a<<endl;
    // }
    // cout<<temp<<" "<<temp->a<<endl;

}