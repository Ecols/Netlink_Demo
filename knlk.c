#include <linux/module.h>
#include <net/sock.h> 
#include <linux/netlink.h>
#include <linux/skbuff.h> 
#define NETLINK_USER 31

struct sock *nl_sk = NULL;

static void send_msg_to_user(int pid, char *umsg)
{
    struct nlmsghdr *nlh;
    struct sk_buff *skb_out;
    int msg_size;
    char kmsg[1000] = "Hello from kernel:";
    int res;

    printk(KERN_INFO "Entering: %s\n", __FUNCTION__);

        if (NULL != umsg)
        {
                snprintf(kmsg, sizeof(kmsg), "[%ld] Hello, kernel received:%s", jiffies, umsg);
        }

    msg_size = strlen(kmsg);

    skb_out = nlmsg_new(msg_size, 0);
    if (!skb_out) {
        printk(KERN_ERR "Failed to allocate new skb\n");
        return;
    }

    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
    NETLINK_CB(skb_out).dst_group = 0; /* not in mcast group */
    strncpy(nlmsg_data(nlh), kmsg, msg_size);

    res = nlmsg_unicast(nl_sk, skb_out, pid);
    if (res < 0)
        printk(KERN_INFO "Error while sending bak to user\n");

        return;
}

static void recv_msg_from_user(struct sk_buff *skb)
{
    struct nlmsghdr *nlh;

    printk(KERN_INFO "Entering: %s\n", __FUNCTION__);
        nlh = (struct nlmsghdr *)skb->data;
    printk(KERN_INFO "Netlink received user msg payload:%s\n", (char *)nlmsg_data(nlh));

        send_msg_to_user(nlh->nlmsg_pid, (char *)nlmsg_data(nlh));
        return;
}
int knlk_init(void)
{
    //nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, 0, hello_nl_recv_msg, NULL, THIS_MODULE);
    struct netlink_kernel_cfg cfg = {
        .input = recv_msg_from_user,
    };

    printk("Entering: %s\n", __FUNCTION__);
    nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
    if (!nl_sk) {
        printk(KERN_ALERT "Error creating socket.\n");
        return -10;
    }

    return 0;
}

void knlk_exit(void)
{
    printk(KERN_INFO "exiting hello module\n");
    netlink_kernel_release(nl_sk);
}
