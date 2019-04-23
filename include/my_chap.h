/*
** EPITECH PROJECT, 2019
** my_chap.h
** File description:
** My_chap
*/

#ifndef MY_CHAP_H_
    #define MY_CHAP_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

#define UDP_HEADER 8
#define INET_HEADER 5
#define DATA_SIZE 1024
#define ARRAY_SIZE(array) sizeof(array) / sizeof(*array)

typedef struct sockaddr_in sockaddr_in_t;
typedef struct sockaddr sockaddr_t;
typedef struct udphdr udphdr_t;
typedef struct iphdr iphdr_t;
typedef int int_socket;

typedef struct s_pseudo_header
{
    u_int32_t  source_address;
    u_int32_t  dest_address;
    u_int16_t  udp_length;
    u_int8_t   placeholder;
    u_int8_t   protocol;
}              pseudo_header_t;

typedef struct s_infos
{
    sockaddr_in_t   src_addr;
    sockaddr_in_t   dst_addr;
    uint8_t         data[DATA_SIZE];
    int_socket      sock;
}              infos_t;

// Init
int udp_client(infos_t);

// Utils
uint16_t check_sum(uint8_t *, int);
int check_ip_port(char *, char *);

// Miscellianous
char **my_str_to_wordtab(char *, char);

#endif /* !MY_CHAP_H_ */
