github地址：https://github.com/yanfeizhang/coder-kung-fu
# 第1章 绪论
####待解决问题
- 过多的TIME_WAIT?一个TIME_WAIT的连接会有哪些开销？
- 长连接开销？短连接/长连接；一条空闲的TCP连接有多大开销?
- cpu消耗？端口不充足时，connect系统调用的CPU消耗大幅增加?
- 不同的语言网络性能差别大？epoll工作原理/同步阻塞，异步阻塞网络IO
- 访问127.0.0.1网卡？本机网路IO和跨机相比的优势？数据过网卡？性能优势？节约的开销？
- 软中断和硬中断，为什么软中断的收发两类中断的数量相差巨大。Linux的top查看CPU开销;us用户空间占比,sy内核空间占比,hi硬中断占比,si软中断占比。
- 零拷贝怎么回事？
- DPDK 旁路网络IO技术，不过内核？
# 第2章 内核时如何接收网络包的
## 相关实际问题
- RingBuffer是什么？其分配的内存为什么会丢包，是预分配还是动态增长
- 网络软/硬中断的协作
- Linux的ksoftirqd内核线程作用？目前是一个CPU一个对应的内核线程
- 多队列的网卡能提升网络性能？
- tcpdump的使用；iptable/netfilter包过滤器工作在内核哪一层？tcpdump能转到iptable过滤掉的包吗？
## 数据如何从网卡到协议栈？
### Linux网络收发包总览
针对TCP/IP的五层模型;==Linux内核/网卡驱动主要针对链路层，网络层，传输层三层上的功能
Linux视角下的网络模型：
![image-20231009161823964](linux网络.assets/image-20231009161823964.png)
下面是内核收包的路径：
![image-20231009162825172](linux网络.assets/image-20231009162825172.png)
解析：
- 网卡接收到来自外部的数据帧
- 网卡把数据帧DMA到内存；==这里DMA传送数据到内存，不需要通知CPU吗？==
- 网卡再向内存发出硬中断
- CPU相应硬中断，经历上半部的中断后，快速释放CPU,发出软中断
- ksoftirqd线程处理软中断，调用网卡驱动程序注册的poll函数轮询收包
- 数据帧从RingBuffer摘下保存为skb（网络模块的核心结构体）
- 协议层处理数据帧，处理完的data放到socket的接收接收对立中。
- 内核唤醒用户进程（数据到达
### Linux启动
在Linux驱动，内核协议栈等模块能接收网卡数据包前，需要准备工作。例如网卡设备子系统初始化，注册的各个协议处理函数，ksoftirqd内核线程的创建。
#### 创建ksotfirqd内核线程
![image-20231009165227491](linux网络.assets/image-20231009165227491.png)
内核启动时会自动创建软中断内核处理线程ksoftirqd；在创建后，其会进入线程循环函数，等待软中断的到来。
#### 网络子系统初始化
网络子系统初始化，为每一个CPU初始化softnet_data;也会为两种收发软中断信号注册处理函数
![image-20231009170845429](linux网络.assets/image-20231009170845429.png)
Linux内核采用subsys_initcall来初始化各个子系统。这里的net_dev_init就是网络子系统初始化，其中软中断注册的处理函数也在初始化时使用open_softirq实现，这里注册的<软中端信号，处理函数>二元组，额外使用softirq_vec变量来进行记录。

