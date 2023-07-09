#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "message.h"
#include "tcp_client.h"
#include "udp_point.h"
#include "addr_mgr.h"
#include "utility.h"
#include "msg_def.h"

#define BUF_SIZE 64
#define DIM(a) sizeof(a)/sizeof(*a)

typedef struct 
{
    const char* cmd;
    void(*handler)(const char*);
} Handler;

static void ParseCommand(const char* s)
{
    const char* desc = s;
    const char* ip = s + 64;
    const char* usage = s + 80;
    int i = 0;
    int j = 0;
    int count = 0;
    char** parts = NULL;
    
    printf("%s:\n", desc);  //打印服务简介
    //所有数据，包括简介、ip地址、用法，
    //%s打印的是字符串，这三段中间是有\0的，定义赋值的时候就是以字符串形式,所以这是打印服务简介
    
    count = CharCount(usage, '\n'); //用法有多个字符串构成，统计\n个数
    parts = Malloc2d(char, count, BUF_SIZE);
    count = DivideByChar(usage, '\n', parts, count, BUF_SIZE);  //将用法的字符串放入二维数组
    
    for(i=0; i<count; i++)
    {
        printf("....%s\n", parts[i]);
        
        int c = CharCount(parts[i], ' ') + 1;       //通过空格统计命令个数
        char** cmd = Malloc2d(char, c, BUF_SIZE);   
        
        c = DivideByChar(parts[i], ' ', cmd, c, BUF_SIZE);//将命令的字符串放入二维数组
        
        for(j=1; j<c; j++)
        {
            printf("%s : %s\n", cmd[j], ip);
            AddrMgr_Add(cmd[j], ip);                //服务地址映射
        }
        
        Free2d(cmd);
    }
    
    Free2d(parts);
}

static void Query_Handler(const char* arg)
{ 
    UdpPoint* udp = UdpPoint_New(8888);
    
    if( udp )
    {
        Message msg = {TYPE_QUERY, 0, 0, 0, 0};
        char remote[16] = {0};
        int port = 0;
        int brd = 1;
        int i = 0;
        
        UdpPoint_SetOpt(udp, SOL_SOCKET, SO_BROADCAST, &brd, sizeof(brd));
        UdpPoint_SendMsg(udp, &msg, "255.255.255.255", 9999);   //向同一网络下的设备发送广播（本地广播）
        
        while( i < 3 )
        {
            if( UdpPoint_Available(udp) != -1 ) //获得报文长度，即收到数据
            {   
                Message* resp = UdpPoint_RecvMsg(udp, remote, &port);
                
                //回复数据
                ParseCommand(resp->payload);
                
                free(resp);
                
                i = 0;
            }
            else    //无数据
            {
                sleep(1);
                i++;
            }
        }
    }
    
    UdpPoint_Del(udp);  
}

static void Touch_Handler(const char* arg)
{
    //与tcp设备建立连接，建立tcp客户端
    TcpClient* client = TcpClient_New();
    
    if( client && *arg )    //touch后面的命令 arg不为空
    {
        char* ip = AddrMgr_Find(arg);
        
        if( ip )            //如果查找到，发起tcp链接
        {
            Message* msg = Message_New(TYPE_TOUCH, 0, 0, 0, arg, strlen(arg)+1);
            
            if( msg && TcpClient_Connect(client, ip, 8888) )
            {
                if( TcpClient_SendMsg(client, msg) )
                {
                    free(msg);
                    
                    msg = TcpClient_RecvMsg(client);
                    
                    if( msg && (msg->type == TYPE_RESPONSE) )
                    {
                        printf("Response: %s\n", msg->payload);
                    }
                }
                else
                {
                    AddrMgr_Remove(arg);    //如果发送数据出现问题，删除映射
                }               
                TcpClient_Close(client);
            }
            
            free(msg);
        }
        else
        {
            printf("Can NOT find service...\n");
        }
    }
    
    TcpClient_Del(client);  
}

static Handler g_handler[] = 
{
    {"query", Query_Handler},
    {"touch", Touch_Handler},
};

int main()
{
    char line[BUF_SIZE] = {0};
    char** arg = Malloc2d(char, 2, BUF_SIZE);
    
    printf("Client Demo @ D.T.Software\n");
    
    while( arg )
    {
        printf("Input >>> ");
        
        fgets(line, sizeof(line)-1, stdin); //命令行输入字符串
        
        line[strlen(line)-1] = 0;
        
        *arg[0] = 0;
        *arg[1] = 0;

        if( *line )
        {
            int i = 0;         
            int r = DivideByChar(line, ' ', arg, 2, BUF_SIZE);  //分割字符串存入二维数组，命令字符串、参数字符串
            
            for(i=0; (i<DIM(g_handler)) && (r>0); i++)
            {
                if( strcmp(g_handler[i].cmd, arg[0]) == 0 ) //判断命令字符串是query还是touch
                {
                    g_handler[i].handler(arg[1]);   //将参数字符串作为函数参数
                    break;
                }
            }
        }
    }
    
    Free2d(arg);
    
    return 0;
}

