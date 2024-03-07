#include <iostream>
// 函数指针
typedef void(*Func)(void);//函数指针，接收void返回类型以及接收无参数

class MyBaseClass {
public:
    // virtual void virtualMethod0() = 0;
    virtual void virtualMethod1() {
        std::cout << "BaseClass::virtualMethod1()" << std::endl;
    }
    virtual void virtualMethod2() {
        std::cout << "BaseClass::virtualMethod2()" << std::endl;
    }
    virtual void virtualMethod3() {
        std::cout << "BaseClass::virtualMethod3()" << std::endl;
    }
    virtual ~MyBaseClass() {
        std::cout<<"Baseclass::out"<<std::endl;
    }
};

class MyDerivedClass : public MyBaseClass {
public:
// virtual void virtualMethod0() override {
//     std::cout << "DerivedClass::virtualMethod0()" << std::endl;
// }
    virtual void virtualMethod3() {
        std::cout << "DerivedClass::virtualMethod3()" << std::endl;
    }
    virtual void virtualMethod4() {
        std::cout << "DerivedClass::virtualMethod4()" << std::endl;
    }
    virtual void virtualMethod5() {
        std::cout << "DerivedClass::virtualMethod5()" << std::endl;
    }
    // ~MyDerivedClass() {
    //     std::cout<<"Derivedclass::out"<<std::endl;
    // }
};

void PrintVtable(void** vtable) {
    // 输出虚函数表中每个函数的地址
    for (int i = 0; vtable[i] != nullptr; i++) {
        // 最多调用五个函数，怕访问到虚函数表非法的地址，因为就五个函数
        if (i >= 8)  {
            return;
        }
        std::cout << "Function " << i << ": " << vtable[i] << std::endl;
        // 将虚函数表中的虚函数转换为函数指针，然后进行调用
        Func func =(Func)vtable[i];//这种强制转换并不推荐，即将一个void*类型的函数指针强制转换为Func的指针，然后再进行调用。
        func();
    }
}

int main() {
    MyDerivedClass obj;

    // 取对象的首地址，然后转换为的指针，就取到了虚函数表指针，指向 obj 对象的虚函数表
    // 因为大多数实现上，虚函数表指针一般都放在对象第一个位置
    // void** vtable = *(void***)(&obj);
    // std::cout << "DerivedClass Vetable:" << std::endl;
    // // 打印子类的虚函数表
    // PrintVtable(vtable);

    std::cout << std::endl <<  "BaseClass Vetable:" << std::endl;
    MyBaseClass base_obj;
    void** vbtable = *(void***)(&base_obj);
    // 打印父类的虚函数表
    PrintVtable(vbtable);
    return 0;
}
