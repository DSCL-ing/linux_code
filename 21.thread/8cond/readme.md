
初识 条件变量 condition variable

条件变量队列中依次挂起依次唤醒


使用条件变量pthread_cond_wait,依次挂起所有线程


1.在主线程中使用cond_signal逐个唤醒进程
观察发现,线程以一定顺序进行运行. --- 

2.使用cond_broadcast一次全部唤醒
线程也是按一定顺序排队运行