#### 协议栈注册
![image-20231009214931129](linux网络.assets/image-20231009214931129.png)
inet_add_protocol把tcp，udp的对应处理函数注册到inet_protos数组中，在“dev_add_pack”中ip的处理函数注册到ptype_base哈希表中。
#### 网卡驱动初始化
每个驱动程序，使用module_init向内核注册一个初始化函数。驱动程序被加载，内核自动调用这个初始化函数。
![image-20231009221226887](linux网络.assets/image-20231009221226887.png)
在pci_register_driver调用完成后，Linux内核可以获取驱动的相关信息。
```c
//file: drivers/net/ethernet/intel/igb/igb_main.c

static struct pci_driver igb_driver = {

    .name     = igb_driver_name,
    .id_table = igb_pci_tbl,
    .probe    = igb_probe,
    .remove   = igb_remove,
    ......

};

static int __init igb_init_module(void){

    ......
    ret = pci_register_driver(&igb_driver);
    return ret;

}
```
之后调用igb_probe函数方法，使网卡处于ready状态。igb_probe执行过程：
![image-20231011193516348](linux网络.assets/image-20231011193516348.png)
网卡驱动实现了ethtool所需接口，完成函数地址注册。之所以能使用ethtool查看网络收发包统计，修改网卡自适应模式，调整RX队列的数量和大小，其使用的是网卡驱动的东西。第6步注册net_device_ops变量，包含igb_open，这个函数在网卡启动时调用。
```c
//file: drivers/net/ethernet/intel/igb/igb_main.c

static const struct net_device_ops igb_netdev_ops = {

  .ndo_open               = igb_open,
  .ndo_stop               = igb_close,
  .ndo_start_xmit         = igb_xmit_frame,
  .ndo_get_stats64        = igb_get_stats64,
  .ndo_set_rx_mode        = igb_set_rx_mode,
  .ndo_set_mac_address    = igb_set_mac,
  .ndo_change_mtu         = igb_change_mtu,
  .ndo_do_ioctl           = igb_ioctl,

 ......
```
第7步注册NAPI机制需要的poll函数，即igb_poll函数。
```c
static int igb_alloc_q_vector(struct igb_adapter *adapter,
                  int v_count, int v_idx,
                  int txr_count, int txr_idx,
                  int rxr_count, int rxr_idx){
    ......
    /* initialize NAPI */
    netif_napi_add(adapter->netdev, &q_vector->napi,
               igb_poll, 64);

}
```
#### 启动网卡
 初始化完成就可以启动网卡，启用网卡时，会使用net_device_ops中初始化的变量，如ndo_open会被调用，其指向了igb_open方法。igb_open通常做的事：
