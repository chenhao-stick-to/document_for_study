#include <iostream>
#include <cstring>
using namespace std;
class MyClass {
public:
    MyClass(const char* str) {
        data = new char[strlen(str) + 1];
        cout<<"default constructor!"<<endl;
        strcpy(data, str);
    }

    // 深拷贝的拷贝构造函数
    MyClass(const MyClass& other) {
        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);
        cout<<"copy constructor!"<<endl;
    }

    // 深拷贝的赋值运算符重载
    MyClass& operator=(const MyClass& other) {
        if (this == &other) {
            return *this;
        }
        
        delete[] data;
        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);
        cout<<"copy !"<<endl;
        return *this;
    }
    
   void SetString(const char* str) {
     if (data != NULL) {
       delete[] data;
     }
     data = new char[strlen(str) + 1];
     strcpy(data, str);
   }
   
    ~MyClass() {
        delete[] data;
    }

    void print() {
        std::cout << data << std::endl;
    }

private:
    char* data;
};

int main() {
    MyClass obj1("Hello, World!");
    // MyClass obj2 ="Hello, World!";//复制省略，直接执行默认构造函数
    MyClass obj2 = obj1; // 深拷贝，执行拷贝构造函数
    MyClass obj3("Hello World!");
    obj3 = obj1;//执行重载的拷贝函数。
    obj1.print(); // 输出：Hello, World!
    obj2.print(); // 输出：Hello, World!
    obj3.print();
    // 修改obj2中的数据，不会影响obj1
    obj1.SetString("Test");
    obj1.print(); // 输出：Hello, World!
    obj2.print(); // 输出：Hello, World!
    obj3.print();
    return 0;
}
