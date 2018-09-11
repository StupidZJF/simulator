#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>
#include <netdb.h>
#include <arpa/inet.h>


#include "simulator.h"

int num = 0;
struct Point *p[10];

double Distance(struct Point *p1, struct Point *p2)
{
    int x=(p1->x)-(p2->x);
    int y=(p1->y)-(p2->y);
    int z=(p1->z)-(p2->z);
    double temp=(double)(x*x+y*y+z*z);
    return sqrt(temp);
}

int read_point()
{
    char buf[1000];
    char temp0[20];
    char temp1[20];
    char temp2[20];
    char temp3[20];
    char temp4[20];
    char *p0;
    char *p1;
    char *p2;
    char *p3;
    char *p4;
    int recv_port;
    
	FILE *fp=fopen("../point_info","r");	//open 
	if(fp==NULL)
	{
		printf("can't open the point_info\n");
		return -1;
	}
	
	printf("NOW READING point_info !\n");
	
	while (fgets(buf, 1000, fp)){
        printf("节点 %d 坐标：\t", num+1);
        p0=strtok(buf,",");
        strcpy(temp0, p0);
		p1=strtok(NULL,",");
		strcpy(temp1, p1);
		p2=strtok(NULL,",");
		strcpy(temp2, p2);
		p3=strtok(NULL,",");
		strcpy(temp3, p3);
		p4=strtok(NULL,",");
		strcpy(temp4, p4);
		
		sscanf(temp0, "%d", &recv_port);
		p[num] = malloc(sizeof(struct Point));
		memset(&p[num]->node_addr, 0, sizeof(struct sockaddr_in));
    	p[num]->node_addr.sin_family      = AF_INET;
    	p[num]->node_addr.sin_port        = htons(recv_port);
    	p[num]->node_addr.sin_addr.s_addr = inet_addr ("127.0.0.1");
    	sscanf(temp1, "%d", &p[num]->send_port);
		sscanf(temp2, "%d", &p[num]->x);
		sscanf(temp3, "%d", &p[num]->y);
		sscanf(temp4, "%d", &p[num]->z);
		printf("node_addr: %s  ", inet_ntoa(p[num]->node_addr.sin_addr));
		printf("node_recv_port: %d  ", ntohs(p[num]->node_addr.sin_port));
		printf("node_send_port: %d  ", p[num]->send_port);
		printf("X: %d  ", p[num]->x);
		printf("Y: %d  ", p[num]->y);
		printf("Z: %d\n", p[num]->z);
		
        num++;
	}
	printf("共计%d个节点\n\n", num);
	return 0;
}

void Output(struct Point *p1, struct Point *p2)
{
    printf("(%d,%d,%d)<-->(%d,%d,%d) = %.4f\n\n",p1->x,p1->y,p1->z,p2->x,p2->y,p2->z,Distance(p1,p2));
}


int main() {
    int server_fd = -1;
    int set;
    int sendbytes;
    int recvbytes;
	int addrLen;
	int source_port;
	int n,i,j,count;
    struct sockaddr_in server_addr;
    struct sockaddr_in source_addr;
    unsigned char recv_buf[11][100];
    
    //创建套接字   
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {  
        perror("socket create failed!");  
        exit(1);  
    }
    
    set = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));
   
    //绑定端口地址
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(4000);
    server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
    
	//服务端套接字绑定自己的IP地址与端口号，客户端那边可以不写，内核会给它分配一个临时的端口
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {  
        perror("bind error!");  
        exit(1);  
    }
    
    read_point();
    count = num;
   
    while (1) {
        printf("Simulator waiting\n");
        addrLen = sizeof(struct sockaddr_in);  
		
		memset(recv_buf, 0, sizeof(recv_buf));   //初始化数据接收缓冲区	    
		//接收客户端发送过来的数据 
		if ((recvbytes = recvfrom(server_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&source_addr, &addrLen)) != -1) {
			
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
			
			source_port = ntohs(source_addr.sin_port);
			printf("检测接收结果 source_port:%d\n\n", source_port);
			
			
			
			for (n=0; n<num; n++) {
				if (p[n]->send_port == source_port) {
					i = n;
					for (j=0; j<count; j++) {
						if ((5-Distance(p[i],p[j])>=0)&&i!=j) {
    						printf("节点%d在发送范围之内：\n", j+1);
    						Output(p[i],p[j]);
    						if((sendbytes = sendto(server_fd, recv_buf, sizeof(recv_buf), 
    								0, (struct sockaddr *)&p[j]->node_addr, sizeof(struct sockaddr))) == -1){
								printf("sendto fail, errno=%d\n", errno);
								return -1;
							}
    					}
					}
				}
			}
		}else{
			printf("recvfrom fail\n");
			return -1;
		}
        
		printf("\n");
    }
}

