这里主要对util和include下的数据结构进行解析，把基本数据结构进行解耦以及分析作用。

## 1.arena内存池，涉及arena.h arena.cc

```c++
class Arena {
 public:
  Arena();

  Arena(const Arena&) = delete;
  Arena& operator=(const Arena&) = delete;

  ~Arena();

  // Return a pointer to a newly allocated memory block of "bytes" bytes.
  char* Allocate(size_t bytes);

  // Allocate memory with the normal alignment guarantees provided by malloc.
  char* AllocateAligned(size_t bytes);

  // Returns an estimate of the total memory usage of data allocated
  // by the arena.
  size_t MemoryUsage() const {
    return memory_usage_.load(std::memory_order_relaxed);
  }

 private:
  char* AllocateFallback(size_t bytes);
  char* AllocateNewBlock(size_t block_bytes);

  // Allocation state
  char* alloc_ptr_;//指向当前block的内存位置的指针
  size_t alloc_bytes_remaining_;//记录当前块剩余的字节数

  // Array of new[] allocated memory blocks
  std::vector<char*> blocks_;//将block串成一个vector

  // Total memory usage of the arena.
  //
  // TODO(costan): This member is accessed via atomics, but the others are
  //               accessed without any locking. Is this OK?
  std::atomic<size_t> memory_usage_;//记录arena总的使用的内存大小,适用于多线程的原子操作。
};
//对于初始化，alloc_ptr初始化为nullptr， memory_usage_，alloc_bytes_remaining_皆初始化为0.
//static const int kBlockSize = 4096
```

如上所示名为arena内存池的类结构，开始是进行拷贝构造以及拷贝赋值运算符的delete。

（1）Allocate函数，返回收到一个具体的指针。三种情况：

申请内存小于等于当前块的alloc_bytes_remaining_，则立即分配，alloc_ptr,alloc_bytes_remaining随之变化。

申请内存大于当前块的alloc_bytes_remaining_，但申请内存大于kBlocksize/4 ；则重新分配一个申请大小的block块。alloc_ptr,alloc_bytes_remaining_没有变化。

申请内存大于当前块的alloc_bytes_remaining_，但申请内存不大于kBlocksize/4 ；则重新分配一个新的Kblocksize大小的块，alloc_ptr,alloc_bytes_remaining随之变化。==此时存在内存的碎片化浪费==。

（2）AllocateAligned函数

```c++
char* Arena::AllocateAligned(size_t bytes) {
  const int align = (sizeof(void*) > 8) ? sizeof(void*) : 8;//指针类型的size和系统位数有关，64位为8字节；32位4字节
  static_assert((align & (align - 1)) == 0,
                "Pointer size should be a power of 2");//判断该值是否为2的倍数，编译时静态断言判断
  size_t current_mod = reinterpret_cast<uintptr_t>(alloc_ptr_) & (align - 1);//转为无符号整型指针，获取地址的二进制表示与align进行地址偏移量计算
  size_t slop = (current_mod == 0 ? 0 : align - current_mod);
  size_t needed = bytes + slop;
  char* result;
  if (needed <= alloc_bytes_remaining_) {
    result = alloc_ptr_ + slop;
    alloc_ptr_ += needed;`
    alloc_bytes_remaining_ -= needed;
  } else {
    // AllocateFallback always returned aligned memory
    result = AllocateFallback(bytes);
  }
  assert((reinterpret_cast<uintptr_t>(result) & (align - 1)) == 0);
  return result;
}
//其代码实现是这样的。。。。
```

**测试**：arena_test.cc

## (2)布隆过滤器：bloom.cc,hash.cc,hash.h,fileter_policy.h(为一个基类接口实现)

初版：leveldb的布隆过滤器的实现是基于类似于 murmur hash的hash策略来实现的。

首先对于hash.cc:

```c++
uint32_t Hash(const char* data, size_t n, uint32_t seed) {
  // Similar to murmur hash
  const uint32_t m = 0xc6a4a793;
  const uint32_t r = 24;
  const char* limit = data + n;
  uint32_t h = seed ^ (n * m);

  // Pick up four bytes at a time
  while (data + 4 <= limit) {
    uint32_t w = DecodeFixed32(data);
    data += 4;
    h += w;
    h *= m;
    h ^= (h >> 16);
  }

  // Pick up remaining bytes
  switch (limit - data) {
    case 3:
      h += static_cast<uint8_t>(data[2]) << 16;
      FALLTHROUGH_INTENDED;
    case 2:
      h += static_cast<uint8_t>(data[1]) << 8;
      FALLTHROUGH_INTENDED;
    case 1:
      h += static_cast<uint8_t>(data[0]);
      h *= m;
      h ^= (h >> r);
      break;
  }
  return h;
}
//对这个源码，其根据随机的种子和普通的加减乘除以及数据类型的转换操作来进行最终哈希值的获得，整个其实就是哈希函数。

