#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "simulator.h"

int main() {
    printf("Client Waiting\n");
    
    int recv_fd = -1;
    int send_fd = -1;
    int set;
    int recvbytes;
    int sendBytes;
	int addrLen;
	struct sockaddr_in serv_addr;
    struct sockaddr_in recv_addr;
    struct sockaddr_in send_addr;
    unsigned char recv_buf[11][100];
    
    //创建套接字   
    if ((recv_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {  
        perror("socket create failed!");  
        exit(1);  
    }
    
    if ((send_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {  
        perror("socket create failed!");  
        exit(1);  
    }
    
    set = 1;
    setsockopt(recv_fd, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));
   
    //绑定端口地址
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));  //清空套接字
    serv_addr.sin_family     = AF_INET;  
    serv_addr.sin_port       = htons(4000);
    serv_addr.sin_addr.s_addr= inet_addr("127.0.0.1");  //inet_addr()将一个点分十进制的IP转换成一个长整数型数
    
    memset(&recv_addr, 0, sizeof(struct sockaddr_in));    
    recv_addr.sin_family      = AF_INET;  
    recv_addr.sin_port        = htons(4002);
    recv_addr.sin_addr.s_addr = htonl (INADDR_ANY);  //绑定到本地默认ip上进行监听
    
    memset(&send_addr, 0, sizeof(struct sockaddr_in));    
    send_addr.sin_family      = AF_INET;  
    send_addr.sin_port        = htons(4020);
    send_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	
	//服务端套接字绑定自己的IP地址与端口号，客户端那边可以不写，内核会给它分配一个临时的端口
    if (bind(recv_fd, (struct sockaddr*)&recv_addr, sizeof(struct sockaddr)) == -1) {  
        perror("bind error!");  
        exit(1);  
    }
    
    if (bind(send_fd, (struct sockaddr*)&send_addr, sizeof(struct sockaddr)) == -1) {  
        perror("bind error!");  
        exit(1);  
    }
   
    while (1) {
        //接受client发送的请示信息
        printf("Server waiting\n");
        addrLen = sizeof(struct sockaddr_in);  
		
		memset(recv_buf, 0, sizeof(recv_buf));   //初始化数据接收缓冲区	    
		//接收客户端发送过来的数据 
		if ((recvbytes = recvfrom(recv_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&recv_addr, &addrLen)) != -1) {
			//pico_stack_recv(recv_buf);
			struct pico_eth_hdr *mac_h = (struct pico_eth_hdr *) recv_buf[0];
            printf("检测接收结果 MAC_Loc_ADDR:%08x\n", mac_h->saddr.addr);
			printf("检测接收结果 MAC_Dst_ADDR:%08x\n", mac_h->daddr.addr);
			printf("检测接收结果 MAC_PROTOCAL:%04x\n\n", mac_h->proto);
						
			struct pico_ipv4_hdr *route_h = (struct pico_ipv4_hdr *) recv_buf[1];
			printf("检测接收结果 ROUTE_HDR_LEN:%u\n", route_h->len);
			printf("检测接收结果 ROUTE_TTL:%d\n", route_h->ttl);
			printf("检测接收结果 ROUTE_PROTOCAL:%d\n", route_h->proto);
			printf("检测接收结果 ROUTE_Loc_ADDR:%08x\n", route_h->src.addr);
			printf("检测接收结果 ROUTE_Dst_ADDR:%08x\n", route_h->dst.addr);
			printf("检测接收结果 ROUTE_Nxt_ADDR:%08x\n\n", route_h->nxt.addr);

			struct pico_udp_hdr *rdt_h = (struct pico_udp_hdr *) recv_buf[2];
			printf("检测接收结果 RDT_Loc_PORT:%04x\n", rdt_h->trans.sport);
			printf("检测接收结果 RDT_Dst_PORT:%04x\n", rdt_h->trans.dport);
			printf("检测接收结果 RDT_Len:%u\n", rdt_h->len);
			printf("检测接收结果 RDT_CRC:%d\n\n", rdt_h->crc);
			
			printf("检测接收结果 payload:%s\n\n", recv_buf[9]);
			
			if((sendBytes = sendto(send_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr))) == -1){
				printf("sendto fail, errno=%d\n", errno);
				return -1;
			}
			
		}else{
			printf("recvfrom fail\n");
			return -1;
		}
        
		printf("\n");
    }
	return 0;
}

