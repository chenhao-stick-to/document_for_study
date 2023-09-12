## c++知识

(1)Arena(const Arena&) = delete;
  Arena& operator=(const Arena&) = delete;该函数删除了实现，说明不需要进行复制构造以及赋值运算符的构造。

（2）原子操作std::atomic<size_t> memory_usage_;适用于多线程对同一个值进行读取和更新操作，而不会发生多线程的数据竞争。

（3）sizeof（指针），其大小为4还是8取决于系统是32位还是64位。

（4）如果namespace为匿名空间，那么该空间声明的所有东西只能用于本源文件，而不能在其他地方进行使用。

（5）explicit用于显式申明构造函数，防止该类进行隐式的类型转换。

```
explicit BloomFilterPolicy(int bits_per_key) : bits_per_key_(bits_per_key)d
int bits = 10;
BloomFilterPolicy filter = bits;  // 隐式类型转换

int bits = 10;
BloomFilterPolicy filter(bits);  // 显式调用构造函数
```



## 代码风格

（1）对类来说尽量所有函数（返回值含义说明）和声明变量有解释。

（2）对于每一对括号，第一个和条件、函数体等在同一行，第二个单独结尾一行。

（3）namespace结尾有说明是哪个namespace，使用#ifndef #/#ednif需要说明是那个#ifdndef的变量（当里面代码体过大无法一次看清时）。