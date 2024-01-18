# leveldb主要参数
# leveldb主要参数
## 特点
- 键和值是任意字节数组。
- 数据按键排序存储。
- 调用者可以提供自定义比较函数来覆盖排序顺序。
- 基本操作包括 Put(key,value)、Get(key)、Delete(key)。
- 可以在一个原子批次中进行多次更改。
- 用户可以创建瞬时快照，以获得一致的数据视图。
- 数据支持向前和向后迭代。
- 数据使用 Snappy 压缩库自动压缩，但也支持 Zstd 压缩。
- 外部活动（文件系统操作等）通过虚拟接口进行转发，因此用户可以自定义操作系统的交互方式。
## 限制
- 这不是一个 SQL 数据库。它没有关系数据模型，不支持 SQL 查询，也不支持索引。
- 一次只能有一个进程（可能是多线程进程）访问一个特定的数据库。//请自己做好并发。
- 该库没有内置客户端-服务器支持。需要这种支持的应用程序必须将自己的服务器封装在库中。
## 性能估计
- 设置：百万并发量，keys:16B;value:100B(压缩后大约50B)
- 写表现：
```c
fillseq      :       1.765 micros/op;   62.7 MB/s//顺序填充，每次操作平均1.765us
fillsync     :     268.409 micros/op;    0.4 MB/s (10000 ops)//同步填充
fillrandom   :       2.460 micros/op;   45.0 MB/s//随机填充
overwrite    :       2.380 micros/op;   46.5 MB/s//数据覆盖
//写性能还是可以的
```
- 读表现:读操作受页高速缓存（磁盘缓冲）不存在的数据成本影响（可能需要去磁盘寻道寻找）。写操作不受影响。
```c
readrandom  : 16.677 micros/op;  (approximately 60,000 reads per second)//大约17us/op
readseq     :  0.476 micros/op;  232.3 MB/s//顺序读
readreverse :  0.724 micros/op;  152.9 MB/s//反向读
//是否压缩数据，也能影响性能
readrandom  : 9.775 micros/op;  (approximately 100,000 reads per second before compaction)
readrandom  : 5.215 micros/op;  (approximately 190,000 reads per second after compaction)
//一般来说压缩数据读性能更好，但是当页高速缓存足够的时候，我们使用非压缩数据可节省部分读取数据解压缩的时间。
```

# leveldb log format
一个record可以对应有多个fragments。
```c
//以下是一个记录或片段的元数据元数据信息，占据7B。
block := record* trailer?
record :=
  checksum: uint32     // crc32c of type and data[] ; little-endian
  length: uint16       // little-endian
  type: uint8          // One of FULL, FIRST, MIDDLE, LAST
  data: uint8[length]

//以下是对一个记录或者片段的类型定义,FULL对应是一个完整的record放在一个bolck块上。当record需要划分为多个片段时（由于一个block放不下）。第一个片段类型为FIRST，中间片段为MIDDLE，最后一个片段为LAST。
FULL == 1
FIRST == 2
MIDDLE == 3
LAST == 4

//A类型为FULL类型，放在第一个块上（1000+7）;对于B来说需要划分为3个片段（第一个块的片段为FULL；第二个块放B的第二个段，为MIDDLE；第三个块放B的第三个段为LAST。此时的情况是A，B占3个块，剩余字节=32x1024x3-(7+1000)-(97270+3*7)=6B);6B放不下C记录的元数据。直接填充0跳过。C类型全放到第四个块。
A: length 1000
B: length 97270
C: length 8000
```
**好处**：无需启发式同步，只需要转到下一个数据块边界扫描。寻找下一个记录边界，只需要查看当前的元信息是FULL/FIRST。则对应一个新记录。==大记录不需要缓冲，拆分为片段，也是块的形式==。
**坏处**：小记录的紧凑存储（元数据就占据7B，block尾部的6B的浪费等）；数据非压缩，可以添加新的记录类型来进行标识。

