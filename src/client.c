/*
** EPITECH PROJECT, 2019
** init.c
** File description:
** init
*/

#include "my_chap.h"

static uint16_t checksum(uint8_t *data, int size)
{
    uint16_t    *p = (uint16_t *)data;
    uint16_t    carry;
    uint16_t    tmp;
    uint16_t    res;
    int         sum = 0;
    int         i = 0;

    while (i < size) {
        sum += *(p++);
        i += 2;
    }
    carry = sum >> 16;
    tmp = 0x0000ffff & sum;
    res = ~(tmp + carry);
    return (res);
}

static int init_udp(infos_t infos_struct, uint8_t *udp_packet)
{
    uint8_t             pseudo_packet[DATA_SIZE];
    udphdr_t            *udph = (udphdr_t *)udp_packet;
    pseudo_header_t     *iph = (pseudo_header_t *)pseudo_packet;
    int                 data_size = strlen((char *)infos_struct.data);
    uint16_t            size = UDP_HEADER + data_size;

    memcpy(udp_packet + UDP_HEADER, infos_struct.data, data_size);
    udph->source = infos_struct.src_addr.sin_port;
    udph->dest = infos_struct.dst_addr.sin_port;
    udph->len = htons(size);
    udph->check = checksum(pseudo_packet, sizeof(pseudo_header_t) + size);
    memcpy(pseudo_packet + sizeof(pseudo_header_t), udph, size);
    iph->source_address = infos_struct.src_addr.sin_addr.s_addr;
    iph->dest_address = infos_struct.dst_addr.sin_addr.s_addr;
    iph->udp_length = udph->len;
    iph->placeholder = 0;
    iph->protocol = IPPROTO_UDP;
    return (size);
}

static int init_ip(infos_t infos_struct, uint8_t *ip_packet,
uint8_t *data, int data_size)
{
    iphdr_t *iph = (iphdr_t *)ip_packet;

    iph->saddr = infos_struct.src_addr.sin_addr.s_addr;
    iph->daddr = infos_struct.dst_addr.sin_addr.s_addr;
    iph->tot_len = htons(INET_HEADER * 4 + data_size);
    iph->protocol = IPPROTO_UDP;
    iph->ihl = INET_HEADER;
    iph->frag_off = 0;
    iph->version = 4;
    iph->check = 0;
    iph->ttl = 64;
    iph->tos = 0;
    iph->id = 0;
    memcpy(ip_packet + sizeof(iphdr_t), data, data_size);
    return (sizeof(iphdr_t) + data_size);
}

int udp_client(infos_t infos_struct)
{
    uint8_t     packet[ETH_DATA_LEN] = {0};
    int         ip = init_udp(infos_struct, packet + sizeof(iphdr_t));
    int         packet_size = init_ip(infos_struct, packet,
                                        packet + sizeof(iphdr_t), ip);

    if (sendto(infos_struct.sock, packet, packet_size, 0,
    (sockaddr_t *)&infos_struct.dst_addr, sizeof(infos_struct.dst_addr)) < 0) {
        perror("sendto failed");
        return (84);
    }
    return (0);
}