![image-20231011200432677](linux网络.assets/image-20231011200432677.png)
==内存，rx/tx队列，ringbuffer关系==？**理解可能有问题**
```c
//file: drivers/net/ethernet/intel/igb/igb_main.c
static int __igb_open(struct net_device *netdev, bool resuming){

    /* allocate transmit descriptors */
    err = igb_setup_all_tx_resources(adapter);

    /* allocate receive descriptors */
    err = igb_setup_all_rx_resources(adapter);

    /* 注册中断处理函数 */
    err = igb_request_irq(adapter);
    if (err)
        goto err_req_irq;

    /* 启用NAPI */
    for (i = 0; i < adapter->num_q_vectors; i++)
        napi_enable(&(adapter->q_vector[i]->napi));
    ......
/*其中igb_setup_all_rx_resources分配了ringbuffer，建立内存和rx队列的关系（这里有一个重要的点是，内存，rx/tx队列，ringbuffer三者关系
）
ringbuffer：循环队列，有读写两个指针。
rx/tx队列：接收和发送队列，这个是基于ringbuffer来实现的。
内存：本质上rx/tx队列是一块预分配内存，这里就是为什么网卡驱动需要建立内存和rx/tx队列的映射关系；预分配内存是为了防止每次访问都需要经过os内核，浪费时间
*/
}
```
下面是ringbuffer的实现，以rx接收队列为例：
```c
//单个接收队列的产生
int igb_setup_rx_resources(struct igb_ring *rx_ring)
{
	struct device *dev = rx_ring->dev;
	int size;

	size = sizeof(struct igb_rx_buffer) * rx_ring->count;

	rx_ring->rx_buffer_info = vzalloc(size);//igb_rx_buffer是内核使用，由vzalloc申请
	if (!rx_ring->rx_buffer_info)
		goto err;

	/* Round up to nearest 4K */
	rx_ring->size = rx_ring->count * sizeof(union e1000_adv_rx_desc);
	rx_ring->size = ALIGN(rx_ring->size, 4096);

	rx_ring->desc = dma_alloc_coherent(dev, rx_ring->size,//e1000_adv_rx_desc是网卡硬件使用，由dma_alloc_coherent分配
					   &rx_ring->dma, GFP_KERNEL);
	if (!rx_ring->desc)
		goto err;

	rx_ring->next_to_alloc = 0;
	rx_ring->next_to_clean = 0;
	rx_ring->next_to_use = 0;

	return 0;
//所以一个ringbuffer其实由两个环形队列数组组成，一个给内核使用，一个给网卡硬件使用。细节见P22
err:
	vfree(rx_ring->rx_buffer_info);
	rx_ring->rx_buffer_info = NULL;
	dev_err(dev, "Unable to allocate memory for the Rx descriptor ring\n");
	return -ENOMEM;
}

/**
 *  igb_setup_all_rx_resources - wrapper to allocate Rx resources
 *				 (Descriptors) for all queues
 *  @adapter: board private structure
 *
 *  Return 0 on success, negative on failure
 **/
static int igb_setup_all_rx_resources(struct igb_adapter *adapter)
{
	struct pci_dev *pdev = adapter->pdev;
	int i, err = 0;

	for (i = 0; i < adapter->num_rx_queues; i++) {
		err = igb_setup_rx_resources(adapter->rx_ring[i]);//构建num_rx_queues个接收队列
		if (err) {
			dev_err(&pdev->dev,
				"Allocation for Rx Queue %u failed\n", i);
			for (i--; i >= 0; i--)
				igb_free_rx_resources(adapter->rx_ring[i]);
			break;
		}
	}

	return err;
}
```
下面就是中断函数的注册：
```c
static int igb_request_irq(struct igb_adapter *adapter){
    if (adapter->msix_entries) {
        err = igb_request_msix(adapter);
        if (!err)
            goto request_done;
        ......
    }

}

static int igb_request_msix(struct igb_adapter *adapter){

    ......
    for (i = 0; i < adapter->num_q_vectors; i++) {
        ...
        err = request_irq(adapter->msix_entries[vector].vector,
                  igb_msix_ring, 0, q_vector->name,
    }
/*其函数栈的调用是igb_open->igb_request_irq->igb_request_msix;
在igb_request_msix中可看到，其为每一个队列都注册了一个中断处理函数igb_request_msix；这样其实是天然支持收到的包对应不同的中断，从而被不同的CPU继续处理（需要修改irqbalance，或者/proc/irq/IRQ_NUMBER/smp_affinity的属性来修改中断和CPU的绑定行为）
*/
```
### 迎接数据到来
#### 硬中断处理
其过程如图：
![image-20231011212548238](linux网络.assets/image-20231011212548238.png)
首先网卡接收数据，然后网卡在分配给自己的ringbuffer中寻找可用的内存位置，DMA引擎再把数据DMA到内存的相应位置。在DMA操作完成，发给CPU一个硬中断，再调用前面的硬中断处理函数处理，再使用NAPI机制，发出软中断。
NAPI机制的poll函数不以中断形式来读取数据，以poll方法来轮询数据，减少中断的出发时间。甚至poll函数可以对ringbuffer的包进行检查，将相关的包进行合并。合并中断请求，将sk_buffer交付给上层处理。后面对ringbuffer的清理等。
==ringbuffer满的时候，新的数据包将被丢弃,ifcofig的overruns字段表示丢包数，如果有丢包，可以使用ethtool函数来增大队列长度==
硬中断处理函数igb_msix_ring：
```c
static irqreturn_t igb_msix_ring(int irq, void *data)
{
	struct igb_q_vector *q_vector = data;

	/* Write the ITR value calculated from the previous interrupt. */
	igb_write_itr(q_vector);
	napi_schedule(&q_vector->napi);
	return IRQ_HANDLED;
}
/*这里是硬中断处理函数，又进行到了napi_schedule机制的调度
```
