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
各个编译器对函数的修饰方法不同，使用extern "C"关键字，可将C++编译器的命名规则转换为C语言的命名规则，使C代码可调用C++语言编写的函数/变量（使用extern "C"修饰）。
extern "C"对函数的参数类型和返回值类型无影响，**使用extern "C"声明函数，需要保证函数的参数类型和返回值类型和C语言定义一样。C语言不支持class，模板等参数类型，需自行确定。**
## mutable 作用
mutable是C++中的一个关键字，用于修饰类的成员变量，表示该成员变量即使在一个const成员函数中也可以被修改。（==有点奇怪==。
==如果需要在const函数里面修改一些跟类状态无关的数据成员，那么这个函数就应该被mutable来修饰==，并且放在函数后后面关键字位置。如下：
```c
#include <iostream>

class Counter {
public:
    Counter() : count(0), cache_valid(false), cached_value(0) {}

    int get_count() const {//声明为const，不能修改成员变量；但用mutable修饰，故可以修改
        if (!cache_valid) {
            // 模拟一个耗时的计算过程
            cached_value = count * 2;
            cache_valid = true;
        }

        return cached_value;
    }

    void increment() {
        count++;
        cache_valid = false; // 使缓存无效，因为count已经更改
    }

private:
    int count;
    mutable bool cache_valid; // 缓存是否有效的标志
    mutable int cached_value; // 缓存的值
};

int main() {
    Counter counter;
    counter.increment();
    counter.increment();

    std::cout << "Count: " << counter.get_count() << std::endl; // 输出 4

    return 0;
}
```
## C++的几种类型转换
static_cast,dynamic_cast,const_cast,reinterpret_cast。
- static_cast,和c语言的（）强制转换基本等价
1）基本类型转换（int，short，long，float，double等互相转换）
2）指针类型转换，在类层次结构中，尤其是将基类指针转换为派生类指针，转换不执行运行时类型检查(dynamic_cast会进行)，可能不安全。
==C++的类型检查，C++的数据类型大部分是在编译时就确定了的，这是就可以进行编译时类型检查；但是由于C++的多态机制（基于虚函数实现），所以会出现运行时确定调用函数，即再确定数据类型的情况，这就是运行时类型检查。（基类指针可以指向任意的派生类的对象，如何知道实际指向的派生类类型，可以运用C++的RTTI机制（typeid和dynamic_cast操作符）将基类指针安全的转换为派生类类型的指针或者引用）==。
3）引用类型的转换
```c++
Derived derived_obj;
Base& base_ref = derived_obj;
Derived& derived_ref = static_cast<Derived&>(base_ref); // 将基类引用base_ref转换为派生类引用derived_ref
```
- dynamic_cast
dynamic_cast在C++中主要应用于父子类层次结构中的安全类型转换,可在运行时指向类型检查，对比static_cast更安全。
1）向下类型转换，基类指针或引用转换为派生类指针或引用时，dynamic_cast可以确保类型兼容性，转换失败，dynamic_cast返回空指针（指针类型）/抛出异常（引用类型）
```c++
class Base { virtual void dummy() {} };
class Derived : public Base { int a; };

Base* base_ptr = new Derived();
Derived* derived_ptr = dynamic_cast<Derived*>(base_ptr); // 将基类指针base_ptr转换为派生
```
2) dynamic_cast底层原理
```c++
class Animal { public: virtual ~Animal() {} };
class Dog : public Animal { public: void bark() { /* ... */ } };
class Cat : public Animal { public: void meow() { /* ... */ } };

Animal* animal_ptr = /* ... */;

// 尝试将Animal指针转换为Dog指针
Dog* dog_ptr = dynamic_cast<Dog*>(animal_ptr);//不是该子类，则返回空指针
if (dog_ptr) {
    dog_ptr->bark();
}

// 尝试将Animal指针转换为Cat指针
Cat* cat_ptr = dynamic_cast<Cat*>(animal_ptr);
if (cat_ptr) {
    cat_ptr->meow();
}
/*
dynamic_cast有效,基类至少有一个虚函数，dynamic_cast只有在基类存在虚函数(虚函数表)的情况下才有可能将基类指针转化为子类
*/
```
3）dynamic_cast 底层原理
其依赖于运行时类型信息（RTTI, Runtime Type Information），包含类的类型信息和类层次结构。
使用虚函数时，该类会生成一个虚函数表，虚函数表有RTTI的信息，以及各虚函数的地址。
```c
class Point
{
public:
	Point(float xval);
	virtual ~Point();
 
	float x() const;
	static int PointCount();
 
protected:
	virtual ostream& print(ostream& os) const;
 
	float _x;
	static int _point_count;
};
```
下面是该类对应的对象模型：
![image-20240305233109207](c++后台开发面试重点知识/image-20240305233109207.png)
每个对象都有一个vptr指向虚函数表，从而可以获取该类的RTTI的信息。
下面是dynamic_cast的工作原理：
- 首先，dynamic_cast通过查询对象的 vptr 来获取其RTTI（这也是为什么 dynamic_cast 要求对象有虚函数）
- 然后，dynamic_cast比较请求的目标类型与从RTTI获得的实际类型。如果目标类型是实际类型或其基类，则转换成功。
- 如果目标类型是派生类，dynamic_cast会检查类层次结构，以确定转换是否合法。如果在类层次结构中找到了目标类型，则转换成功；否则，转换失败。
- 当转换成功时，dynamic_cast返回转换后的指针或引用。
- 如果转换失败，对于指针类型，dynamic_cast返回空指针；对于引用类型，它会抛出一个std::bad_cast异常。
dynamic_cast是运行时动态类型检查，性能比static在编译器转换时要慢。
3）const_cast
- 修改const对象,有尝试过，在const关键字有讲到。
- const对象调用非const成员函数
```c++
class MyClass {
public:
    void non_const_function() { /* ... */ }
};

const MyClass my_const_obj;
MyClass* mutable_obj_ptr = const_cast<MyClass*>(&my_const_obj); // 删除const属性，使得可以调用非const成员函数
mutable_obj_ptr->non_const_function(); // 调用非const成员函数
//用于const的常量对象的const去除，来调用对应类的非常量函数。
//！行为不是很安全，请自己谨慎操作
```
- reinterpret_cast
用法 reinterpret_cast <new_type> (expression)，用于在不同类型之间进行低级别的转换，不进行类型的检查，仅仅是重新解释底层比特（也就是对指针所指针的那片比特位换个类型做解释）。可能导致==未定义的行为==。
```c++
int a = 42;
int* int_ptr = &a;
char* char_ptr = reinterpret_cast<char*>(int_ptr); // 将int指针转换为char指针
//如图像数据的转换为char*字符流进行保存！
```
对reinterpret_cast的详细解释 [reinterpret_cast解释](https://zhuanlan.zhihu.com/p/33040213)

# C++面向对象
## 面向对象三大特性，封装，继承，多态
- 封装，将数据（属性）和操作这些数据的函数（方法）组合再一个类中的过程。封装的目的是隐藏类的内部实现细节，仅暴露外部必要接口。控制类成员的访问级别。
- 继承，继承允许我们创建具有共享代码的类层次结构，访问修饰符（如 public、protected、private）控制了派生类对基类成员的访问权限。
- 多态，在 C++ 中，多态主要通过虚函数（Virtual Function）和抽象基类（Abstract Base Class）来实现。虚函数允许在派生类中重写基类的方法，而抽象基类包含至少一个纯虚函数（Pure Virtual Function），不能被实例化，只能作为其他派生类的基类。==派生类也必须有这个抽象基类的全部纯虚函数的实现，这样它才不是抽象类，可以有实例对象==
## c++类成员访问权限
public，protected，private三种继承，派生类都不能直接访问基类的私有成员。只能访问公有和保护成员，但是这两个成员在派生很类的访问权限和继承方式相关，小于这个权限就变成这个权限，大于则保持不变。（权限：public < protected < private）              
## 重载、重写、隐藏的区别       
- 重载，相同作用域（命名空间，同一个类）拥有相同的方法名。特点：
方法具有相同的名称。方法具有不同的参数类型或参数数量。返回类型可以相同或不同。同一作用域，比如都是一个类的成员函数，或者都是全局函数。
- 重写，用在派生类重新定义基类的方法。条件有：方法具有相同的名称。方法具有相同的参数类型和数量。方法具有相同的返回类型。重写的基类中被重写的函数必须有virtual修饰。重写主要在继承关系的类之间发生。
- 隐藏，隐藏是指派生类的函数屏蔽了与其同名的基类函数。注意只要同名函数，不管参数列表是否相同，基类函数都会被隐藏。
```c++
#include<iostream>
using namespace std;

classA{
public:
  void fun1(int i, int j){
    cout <<"A::fun1() : " << i <<" " << j << endl;
  }
};
classB : public A{
public:
  //隐藏
  void fun1(double i){
    cout <<"B::fun1() : " << i << endl;
  }
};
int main(){
  B b;
  b.fun1(5);//调用B类中的函数
  b.fun1(1, 2);//出错，因为基类函数被隐藏
  system("pause");
  return 0;
}
```
- **区别**
1）重载和重写：重写不同类，重载相同作用域。重写参数列表一定相同，重载参数列表一定不同。重写，基类一定有virtual修饰，重载则可有可没有。
2）隐藏，重写，重载区别。隐藏函数和被隐藏函数参数列表可以相同，也可以不同，但函数名一定同；当参数不同时，无论基类中的函数是否被virtual修饰，基类函数都是被隐藏，而不是被重写
## C++ 类对象的初始化和析构顺序
- 基类初始化顺序，**如果当前类继承自一个或多个基类，它们将按照==声明顺序进行初始化==，但是在有虚继承和一般继承存在的情况下，优先虚继承**。
- 成员变量初始化顺序，类成员变量按在类定义的声明顺序进行初始化！有基类的初始化则先初始化基类。
- 执行构造函数，如下：
```c++
#include <iostream>

class Base {
public:
    Base() { std::cout << "Base constructor" << std::endl; }
    ~Base() {
        std::cout << "Base destructor" << std::endl;
    }
};

class Base1 {
public:
    Base1() { std::cout << "Base1 constructor" << std::endl; }
    ~Base1() {
        std::cout << "Base1 destructor" << std::endl;
    }
};

class Base2 {
public:
    Base2() { std::cout << "Base2 constructor" << std::endl; }
    ~Base2() {
        std::cout << "Base2 destructor" << std::endl;
    }
};

class Base3 {
public:
    Base3() { std::cout << "Base3 constructor" << std::endl; }
    ~Base3() {
        std::cout << "Base3 destructor" << std::endl;
    }
};

class MyClass : public virtual Base3, public Base1, public virtual Base2 {//声明顺序
public:
    MyClass() : num1(1), num2(2) {
        std::cout << "MyClass constructor" << std::endl;
    }
    ~MyClass() {
        std::cout << "MyClass destructor" << std::endl;
    }

private:
    int num1;
    int num2;
    // 这个是为了看成员变量的初始化顺序
    Base base;
};

int main() {
    MyClass obj;
    return 0;
}
//初始化列表的初始化顺序，总结就是一个派生类，按照继承的声明顺序排列，虚继承的类的初始化优先，然后再实际的父类初始化，最后再初始化本类的成员变量。
```
==**虚继承相关知识**==：
每个虚继承的子类都有一个虚基类表指针（占用一个指针的存储空间，4字节）和虚基类表（不占用类对象的存储空间）。虚基类表指针（virtual base table pointer）指向虚基类表（virtual table），虚表中记录了虚基类与本类的偏移地址，通过偏移地址，就找到了虚基类成员。对比虚函数实现：
它们有相似之处，都利用了虚指针（均占用类的存储空间）和虚表（均不占用类的存储空间）。
虚基类依旧存在继承类中，占用存储空间；虚函数不占用存储空间。
**虚基类表存储的是虚基类相对直接继承类的偏移；而虚函数表存储的是虚函数地址。**
```c++
class A    // 大小为 4  
{  
public:  
    int a;  
};  
class B :virtual public A    // 大小为 12，变量 a, b 共 8 字节，虚基类表指针 4  
{  
public:  
    int b;  
};  
class C :virtual public A   // 与 B 一样 12  
{  
public:  
    int c;  
};  
class D :public B, public C    // **24, 变量 a, b, c, d 共 16，B 的虚基类指针 4，C 的虚基类指针 4**
{  
public:  
    int d;  
}; 
//虚基类指针有继承关系，会包含类层次结构上，所有类的虚基类指针；但是虚函数指针，每个类只有一个。
```
## c++析构函数可以抛出异常吗？
**effective c++ 别让异常逃离析构函数**：
析构函数常常被自动调用，在析构函数中抛出的异常往往会难以捕获，引发程序非正常退出或未定义行为。
析构函数是由C++来调用的，源代码中不包含对它的调用，因此它抛出的异常不可被捕获
==对于栈中的对象而言，在它离开作用域时会被析构；对于堆中的对象而言，在它被delete时析构。==
如果在析构函数**中真的可能存在异常，该如何处理呢**？
```c++
class DBConnection
{ 
public:
　　 ...
　　 static DBConnection create(); //返回DBConnection对象；为求简化暂略参数
　　 void close(); //关闭联机；失败则抛出异常。
};

//如何保证客户不忘记在DBConnection对象上调用close(),创建管理DBConnection资源的class，在析构函数中调用close。即以对象管理资源RAII.
//这个class用来管理DBConnection对象
class DBConn
{ 
public:
　　 ...
　　DBConn(const DBConnection& db)
　　{
       this->db=db;
   }
　 ~DBConn() //确保数据库连接总是会被关闭
　　{
　　    db.close();
　　}
　　
private:
　　 DBConnection db;
};
```
调用close成功，无任何问题，但如果调用产生异常，DBConn析构函数会传播该异常。离开析构函数会造成问题，如何解决？
```c++
//1)如果close抛出异常就结束程序，通常调用abort完成。防止异常导致程序后面的不明确行为。
DBConn::~DBconn()
{
    try
    {
	    db.close(); 
    }
    catch(...)
    {
        abort();//终止程序
    }
}
//2）吞下因调用 close 而发生的异常，理解为打日志，记录发生了什么事，前提事程序能可靠执行。
DBConn::~DBConn
{
    try{ db.close();}
    catch(...) 
    {
        //制作运转记录，记下对close的调用失败！
    }
}

//3）重新设计 DBConn 接口，使客户有机会对出现的异常作出反应
// 在DBConn中添加close函数，将责任转移到DBConn客户中。当存在一种操作失败必须抛出异常时，必须放在析构函数之外去解决这个异常。因为在析构函数吐出异常，会带来"过早结束程序/发生未定义行为危险"
```
析构函数可抛出异常，但不推荐！可能造成：
- 资源泄露：当一个对象被析构时，析构函数负责释放该对象持有的资源。如果析构函数抛出异常，这个过程可能会中断，导致资源泄露。
- 叠加异常：如果析构函数在处理另一个异常时抛出异常，会导致异常叠加。这种情况下，程序将无法处理两个异常，从而可能导致未定义行为或程序崩溃。
为了避免这些问题，**通常建议在析构函数中处理异常或者避免执行会抛出异常的函数，可以在析构函数中使用 try-catch 块来捕获和处理潜在的异常**，确保资源得到正确释放和清理。 
## c++中的深浅拷贝
- 浅拷贝
浅拷贝仅复制对象的基本类型成员和指针成员的值，不复制指针指向的内存。
可能导致两个对象共享相同的资源，从而引发潜在的问题，如内存泄漏、意外修改共享资源等。
编译器默认执行浅拷贝。POD型数据适合浅拷贝。
- 深拷贝
深拷贝不仅复制对象的基本类型成员和指针成员的值，还复制指针所指向的内存。因此，两个对象不会共享相同的资源，避免了潜在问题。==**深拷贝通常需要显式实现拷贝构造函数和赋值运算符重载。**==
```c++
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

```
## C++多态的实现方式
C++实现多态的方法主要包括虚函数、纯虚函数和模板函数；**虚函数、纯虚函数实现的多态叫动态多态（运行时），模板函数、重载等实现的叫静态多态(编译期)。**
- 虚函数、纯虚函数实现多态
C++多态必须满足的条件，必须通过基类的指针或者引用调用虚函数；被调用的函数是虚函数，且必须完成对基类虚函数的重写。
**虚函数用于实现类的多态功能；但是纯虚函数一般用于实现一个抽象基类，限制派生类必须实现某一种行为函数！**
```c++
//派生类中继承的基类的数据成员一定是交给基类进行初始化，无论显式调用基类的构造函数，还是执行默认构造函数。所以基类成员变量不能在初始化列表中进行初始化，但可以在函数内部进行赋值。
#include <iostream>
using namespace std;
class Shape {
    public:
    Shape(){
        width =100;
    height =10;
    }
    //  Shape(int a,int b):width(a),height(b){
    //     cout<<"constrructor!";
    //  }
      virtual int area() = 0;
      int width;
      int height;
    
};

class Rectangle: public Shape {
   public:
        Rectangle(int wide,int he){
        //     width = wide;
        //    height = he;
        }
      int area () { 
         cout << "Rectangle class area :"; 
         return (width * height); 
      }
};

class Triangle: public Shape{
   public:
  Triangle(int wide,int he){
           this->width = wide;
           this->height = he;
        }
      int area () { 
         cout << "Triangle class area :"; 
         return (width * height / 2); 
      }
};

int main() {
    Shape *shape;
    Rectangle rec(10,7);
    Triangle  tri(10,5);

    shape = &rec;
    cout<<shape->area();
    cout<<endl;
    shape = &tri;
    cout<<shape->area();

   return 0;
}
//对于初始化列表，一般有下面三种情况，可在初始化列表中进行初始化！
// 初始化非静态的数据成员：非静态的数据成员可以在初始化列表中初始化。如果你没有在初始化列表中初始化它们，那么它们将使用默认的构造函数进行初始化。
// 初始化基类：如果你的类从一个或者多个基类继承，你可以在初始化列表中调用基类的构造函数以初始化基类的部分。
// ！！！初始化const和引用类型的成员：const数据成员和引用必须在构造过程中初始化，因此你需要在初始化列表中为它们提供初始化值
//为啥尽量在初始化列表中进行初始化，其本质是因为类的成员初始化其实是在初始化列表中完成的，而构造函数花括号里面的其实全部是在进行赋值。所以在初始化列表可以省略一个拷贝赋值的时间！
//至于静态成员变量，岂不是属于类的对象的，是属于类的，所以这个值需要在类外进行初始化！
```
- 模板函数多态
```c++
template <class T>
T GetMax (T a, T b) {
   return (a>b?a:b);
}

int main () {
   int i=5, j=6, k;
   long l=10, m=5, n;
   k=GetMax<int>(i,j);
   n=GetMax<long>(l,m);
   cout << k << endl;
   cout << n << endl;
   return 0;
}
//模板函数可以根据传递参数的不同类型，自动生成相应类型的函数代码。模板函数可以用来实现多态。
```
## this 指针
- this指针：指向当前对象的指针，常量成员函数中，this指针类型为指向常量对象的常量指针，不能用于修改成员变量的值！
- static 函数不能访问成员变量，static函数为静态成员函数，与类而不是类的对象相关！==静态函数没有this指针，所以不能访问任何非静态成员变量，静态成员函数不能有任何非静态成员变量，否则会进行报错！==
总结：this 实际上是成员函数的一个形参，在调用成员函数时将对象的地址作为实参传递给 this。this是一个隐式的形参，本质上是成员函数的局部变量，对象调用成员函数才进行赋值。==成员函数最终被编译成与对象无关的普通函数，除了成员变量，会丢失所有信息，所以编译时要在成员函数中添加一个额外的参数，把当前对象的首地址传入，以此来关联成员函数和成员变量。==
## 虚函数表
### c++对象模型
指向了对 C++的内存模型虚函数表部分的理解。
==C++ 是如何存储一个对象的数据（成员函数、成员变量、静态变量、虚函数等等）==
一般来说通过父类指针来访问子类中没有覆盖父类的成员函数是非法的，但是可以通过获取虚函数表，基于偏移的形式来进行调用！
```c++
#include <iostream>

// 函数指针
typedef void(*Func)(void);

class MyBaseClass {
public:
    virtual void virtualMethod1() {
        std::cout << "BaseClass::virtualMethod1()" << std::endl;
    }
    virtual void virtualMethod2() {
        std::cout << "BaseClass::virtualMethod2()" << std::endl;
    }
    virtual void virtualMethod3() {
        std::cout << "BaseClass::virtualMethod3()" << std::endl;
    }

};

class MyDerivedClass : public MyBaseClass {
public:
    virtual void virtualMethod3() {
        std::cout << "DerivedClass::virtualMethod3()" << std::endl;
    }
    virtual void virtualMethod4() {
        std::cout << "DerivedClass::virtualMethod4()" << std::endl;
    }
    virtual void virtualMethod5() {
        std::cout << "DerivedClass::virtualMethod5()" << std::endl;
    }
};

void PrintVtable(void** vtable) {
    // 输出虚函数表中每个函数的地址
    for (int i = 0; vtable[i] != nullptr; i++) {
        // 最多调用五个函数，怕访问到虚函数表非法的地址，因为就五个函数
        if (i >= 5)  {
            return;
        }
        std::cout << "Function " << i << ": " << vtable[i] << std::endl;
        // 将虚函数表中的虚函数转换为函数指针，然后进行调用
        Func func = (Func) vtable[i];
        func();
    }
}

int main() {
    MyDerivedClass obj;

    // 取对象的首地址，然后转换为的指针，就取到了虚函数表指针，指向 obj 对象的虚函数表
    // 因为大多数实现上，虚函数表指针一般都放在对象第一个位置
    void** vtable = *(void***)(&obj);
    std::cout << "DerivedClass Vetable:" << std::endl;
    // 打印子类的虚函数表
    PrintVtable(vtable);

    std::cout << std::endl <<  "BaseClass Vetable:" << std::endl;
    MyBaseClass base_obj;
    void** vbtable = *(void***)(&base_obj);
    // 打印父类的虚函数表
    PrintVtable(vbtable);
    return 0;
}
//特别注意多重继承和虚继承的一些情况
```
### 动态多态底层原理
C++**的动态多态必须满足两个条件**：
- 必须通过基类的指针或者引用调用虚函数。（用派生类的指针，编译时确定对象类型为派生类型，直接生成调用的派生类虚函数的代码即可，为静态绑定）
- 被调用的函数是虚函数，且必须完成对基类虚函数的重写。
## C++的纯虚函数
一般在基类定义，则基类变为抽象基类，一般用于强制派生类实现纯虚函数的行为（派生类不实现则也是抽象类，不能有直接实例对象，可以指针或者引用转换，实现多态）。
## 为什么C++的构造函数不能是虚函数
- 语法层面来说，虚函数的目的是多态，派生类覆盖基类。而构造函数负责初始化类的对象，即时派生类也会调用基类的构造函数进行初始化，所以没有覆盖必要，所以不需要构造函数为虚函数。
- 虚函数表的机制来说,虚函数表是类独有的，而不是对象独有的。==**特别注意的是纯虚函数的情况，纯虚函数在虚函数表中是有条目的，但是和虚函数有准确的目标地址不同的是，纯虚函数的条目所存储的值是无效地址。所以不能将有纯虚函数条目的虚函数表对应的类进行实例化！**==
## 为什么 C++ 基类析构函数需要是虚函数？
```c++
#include <iostream>

class Base {
public:
    // 注意，这里的析构函数没有定义为虚函数
    ~Base() {
        std::cout << "Base destructor called." << std::endl;
    }
};

class Derived : public Base {
public:
    Derived() {
        resource = new int[100]; // 分配资源
    }

    ~Derived() {
        std::cout << "Derived destructor called." << std::endl;
        delete[] resource; // 释放资源
    }

private:
    int* resource; // 存储资源的指针
};

int main() {
    Base* ptr = new Derived();
    delete ptr; // 只会调用Base的析构函数，Derived的析构函数不会被调用
    return 0;
}
//如上基类的析构函数不是虚函数，则没有加入到虚函数表中，所以基类指针指向派生类对象，在删除这个基类指针时，没有多态现象，所以只会调用基类的析构函数，派生类析构函数不会被调用而导致派生类的一部分资源没有被释放。所以这里需要将基类的析构函数设置为虚函数，这样即使在释放指向派生列对象的基类指针的时候，也会运用多态现象，去调用派生类的析构函数，派生类析构函数会正确的释放所有资源！
```
**那为什么默认的析构函数不是虚函数**？
原因是由于，不可能创建一个类就将其作为基类，所以不会默认将其析构函数定义为虚函数，因为虚函数会有虚函数指针以及虚函数表等额外的一些内存开销。==所以当使用者定义基类时，我们才需要有意识的将其析构函数定义为虚函数==！
**零成本抽象原则**？
- 不需要为没有使用到的语言特性付出代价。（如果一个类不会被其他类继承，没有必要设置析构函数为虚函数）
- 使用某种语言特性，不会带来运行时的代价。（虚函数一但使用，单来虚函数指针和虚函数表的内存开销）
## 为什么C++的成员模板函数不能是virtual的？
即一个类里面的成员函数不能既是template模板，又不能时virtual的。
==涉及到具体的编译链接的问题。对于c/c++来说，各文件编译时是单独编译的，最后再链接器链接起来。而对于一个类的内存布局（虚函数表的内存布局）来说，在编译时被完全确定；而对于模板函数，其最后到底会instance实例化多少个函数，需要等到所有的编译文件都链接在一起才能确定，即在链接后确定。所以对模板函数是虚函数的情况来说是矛盾的，一个在编译期就需要确定虚函数表的布局。一个确实在链接期后才能确定实例化的个数，所以C++规定了类的成员函数不能既是模板函数又是虚函数！==
## sizeof一个空类有多大
```c++
class Empty {};

int main() {
    Empty e1;
    Empty e2;
    std::cout << "Size of Empty class: " << sizeof(Empty) << std::endl;//一般输出为1字节
}

//需要确保空类的两个对象的地址不一，所以需要1字节保留
class Empty { };

void f()
{
    Empty a, b;
    if (&a == &b) cout << "impossible: report error to compiler supplier";

    Empty* p1 = new Empty;
    Empty* p2 = new Empty;
    if (p1 == p2) cout << "impossible: report error to compiler supplier";
}

//如果一个空类做基类，那么在派生类中不需要用一个单独的字节来表示；这是一个有趣的规则，即空基类在派生类中是没有一个单独字节来表示的！;即时派生类也是空类，也不是需要2字节表示，而也只需要1字节！
struct X : Empty {
  int a;
  // ...
};

void f(X* p)
{
  void* p1 = p;
  void* p2 = &p->a;
  if (p1 == p2) cout << "nice: good optimizer";//大多数编译器有这个空基类优化
}
```

# C++内存管理
## c/c++的内存是什么？
涉及内存的编制（统一从小到大进行编址），内存的地址空间（计算机的位数，cpu/寄存器的位数），变量的本质（存储在内存的一个区域里面，大小端序的问题，一般大端符合阅读习惯）。
## 深入理解C/C++的指针
[为什么指针是C语言的灵魂](https://mp.weixin.qq.com/s/d1ng1-AFFdPGLWDXVKPJZg)
### 指针是什么东西？
- 变量放在虚拟内存中！
- 指针本质，指向变量的首地址，需要类型的原因是需要对指向的地址存储的类型进行解释！
- 解引用，这也是为什么将各种类型指针可以互相转换，但是各种指针类型对指向的这片内存区域的解释是不同的。
```c++
float f = 1.0;
short c = *(short*)&f;
```
- **如下是一个小问题**：
![img](c++后台开发面试重点知识.assets/0081Kckwgy1gk8bk5n64oj30a508xdi7.jpg)
为什么float类型的值写入然后读出不一样。本质是因为buffer是一个char类型的指针，当对其进行解引用，只会取第一个字节的数据出来（看成是一个buffer数组，直接buffer只会取第一个）;所以这里需要的是