![image-20240301001636573](c++后台开发面试重点知识/image-20240301001636573.png)
如上是快速突击C++的面试的准备。
# C++基础
## sizeof关键字
sizeof的计算，牢记下面4条规则：
- 指针的大小永远是固定的，取决于处理器位数，32位就是 4 字节，64位就是 8 字节
- 数组作为函数参数时会退化为指针，大小要按指针的计算。
- struct 结构体要考虑字节对齐，字节对齐的具体规则请看这篇文章: 字节对齐(opens new window)
- 字符串数组要算上末尾的 '\0
对数组退化为指针，只有函数参数是普通拷贝时发生，避免整个数组拷贝在栈上传递。而对于函数参数是数组引用传递，则不会退化为指针，因为其本来就不用拷贝。**为啥调用sizeof()直接计算这个数组的大小不会发生数组退化，因为sizeof是在编译期间计算得到的结果，可直接得到数组的大小**
**字节对齐：**
有助于提高内存访问速度，许多处理器都优化了对齐数据的访问，有一定的空间浪费。
编译器指令 #pragma pack更改默认的对齐规则，全局生效，减小数据结构的大小，降低访问性能。
C++11及更高版本，alignas(16) int x; 将确保 x 的地址是 16 的倍数。
```c
#pragma pack(push, 1) // 设置字节对齐为 1 字节，取消自动对齐
struct UnalignedStruct {//结果为7字节
    char a;
    int b;
    short c;
};
#pragma pack(pop) // 恢复默认的字节对齐设置
```
## sizeof 和 strlen
sizeof计算数据类型或对象占用字节数，包含空字符"\0"。
strlen则计算c风格字符串长度，不包含空字符"\0".
## const关键字
### 修饰变量
const关键字修饰变量不可直接修改，可以使用取指针，然后使用const_cast进行类型转换来修改值，不会报错。但是我在linux和windows上均尝试了，且尝试了一些禁用编译器(-fno-strict-aliasing)的常量传播/常量折叠的选项，还是无法通过指针来改变，最终结果是在同一个地址上，cout原来值和指针改变后的值却不同？
怀疑是编译器将const常量的值放入了.rodata段，其对应的地址不能修改值，即使指针可以绕一下，但是还是不管用。
### 修饰函数参数，表示函数不会修改参数
const引用常用
### 修饰函数返回值
这样做可以使函数返回的值更加安全，避免被误修改。
### 修饰指针或引用
指向只读变量的指针，底层const。指针可修改，指针指向变量不可修改。
只读指针，顶层const。指针本身是只读变量。
只读指针指向只读变量,无法修改指针和变量
常量引用，不能常量引用修改值
### 修饰成员函数
```c
class A {//如下修饰了成员函数，可以被常量对象调用，修饰的成员函数不可调用非const的成员函数
public:
    int func() const {
        // 编译错误，不能修改成员变量的值
        m_value = 10;
        return m_value;
    }
private:
    int m_value;
};
```
## C++中的static关键字
static修饰的全局变量，将变量的作用域限定在当前文件中，使得其他文件无法访问该变量。static修饰的变量在程序启动时进行初始化（main函数之前），生命周期和程序一样。
```c
// a.cpp 文件
static int a = 10;  // static 修饰全局变量
int main() {
    a++;  // 合法，可以在当前文件中访问 a
    return 0;
}

// b.cpp 文件
extern int a;  // 声明 a
void foo() {
    a++;  // 非法，会报链接错误，其他文件无法访问 a
}
//如上面就是将一个文件的static变量，在另一个文件用extern引用会出错。
```
原因看另一个：**extern的作用**
- 符号的声明和定义，如下：
```c++
//对变量
// 声明
extern int global_var;

// 定义
int global_var = 42;

//对函数
// 声明
int sum(int a, int b);

// 定义
int sum(int a, int b) {
  return a + b;
}
```
- C/c++中的链接属性，包含了外部链接（全局变量和函数；可在不同源文件间共享），内部链接（static修饰的全局变量和函数有内部链接，只在当前源文件使用），无链接（只在当前 代码块/函数 进行使用，const/constexpr修饰的常量具无链接属性），外部c链接(extern "C" 关键字，可在不同源文件之间共享，具C语言的名称和调用约定，可以和c语言编写的代码交互)。
- extern作用
1）声明但不定义变量
```c
//fileA.cpp
int i = 1;         //声明并定义全局变量i

//fileB.cpp
extern int i;    //声明i，链接全局变量

//fileC.cpp
extern int i = 2;        //错误，多重定义
int i;                    //错误，这是一个定义，导致多重定义
main()
{
    extern int i;        //正确
    int i = 5;            //正确，新的局部变量i;
}
```
2) 常量全局变量的外部链接
```c
//正确声明定义一个常量的多文件共享的变量
//fileA.cpp
extern const int i = 1;        //定义

//fileB.cpp                    //声明
extern const int i;

//错误声明定义
//fileA.cpp
const int i = 1;        //定义 (不用 extern 修饰)

//fileB.cpp             //声明
extern const int i;
```
3） 编译和链接过程
编译期，extern告诉编译器该变量定义在其他函数中，建立一个该符合引用，是一个未定义的符号。
链接期，其他目标文件找到定义，链接。
## C++中volatile作用
volatile关键字可以禁止被进行外部因素更改，如硬件，os/编译器，其他线程等。
当一个变量被声明为volatile时，编译器会禁止对该变量进行优化，以确保每次访问变量时都会从内存中读取其值，而不是从寄存器或缓存中读取。
但是无法解决多线程的数据竞争问题，解决这个问题还是需要使用互斥锁，原子操作，信号量等同步机制。
## 字节序
字节序指的是在多字节数据类型如整数，浮点数中，字节在内存中的存储顺序。
一个数字的高低位在地址中的存储位置构成字节序。
大端字节序：将数字的高位存储在地址高位，低位存储在地址低位。
小段字节序，和大端字节序相反。
```c++
#include <iostream>//判断字节序
int main() {
    int num = 1;
    // 将int类型指针转换为char类型指针，取第一个字节
    char* ptr = reinterpret_cast<char*>(&num);

    if (*ptr == 1) {
        std::cout << "Little-endian" << std::endl;
    } else {
        std::cout << "Big-endian" << std::endl;
    }
    return 0;
}
```
### 常见的大小端字节序
- 网络传输，tcp/ip规定大端传输，本地是小端需要转换，使用htonl(),htons(),ntohl(),ntohs()等函数完成。
- linux，x86是小端，power PC是大端，取决于具体的硬件平台。
- windows，主要运行在x86上，小段字节序。
- mac，arm架构（m1芯片）/intel处理器，小端。
## class 和struct的区别
不同点：
- class 中类中的成员默认都是 private 属性的，而在 struct 中结构体中的成员默认都是 public 属性的。
- class 继承默认是 private 继承，而 struct 继承默认是 public 继承。
- class 可以用于定义模板参数，struct 不能用于定义模板参数。
实际使用中，struct 我们通常用来定义一些 POD(plain old data)，POD是 C++ 定义的一类数据结构概念，比如 int、float 等都是 POD 类型的。而class 多用于面向对象编程。
## 宏定义define和内联函数inline
define 用于定义宏，主要定义常量和创建宏函数。如：
```c
#define SQUARE_SUM(x, y) ((x) * (x) + (y) * (y))
//参数和函数体用括号包围。避免因为运算优先级导致的错误。
```
inline 用于内联函数，==注意inline的就地展开和编译器的重复代码消除，这个是在编译时编译器来进行确定，针对不同类型函数选用不同策略，不一定用户指定了inline，就一定会展开。而define则是在编译器一定会展开，至于后续是否对展开的代码进行优化，这个取决于编译器和具体的编译优化策略。此外define不会像inline进行参数的类型以及作用域等的检查，不太安全；define不可进行调试，但inline可以，==
### 宏可导致不合理计算
```c
#define MAX(a, b) ((a) > (b) ? (a) : (c))

int x = 1;
int y = MAX(x++, 10);
// 宏定义 x++ 会被执行两次, 因为 x++ 会被替换到两个地方，使用内联函数则不会出现这个问题，本质是因为define直接替换，而inline需要进行参数传递，所以会有赋值之类的。
//！注意的就是使用宏替换时，直接替换导致的一些不合理替换计算。
```
## define 和typedef 定义别名的方法的区别
- define在预处理阶段，typedef在编译的时候，后者有类型检查
- 作用域限制， 宏定义无作用域限制，如代码块，命名空间限制；typedef有。
- 模板支持，宏不支持模板别名，typedef支持。如下：
```c
// 使用 typedef 定义模板类型别名
template <typename T>
struct MyContainer {
    typedef std::vector<T> Type;
};

// ==使用 using 定义模板类型别名（C++11 及以后）==
template <typename T>
struct MyContainer {
    using Type = std::vector<T>;
};

MyContainer<int>::Type intVector;//具体使用就是如此使用，声明一个vector<int> 变量。
```
## explicit作用
可以防止修饰的构造函数进行隐式转换，只能使用显式转换。
```c
class MyInt {
public:
    MyInt(int n) : num(n) {}
private:
    int num;
};

MyInt a = 10;//进行了MyInt(10)的隐式转换为MyInt类型，再进行赋值；但编译器优化，不会产生这个临时对象，而是原地初始化这个对象。
void f(MyInt n) {
    // do something
}
f(10);//我们可能不希望有隐式转换

class MyInt {
public:
    explicit MyInt(int n) : num(n) {}//explicit那么不可以进行隐式的转换
private:
    int num;
};
//针对编译器的复制消除优化，如下：
class MyClass {
public:
    MyClass() {  }
    MyClass(const MyClass& other) { std::cout << "Copy constructor called" << std::endl; }
};

MyClass foo() {
    return MyClass();
}

int main() {
    MyClass obj = foo();//理论式是要调用拷贝构造函数，但由于编译器的复制省略优化，可原地再obj的地址构造foo()返回值，而不需要拷贝构造函数。
    return 0;
}
```
## extern C的作用
各个编译器对函数的修饰方法不同，使用extern "C"关键字，可将C++编译器的命名规则转换为C语言的命名规则，使C++代码可调用C语言的函数/变量。

