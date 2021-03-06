#include <stdio.h>   
#include <stdlib.h>   
#include <errno.h>   
#include <string.h>   
#include <sys/types.h>   
#include <netinet/in.h>   
#include <sys/socket.h>   
#include <sys/wait.h> 
#include <unistd.h>  

struct pico_addr
{
    uint32_t addr;
};

struct pico_trans
{
    uint16_t sport;
    uint16_t dport;
};

struct pico_eth_hdr {
    struct pico_addr saddr;
    struct pico_addr daddr;
    uint16_t proto;
};

struct pico_ipv4_hdr {
    uint16_t len;
    uint8_t ttl;
    uint8_t proto;
    struct pico_addr src;
    struct pico_addr dst;
    struct pico_addr nxt;
};

struct pico_udp_hdr {
    struct pico_trans trans;
    uint16_t len;
    uint16_t crc;
};

struct pico_remote_endpoint {
    struct pico_addr remote_addr;
    uint16_t remote_port;
};

struct Point {
    struct sockaddr_in node_addr;
    int send_port;
    int x;
    int y;
    int z;
};
