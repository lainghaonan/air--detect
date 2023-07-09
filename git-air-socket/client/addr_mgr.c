//服务命令与设备地址的映射

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "list.h"           //linux内核链表
#include "addr_mgr.h"

#define CMD_SIZE  48
#define IP_SIZE   16

//命令与地址的映射
typedef struct
{
    struct list_head head;  //创建链表
    char cmd[CMD_SIZE];
    char ip[IP_SIZE];
} SvrAddr;

static LIST_HEAD(g_svrList);    //定义全局链表，初始化链表

int AddrMgr_Add(const char* cmd, const char* addr)
{
    int ret = 0;
    
    if( cmd && addr )
    {
        char* ip = AddrMgr_Find(cmd);   //先查找链表中有没有相同cmd
        
        if( ip )
        {
            ret = !!strcpy(ip, addr);   //有相同的cmd，那直接更新ip
        }
        else
        {
            SvrAddr* sa = malloc(sizeof(SvrAddr));  //创建新节点
            
            if( ret = !!sa )
            {
                strncpy(sa->cmd, cmd, CMD_SIZE);
                strncpy(sa->ip, addr, IP_SIZE);
                
                sa->cmd[CMD_SIZE-1] = 0;
                sa->cmd[IP_SIZE-1] = 0;
                
                list_add((struct list_head*)sa, &g_svrList);//将节点加入链表
            }
        }
    }
    
    return ret;
}

//通过cmd找ip
//命令与地址的映射关系存储再链表中
char* AddrMgr_Find(const char* cmd)
{
    char* ret = NULL;
    
    if( cmd )
    {
        struct list_head* pos = NULL;
        
        list_for_each(pos, &g_svrList)  //遍历链表，pos为位置，list_head类型，for循环
        {
            SvrAddr* c = (SvrAddr*)pos;
            //为什么这种可以强制转换
            //两个结构体的起始地址相同，类型转换可以理解为首地址不变，我们把其读取方式改变
            //上面的两个结构体，他们的首地址的一样，其第一个偏移也是一样（废话都是链表结构体）所以可以进行强制类型转换
            
            if( strcmp(cmd, c->cmd) == 0 )  //cmd比较链表中的每个节点的字符串
            {
                ret = c->ip;
                break;
            }
        }
    }
    
    return ret;
}

void AddrMgr_Remove(const char* cmd)
{
    char* ip = AddrMgr_Find(cmd);   //通过查找，可以判断cmd是否合法

    if( ip )
    {
        SvrAddr* sa = container_of(ip, SvrAddr, ip);    //通过一个结构变量中一个成员的地址找到这个结构体变量的首地址
                                    //参数1：指针（已知的那个成员的地址），类型（结构体类型）、成员（参数1对应结构体成员）
        list_del((struct list_head*)sa);
        
        free(sa);
    }
}

void AddrMgr_Clear()
{
    while( !list_empty(&g_svrList) )
    {
        struct list_head* sa = g_svrList.next;
        
        list_del(sa);
        
        free(sa);
    }
}