# leveldb impl
每个leveldb数据库由存储在一个目录中的一组文件表示。
## 排序表
排序表 (*.ldb) 存储按键排序的条目序列。每个条目要么是键的值，要么是键的删除标记。(保留删除标记是为了隐藏旧排序表中的过时值）。
对应的是sst table，level0为2/4MB，可能有冗余key；下次1~n级每级10的级数次方MB大小;上一级level到达大小后，需要选出至少一个sst，在对应的键值范围进行合并，显然level0后面的level都不具有冗余keys。
## manifest
MANIFEST 文件列出了构成每个级别的排序表集、相应的键范围以及其他重要元数据。每当数据库重新打开时，都会创建一个新的 MANIFEST 文件（文件名中嵌入一个新编号）。MANIFEST 文件的格式为日志，对服务状态所做的更改（如文件的添加或删除）都会附加到该日志中。
## Current
CURRENT 是一个简单的文本文件，包含最新MANIFEST文件的名称.(因为manifest会一直更新，所以current可以指向最新的manifest)。
## Info logs
信息会打印到名为 LOG 和 LOG.old 的文件中。
## level 0
当日志文件（WAL）超过一定大小（默认为 4MB）时：创建一个全新的内存表（memtble，原来的memtable打上immutable的标志）和日志文件（WAL），并将未来的更新导向此处。
将之前 memtable（immutable） 的内容写入 sstable。
丢弃 memtable。
删除旧日志文件和旧 memtable。
将新的 sstable 添加到年轻（0 级）级别。
## Compactions
超出当前层的大小限制，选取一个或几个sstable和对应的键值范围的下一层的sstable进行compact合并。
合并规则：
- 选取L层部分和L+1层重叠部分进行合并。当输出文件的关键字范围可以和10个以上的L+2层文件重叠时，需要处理，合并成L+2一级，防止下一次L+1级文件压缩和L+2级重叠范围过多。
- 对于每个层级L。需要记住上次压缩的结束的key；下一次压缩需要选取该结束密钥后下一个开始文件来进行compact操作。
- compact会删除掉被覆盖的值，即删除当前L层的覆盖的值，但是如果没有键重叠的更高级别，还是会放弃删除标记。
## Timing
对于用户来说，只有level0到下一级的压缩会直观的影响用户的体验（immutable 写入level0，需要保证level0的充足；所以level0的合并时间是个问题）；所以就是增大level0的文件数，减少level0的合并频率。
但是增加大大量的level0文件数，就会影响读操作的效果，因为需要一定的解决办法。
- 解决方案 1：为了减少这个问题，我们不妨在 0 级文件数量较多时提高日志切换阈值（即增大缓存，减少写入频率，较少sst数量）。不过这样做的缺点是，阈值越大，我们就需要更多内存来保存相应的 memtable。
- 解决方案 2：当 0 级文件数量增加时，我们可以人为降低写入速率。
- 解决方案 3：我们要努力降低大范围合并的成本（合并成本降低）。也许大多数 0 级文件都会在缓存中保留未压缩的数据块，我们只需担心合并迭代器的 O(N) 复杂性。
## Number of files
文件数量的影响，可避免创建2MB的文件，而为更高级别创造更大的文件，减少文件数。但事实上现代操作系统中，文件数量对打开一个文件的时间影响较小，其实没什么分片的必要？
## leveldb的recovery
读取 CURRENT 以查找最新提交的 MANIFEST 文件名
读取已命名的 MANIFEST 文件
清理陈旧文件
我们可以在这里打开所有的 sstable，但最好还是偷个懒...
将日志块转换为新的 0 级 sstable（理解是断电等异常，将WAL日志块的数据又新写到memtable中，继续走流程；最后可能转换为新的sstable。当然写入到sstable中后，WAL日志块可以删除，创建新的WAL块来处理）
开始引导新的写入到新的日志文件，并恢复 sequence#
## garbage collection of files（垃圾回收
每次压缩结束和恢复结束时都会调用 RemoveObsoleteFiles()。它会查找数据库中所有文件的名称。它会删除所有非当前日志文件的日志文件（WAL文件）。删除所有未从某个级别引用的表文件，这些文件也不是当前压缩的输出文件。
# leveldb table_format（待看
# leveldb index（实际使用
leveldb的key有序，根据比较器函数指定排序。
```c
//下面代码是正常的创建一个leveldb的数据库，Opening A Database
#include <cassert>
#include "leveldb/db.h"
leveldb::DB* db;
leveldb::Options options;
options.create_if_missing = true;
leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
assert(status.ok());
options.error_if_exists = true;//当数据库存在时，报错。

//Status，判断操作是否成功
leveldb::Status s = ...;//接各种操作
if (!s.ok()) cerr << s.ToString() << endl;

//Closing A Database
... open the db as described above ...
... do something with db ...
delete db;

//Reads And Writes,提供put，get，delete
std::string value;
leveldb::Status s = db->Get(leveldb::ReadOptions(), key1, &value);
if (s.ok()) s = db->Put(leveldb::WriteOptions(), key2, value);
if (s.ok()) s = db->Delete(leveldb::WriteOptions(), key1);

//atomic Updates,上面读写，当put完后，delete之前进程dump掉，则数据库多个key对应同一个值，用原子操作解决；和rocksdb类似
#include "leveldb/write_batch.h"
...
std::string value;
leveldb::Status s = db->Get(leveldb::ReadOptions(), key1, &value);
if (s.ok()) {
  leveldb::WriteBatch batch;
  batch.Delete(key1);
  batch.Put(key2, value);
  s = db->Write(leveldb::WriteOptions(), &batch);
}

//默认情况下，对 leveldb 的每次写入都是异步的：将写入从进程推送到操作系统后返回。从操作系统内存到底层持久化存储的传输是异步进行的。可以为特定的写操作开启同步标志，使写操作在写入的数据一直推送到持久化存储后才返回。
leveldb::WriteOptions write_options;
write_options.sync = true;//同步写入
db->Put(write_options, ...);
//异步写入的速度通常是同步写入的一千倍以上。异步写入的缺点是，机器崩溃可能会导致最后几次更新丢失。
//异步写入通常可以安全使用。崩溃后重新批量加载处理丢失更新；混合更新；每N次写入同步，发生崩溃，则从最后一次的同步开始加载，（同步写入更新标记，描述崩溃启动的标志）
//writebatch提高替代异步写入的方法，即多个更新可以放在同一个 WriteBatch 中，并使用同步写入（即 write_options.sync 设置为 true）一起应用。
```
## concurrrency
一个数据库一次只能由一个进程打开。leveldb 实现会从操作系统获取一个锁，以防止误操作。在一个进程中，多个并发线程可以安全地共享同一个 leveldb::DB 对象。也就是说，不同的线程可以在同一数据库中写入或获取迭代器或调用 Get，而无需任何外部同步（leveldb 实现会自动完成所需的同步）。但其他对象（如 ==Iterator 和 WriteBatch==）可能需要外部同步。如果两个线程共享此类对象，它们必须使用自己的锁定协议来保护对该对象的访问。
## Iteration
```c
//使用迭代器遍历打印所有的key和value
leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
for (it->SeekToFirst(); it->Valid(); it->Next()) {
  cout << it->key().ToString() << ": "  << it->value().ToString() << endl;
}
assert(it->status().ok());  // Check for any errors found during the scan
delete it;

//只查询在[start,limit)范围内的key
for (it->Seek(start);
   it->Valid() && it->key().ToString() < limit;
   it->Next()) {
  ...
}

//反向遍历，比正向遍历要满一些
for (it->SeekToLast(); it->Valid(); it->Prev()) {
  ...
}
//
```
## Snapshots
快照为键值存储的整个状态提供一致的只读视图。ReadOptions::snapshot 可以为非空，以表示读取操作应在特定版本的数据库状态上进行。如果 ReadOptions::snapshot 为空，读取将在当前状态的隐式快照上进行。
```c
leveldb::ReadOptions options;
options.snapshot = db->GetSnapshot();
... apply some updates to db ...
leveldb::Iterator* iter = db->NewIterator(options);
... read using iter to view the state when the snapshot was created ...
delete iter;
db->ReleaseSnapshot(options.snapshot);//不需要快照立即释放，允许实现摆脱为支持读取快照维护的状态  
```
## Slice
在leveldb中，键和值都是leveldb::Slice的实例，Slice包含长度和一个指向外部字节数组的指针。返回Slice比返回std::string更方便（不需要复制很大的键和值）;slice和string之间方便转换
```c
leveldb::Slice s1 = "hello";
std::string str("world");
leveldb::Slice s2 = str;

//string和Slice的转换
std::string str = s1.ToString();
assert(str == std::string("hello"));

//请确保Slice指向的外部字节数组，在使用时是存在的！
leveldb::Slice slice;
if (...) {
  std::string str = ...;
  slice = str;
}
Use(slice);//str在局部if出来进行了析构，此时str是不存在的。
```
## Comparators
自定义比较器的实现
```c
//如下定义了一个比较器，可以实现的是如果key都是两个数字组成，按第一个排序，第二个打破平均排序
class TwoPartComparator : public leveldb::Comparator {
 public:
  // Three-way comparison function:
  //   if a < b: negative result
  //   if a > b: positive result
  //   else: zero result
  int Compare(const leveldb::Slice& a, const leveldb::Slice& b) const {
    int a1, a2, b1, b2;
    ParseKey(a, &a1, &a2);
    ParseKey(b, &b1, &b2);
    if (a1 < b1) return -1;
    if (a1 > b1) return +1;
    if (a2 < b2) return -1;
    if (a2 > b2) return +1;
    return 0;
  }
 // Ignore the following methods for now:
  const char* Name() const { return "TwoPartComparator"; }
  void FindShortestSeparator(std::string*, const leveldb::Slice&) const {}
  void FindShortSuccessor(std::string*) const {}
};

//下面是使用这个比较器
TwoPartComparator cmp;
leveldb::DB* db;
leveldb::Options options;
options.create_if_missing = true;
options.comparator = &cmp;
leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
...
```
## 向后兼容性 backward compatibility
比较器名称方法的结果会在创建数据库时附加到数据库中，并在以后每次打开数据库时进行检查。如果名称发生变化，leveldb::DB::Open 调用将失败。因此，只有在新的键格式和比较函数与现有数据库不兼容的情况下（打开数据库，检查键格式和比较函数是否和现有数据库兼容，不兼容再修改），才可更改名称，并且可以丢弃所有现有数据库的内容。

不过，只要事先稍加规划，你还是可以随着时间的推移逐渐发展你的密钥格式。例如，你可以在每个密钥的末尾存储一个版本号（对于大多数用途来说，一个字节就足够了）。当你想改用新的密钥格式时（例如，在 TwoPartComparator 处理的密钥中添加可选的第三部分），(a) 保留相同的比较器名称 (b) 为新密钥增加版本号 (c) 改变比较器函数，使其使用密钥中的版本号来决定如何解释密钥。？？？？？
## Performance
可以通过更改 include/options.h 中定义的类型的默认值来调整性能。
## block size
==leveldb 会将相邻的密钥分组到同一个数据块中，这样的数据块是传输到持久存储区和从持久存储区传输的单位==。默认数据块大小约为 4096 个未压缩字节。主要对数据库内容进行批量扫描的应用程序可能希望增大该大小。如果性能测量结果表明数据块大小有所改善，则经常对小值进行点读取的应用程序可以改用较小的数据块大小。使用小于 1 千字节或大于几兆字节的数据块并无太大益处。还需注意的是，==块大小越大，压缩效果越好==。
## Compression 
在写入持久性存储之前，每个数据块都会被单独压缩。由于默认的压缩方法速度非常快，因此默认情况下压缩是开启的，对于无法压缩的数据，压缩会自动关闭。
```c
leveldb::Options options;
options.compression = leveldb::kNoCompression;//选取一个压缩算法
... leveldb::DB::Open(options, name, ...) ....
```
## Cache
数据库的内容存储在文件系统中的一组文件中，每个文件存储一个压缩块序列。如果 options.block_cache 为非空，它将用于缓存常用的未压缩块内容。
```c
#include "leveldb/cache.h"
leveldb::Options options;
options.block_cache = leveldb::NewLRUCache(100 * 1048576);  // 100MB cache,用于缓存常用的未压缩块的内容
leveldb::DB* db;
leveldb::DB::Open(options, name, &db);
... use the db ...
delete db
delete options.block_cache;
```
请注意，==缓存保存的是未压缩的数据，因此其大小应与应用级数据大小一致，不会因压缩而减小==。(压缩块的缓存由操作系统缓冲区缓存或客户端提供的任何自定义 Env 实现）
```c
leveldb::ReadOptions options;
options.fill_cache = false;
leveldb::Iterator* it = db->NewIterator(options);
for (it->SeekToFirst(); it->Valid(); it->Next()) {
  ...
}
delete it;
//当批量更新时，可能的是不想要cache（防止读取内容占用大量cache），此时可以设置上面的禁用cache来进行读取。
```
## key layout
请注意，磁盘传输和缓存的单位是块。相邻的键值（根据数据库排序顺序）通常会放在同一个块中。因此，应用程序可以通过将一起访问的键放在彼此附近，并将不常用的键放在键空间的单独区域来提高性能。
```c
filename -> permission-bits, length, list of file_block_ids//文件由长度，块的编号等组成
file_block_id -> data//根据块编号来获取数据。
//在文件的keys和block的keys之间有一个映射，避免文件keys大量的字符，存储直接使用映射的轻量级字符。
```
## Fileters
由于 leveldb 数据在磁盘上的组织方式，一次 Get() 调用可能会涉及从磁盘上的多次读取。可选的过滤策略（FilterPolicy）机制可用于大幅减少磁盘读取次数。
```c
leveldb::Options options;
options.filter_policy = NewBloomFilterPolicy(10);//使用布隆过滤器
leveldb::DB* db;
leveldb::DB::Open(options, "/tmp/testdb", &db);
... use the database ...
delete db;
delete options.filter_policy;
//布隆过滤器可将Get()调用所需的不必要磁盘的读取次数减少约100倍。增大密钥位数导致更大减少->内存的更多使用。
```
==那些工作集无法容纳在内存中且需要进行大量随机读取的应用程序设置过滤器策略==
比较器和过滤策略要保持兼容（特别对于自定义的过滤器和比较器），对keys来说，是否忽略尾部空格等
```c
class CustomFilterPolicy : public leveldb::FilterPolicy {
 private:
  leveldb::FilterPolicy* builtin_policy_;

 public:
  CustomFilterPolicy() : builtin_policy_(leveldb::NewBloomFilterPolicy(10)) {}
  ~CustomFilterPolicy() { delete builtin_policy_; }

  const char* Name() const { return "IgnoreTrailingSpacesFilter"; }

  void CreateFilter(const leveldb::Slice* keys, int n, std::string* dst) const {
    // Use builtin bloom filter code after removing trailing spaces
    std::vector<leveldb::Slice> trimmed(n);
    for (int i = 0; i < n; i++) {
      trimmed[i] = RemoveTrailingSpaces(keys[i]);
    }
    builtin_policy_->CreateFilter(trimmed.data(), n, dst);//使用布隆过滤器
  }
};
```
## checksums 校验和检查
leveldb在文件系统存储的所有数据关联校验和。
- 读取选项::verify_checksums（校验和）可设置为 true，以强制对代表特定读取从文件系统读取的所有数据进行校验和验证。默认情况下，不会进行此类校验。
- 在打开数据库之前，可以将 Options::paranoid_checks 设置为 true，以便数据库实现在检测到内部损坏时立即引发错误。错误可能在打开数据库时发生，也可能在之后的其他数据库操作中发生，这取决于数据库的哪个部分被破坏。==默认情况下，偏执检查是关闭的，这样即使数据库的部分持久存储已损坏，也可以使用数据库==。
## Approximate Sizes
```c
leveldb::Range ranges[2];
ranges[0] = leveldb::Range("a", "c");
ranges[1] = leveldb::Range("x", "z");
uint64_t sizes[2];
db->GetApproximateSizes(ranges, 2, sizes);//获取指定键值范围的数据占用空间大概大小
```
