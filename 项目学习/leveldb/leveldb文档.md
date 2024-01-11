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
# Current
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
# leveldb file format（待看
## leveldb index（实际使用
leveldb的key有序，根据比较器函数指定排序。
