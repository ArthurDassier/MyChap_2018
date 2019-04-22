/*
** EPITECH PROJECT, 2019
** main.c
** File description:
** Main
*/

#include "my_chap.h"

infos_t init_struct()
{
    infos_t infos_struct;

    infos_struct.src_addr.sin_family = AF_INET;
    infos_struct.src_addr.sin_port = htons(2048);
    inet_aton("192.168.0.1", &infos_struct.src_addr.sin_addr);
    infos_struct.dst_addr.sin_family = AF_INET;
    infos_struct.dst_addr.sin_port = htons(4086);
    inet_aton("127.0.0.1", &infos_struct.dst_addr.sin_addr);
    strcpy((char *)infos_struct.data, "hello client");
    return (infos_struct);
}

int main(int ac, char **av)
{
    infos_t infos_struct = init_struct();

    if ((infos_struct.sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("socket");
        return (84);
    }
    return (udp_client(infos_struct));
}