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

unsigned char send_buf[11][100];

int main(int argc, char* argv[])
{

    int node1_fd;
    int sendBytes;
    struct sockaddr_in serv_addr;
    struct sockaddr_in node1_addr;
    
    struct pico_eth_hdr mac_hdr;
    struct pico_ipv4_hdr route_hdr;
    struct pico_udp_hdr rdt_hdr;
    struct pico_remote_endpoint remote_endpoint;
    
    uint32_t buffer_len = 54;
    uint32_t len = 54;
    uint16_t net_len = 16;
	uint16_t transport_len = 28;
	uint8_t flags = 0;
	
	unsigned char payload[100] = "test";
    uint16_t payload_len = 20;
  
    printf("手动赋值 mac_hdr!\n");
	mac_hdr.saddr.addr = 0x0101a8c0;
	mac_hdr.daddr.addr = 0x0201a8c0;
	mac_hdr.proto = 0x0008;
	printf("手动赋值 mac_hdr 结束!\n");
	
	printf("手动赋值 route_hdr!\n");
	route_hdr.len = 11264;
	route_hdr.ttl = 64;
	route_hdr.proto = 17;
	route_hdr.src.addr = 0x0101a8c0;	
	route_hdr.dst.addr = 0x0201a8c0;
	route_hdr.nxt.addr = 0x0201a8c0;
	printf("手动赋值 route_hdr 结束!\n");
	
	printf("手动赋值 rdt_hdr!\n");
	rdt_hdr.trans.sport = 0x0b1a;
	rdt_hdr.trans.dport = 0x0b1a;
	rdt_hdr.len = 7168;
	rdt_hdr.crc = 0;
	printf("手动赋值 rdt_hdr 结束!\n");
	
	printf("手动赋值 remote_endpoint!\n");
	remote_endpoint.remote_addr.addr = 0x0201a8c0;
	remote_endpoint.remote_port = 0x0b1a;
	printf("手动赋值 remote_endpoint 结束!\n\n");
	
    printf("赋值结果 MAC_Loc_ADDR:%08x\n", mac_hdr.saddr.addr);
	printf("赋值结果 MAC_Dst_ADDR:%08x\n", mac_hdr.daddr.addr);
	printf("赋值结果 MAC_PROTOCAL:%04x\n\n", mac_hdr.proto);
	
	printf("赋值结果 ROUTE_HDR_LEN:%u\n", route_hdr.len);
	printf("赋值结果 ROUTE_TTL:%d\n", route_hdr.ttl);
	printf("赋值结果 ROUTE_PROTOCAL:%d\n", route_hdr.proto);
	printf("赋值结果 ROUTE_Loc_ADDR:%08x\n", route_hdr.src.addr);
	printf("赋值结果 ROUTE_Dst_ADDR:%08x\n", route_hdr.dst.addr);
	printf("赋值结果 ROUTE_Nxt_ADDR:%08x\n\n", route_hdr.nxt.addr);
	
	printf("赋值结果 RDT_Loc_PORT:%04x\n", rdt_hdr.trans.sport);
	printf("赋值结果 RDT_Dst_PORT:%04x\n", rdt_hdr.trans.dport);
	printf("赋值结果 RDT_Len:%u\n", rdt_hdr.len);
	printf("赋值结果 RDT_CRC:%d\n\n", rdt_hdr.crc);
	
    printf("赋值结果 info_remote_addr:%08x\n", remote_endpoint.remote_addr.addr);
	printf("赋值结果 info_remote_port:%04x\n\n", remote_endpoint.remote_port);
	
	memset(send_buf[0],0,100);
    memcpy(send_buf[0], &mac_hdr,sizeof(struct pico_eth_hdr)); //(3)结构体转换成字符串

    memset(send_buf[1],0,100);
    memcpy(send_buf[1], &route_hdr,sizeof(struct pico_ipv4_hdr)); 

    memset(send_buf[2],0,100);
    memcpy(send_buf[2], &rdt_hdr,sizeof(struct pico_udp_hdr));

    memset(send_buf[3],0,100);
    memcpy(send_buf[3], &remote_endpoint,sizeof(struct pico_remote_endpoint));
    
    sprintf(send_buf[4],"%d",buffer_len);
    sprintf(send_buf[5],"%d",len);
    sprintf(send_buf[6],"%d",net_len);
    sprintf(send_buf[7],"%d",transport_len);
    sprintf(send_buf[8],"%d",flags);
    memcpy(send_buf[9],payload,sizeof(payload)); //(3)结构体转换成字符串
    sprintf(send_buf[10],"%d",payload_len);
    
    struct pico_eth_hdr *mac_h = (struct pico_eth_hdr *) send_buf[0];
	printf("检测赋值结果 MAC_Loc_ADDR:%08x\n", mac_h->saddr.addr);
	printf("检测赋值结果 MAC_Dst_ADDR:%08x\n", mac_h->daddr.addr);
	printf("检测赋值结果 MAC_PROTOCAL:%04x\n\n", mac_h->proto);
	
	struct pico_ipv4_hdr *route_h = (struct pico_ipv4_hdr *) send_buf[1];
	printf("检测赋值结果 ROUTE_HDR_LEN:%u\n", route_h->len);
	printf("检测赋值结果 ROUTE_TTL:%d\n", route_h->ttl);
	printf("检测赋值结果 ROUTE_PROTOCAL:%d\n", route_h->proto);
	printf("检测赋值结果 ROUTE_Loc_ADDR:%08x\n", route_h->src.addr);
	printf("检测赋值结果 ROUTE_Dst_ADDR:%08x\n", route_h->dst.addr);
	printf("检测赋值结果 ROUTE_Nxt_ADDR:%08x\n\n", route_h->nxt.addr);

	//struct pico_udp_hdr *rdt_h = (struct pico_udp_hdr *) send_udp;
	struct pico_udp_hdr *rdt_h = (struct pico_udp_hdr *) send_buf[2];
	printf("检测赋值结果 RDT_Loc_PORT:%04x\n", rdt_h->trans.sport);
	printf("检测赋值结果 RDT_Dst_PORT:%04x\n", rdt_h->trans.dport);
	printf("检测赋值结果 RDT_Len:%u\n", rdt_h->len);
	printf("检测赋值结果 RDT_CRC:%d\n\n", rdt_h->crc);
	
	struct pico_remote_endpoint *r_ep = (struct pico_remote_endpoint *) send_buf[3];
    printf("检测赋值结果 info_remote_addr:%08x\n", r_ep->remote_addr.addr);
	printf("检测赋值结果 info_remote_port:%04x\n\n", r_ep->remote_port);
	
	printf("检测赋值结果 send_buf[4]:%s\n", send_buf[4]);
	printf("检测赋值结果 send_buf[5]:%s\n", send_buf[5]);
	printf("检测赋值结果 send_buf[6]:%s\n", send_buf[6]);
	printf("检测赋值结果 send_buf[7]:%s\n", send_buf[7]);
	printf("检测赋值结果 send_buf[8]:%s\n", send_buf[8]);
	printf("检测赋值结果 send_buf[9]:%s\n", send_buf[9]);
	printf("检测赋值结果 send_buf[10]:%s\n", send_buf[10]);

    if ((node1_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {  //创建套接字
        perror("socket error!");  
        exit(1);  
    }  

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));  //清空套接字
    serv_addr.sin_family     = AF_INET;  
    serv_addr.sin_port       = htons(4000);
    serv_addr.sin_addr.s_addr= inet_addr("127.0.0.1");  //inet_addr()将一个点分十进制的IP转换成一个长整数型数
    
    memset(&node1_addr, 0, sizeof(struct sockaddr_in));
    node1_addr.sin_family     = AF_INET;
    node1_addr.sin_port       = htons(4010);
    node1_addr.sin_addr.s_addr= inet_addr("127.0.0.1");
   
    if (bind(node1_fd, (struct sockaddr*)&node1_addr, sizeof(struct sockaddr)) == -1) {  
        perror("bind error!");  
        exit(1);  
    }

	if((sendBytes = sendto(node1_fd, send_buf, sizeof(send_buf), 0, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr))) == -1){
		printf("sendto fail, errno=%d\n", errno);
		return -1;
	}
    close(node1_fd);
	return 0;
}  

