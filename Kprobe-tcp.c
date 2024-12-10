#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/net.h>
#include <linux/inet.h>
#include <linux/socket.h>
#include <net/tcp.h>

static struct kprobe kp;  // kprobe 钩子结构体

// 定义一个 kprobe 的处理函数
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    struct sock *sk;
    struct sockaddr_in *addr;
    int pid = current->pid;  // 获取当前进程的 PID

    // 获取网络连接相关信息
    sk = (struct sock *)regs->di;  // 获取连接的 sock 结构
    addr = (struct sockaddr_in *)regs->si;  // 获取目标地址结构

    pr_info("PID: %d, TCP connect to %pI4:%d\n", pid, &addr->sin_addr, ntohs(addr->sin_port));

    return 0;  // 返回 0，表示继续执行原函数
}

// 模块初始化函数
static int __init kprobe_init(void)
{
    kp.symbol_name = "tcp_v4_connect";  // 设置要钩住的函数名
    kp.pre_handler = handler_pre;  // 设置 pre_handler 函数

    // 注册 kprobe 钩子
    if (register_kprobe(&kp) < 0) {
        pr_err("Registering kprobe failed\n");
        return -1;
    }

    pr_info("kprobe registered on tcp_v4_connect\n");
    return 0;
}

// 模块清理函数
static void __exit kprobe_exit(void)
{
    unregister_kprobe(&kp);  // 注销 kprobe 钩子
    pr_info("kprobe unregistered\n");
}

module_init(kprobe_init);
module_exit(kprobe_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple kprobe example for network connection.");

