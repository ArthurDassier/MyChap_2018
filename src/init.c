/*
** EPITECH PROJECT, 2019
** init.c
** File description:
** init
*/

#include "my_chap.h"

static sockaddr_in_t init_sock_addr(int port)
{
    sockaddr_in_t     sin;

    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    return (sin);
}

int_socket init_socket(int port)
{
    sockaddr_in_t   sin;
    socklen_t       recsize = sizeof(sin);
    int_socket      sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        perror("Error on socket");
        return (84);
    }
    sin = init_sock_addr(port);
    if (bind(sock, (sockaddr_t*)&sin, recsize) == -1) {
        perror("Error on bind");
        return (84);
    }
    if (listen(sock, 5) == -1) {
        perror("Error on listen");
        return (84);
    }
    return (sock);
}