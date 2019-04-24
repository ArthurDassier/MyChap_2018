/*
** EPITECH PROJECT, 2019
** init.c
** File description:
** init
*/

#include "my_chap.h"

static int init_pseudo(infos_t *infos_struct, udphdr_t *udph, uint16_t size)
{
    uint8_t             pseudo_packet[DATA_SIZE];
    pseudo_header_t     *iph = (pseudo_header_t *)pseudo_packet;

    iph->source_address = infos_struct->src_addr.sin_addr.s_addr;
    iph->dest_address = infos_struct->dst_addr.sin_addr.s_addr;
    iph->udp_length = udph->len;
    iph->placeholder = 0;
    iph->protocol = IPPROTO_UDP;
    return (size);
}

static int init_udp(infos_t *infos_struct, uint8_t *udp_packet)
{
    udphdr_t    *udph = (udphdr_t *)udp_packet;
    int         data_size = strlen((char *)infos_struct->data);
    uint16_t    size = UDP_HEADER + data_size;

    udph->source = infos_struct->src_addr.sin_port;
    udph->dest = infos_struct->dst_addr.sin_port;
    udph->len = htons(size);
    udph->check = 0;
    memcpy(udp_packet + UDP_HEADER, infos_struct->data, data_size);
    return (init_pseudo(infos_struct, udph, size));
}

static int init_ip(infos_t *infos_struct, uint8_t *ip_packet, int data_size)
{
    iphdr_t *iph = (iphdr_t *)ip_packet;

    iph->saddr = infos_struct->src_addr.sin_addr.s_addr;
    iph->daddr = infos_struct->dst_addr.sin_addr.s_addr;
    iph->tot_len = htons(INET_HEADER * 4 + data_size);
    iph->protocol = IPPROTO_UDP;
    iph->ihl = INET_HEADER;
    iph->frag_off = 0;
    iph->version = 4;
    iph->check = 0;
    iph->ttl = 64;
    iph->tos = 0;
    iph->id = 0;
    return (sizeof(iphdr_t) + data_size);
}

int udp_client(infos_t *infos_s)
{
    uint8_t     packet[ETH_DATA_LEN] = {0};
    int         ip = init_udp(infos_s, packet + sizeof(iphdr_t));
    int         packet_size = init_ip(infos_s, packet, ip);

    if (sendto(infos_s->sock, packet, packet_size, 0,
        (sockaddr_t *)&infos_s->dst_addr, sizeof(infos_s->dst_addr)) < 0) {
        fprintf(stderr, "Sendto failed\n");
        return (84);
    }
    free(infos_s);
    return (0);
}