```

对于bloom.cc，其实现有：

static uint32_t BloomHash(const Slice& key) ##用于根据key调用哈希函数

class BloomFilterPolicy : public FilterPolicy ##用于创建布隆过滤器的策略的类，有默认函数，CreateFilter，KeyMayMatch的实现。

const FilterPolicy* NewBloomFilterPolicy(int bits_per_key)

其总结就是:CreateFilter使用BloomHash，在根据实际存储的key键值数据创建一个array，这个array就是创建的布隆过滤器，KeyMayMatch基于上面创建的array，对新的key使用同样的方法，来判断对应位是否存在，循环k次，对应布隆过滤器算法的其实是创建了k个哈希函数。这样来判断key值是否在存储的key中。

贴上void CreateFilter(const Slice* keys, int n, std::string* dst) const override 代码：

```c++
 void CreateFilter(const Slice* keys, int n, std::string* dst) const override {
    // Compute bloom filter size (in both bits and bytes)
    size_t bits = n * bits_per_key_;

    // For small n, we can see a very high false positive rate.  Fix it
    // by enforcing a minimum bloom filter length.
    if (bits < 64) bits = 64;

    size_t bytes = (bits + 7) / 8;
    bits = bytes * 8;

    const size_t init_size = dst->size();
    dst->resize(init_size + bytes, 0);
    dst->push_back(static_cast<char>(k_));  // Remember # of probes in filter
    char* array = &(*dst)[init_size];
    for (int i = 0; i < n; i++) {
      // Use double-hashing to generate a sequence of hash values.
      // See analysis in [Kirsch,Mitzenmacher 2006].
      uint32_t h = BloomHash(keys[i]);
      const uint32_t delta = (h >> 17) | (h << 15);  // Rotate right 17 bits
      for (size_t j = 0; j < k_; j++) {
        const uint32_t bitpos = h % bits;
        array[bitpos / 8] |= (1 << (bitpos % 8));
        h += delta;
      }
    }
  }
```

测试：bloom_test.cc

## (3)slice 涉及slice.h文件

```c++
class LEVELDB_EXPORT Slice {
 public:
  // Create an empty slice.
  Slice() : data_(""), size_(0) {}

  // Create a slice that refers to d[0,n-1].
  Slice(const char* d, size_t n) : data_(d), size_(n) {}

  // Create a slice that refers to the contents of "s"
  Slice(const std::string& s) : data_(s.data()), size_(s.size()) {}

  // Create a slice that refers to s[0,strlen(s)-1]
  Slice(const char* s) : data_(s), size_(strlen(s)) {}     

  // Intentionally copyable.
  Slice(const Slice&) = default;
  Slice& operator=(const Slice&) = default;

  // Return a pointer to the beginning of the referenced data
  const char* data() const { return data_; }

  // Return the length (in bytes) of the referenced data
  size_t size() const { return size_; }

  // Return true iff the length of the referenced data is zero
  bool empty() const { return size_ == 0; }

  // Return the ith byte in the referenced data.
  // REQUIRES: n < size()
  char operator[](size_t n) const {
    assert(n < size());
    return data_[n];
  }

  // Change this slice to refer to an empty array
  void clear() {
    data_ = "";
    size_ = 0;
  }

  // Drop the first "n" bytes from this slice.
  void remove_prefix(size_t n) {
    assert(n <= size());
    data_ += n;
    size_ -= n;
  }

  // Return a string that contains the copy of the referenced data.
  std::string ToString() const { return std::string(data_, size_); }

  // Three-way comparison.  Returns value:
  //   <  0 iff "*this" <  "b",
  //   == 0 iff "*this" == "b",
  //   >  0 iff "*this" >  "b"
  int compare(const Slice& b) const;

  // Return true iff "x" is a prefix of "*this"
    //判断x是否是其子slice，返回bool类型 
  bool starts_with(const Slice& x) const {
    return ((size_ >= x.size_) && (memcmp(data_, x.data_, x.size_) == 0));
  }

 private:
  const char* data_;
  size_t size_;
};
```

slice支持string，const char* ，以及加上指定的截断大小来进行构造。有data(),size(),empty(),operator[]函数实现。clear函数清空数据。









































