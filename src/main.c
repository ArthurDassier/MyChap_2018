/*
** EPITECH PROJECT, 2019
** main.c
** File description:
** Main
*/

#include <getopt.h>
#include "my_chap.h"

static infos_t init_struct(char *target, char *port)
{
    infos_t infos_struct;

    infos_struct.src_addr.sin_family = AF_INET;
    infos_struct.src_addr.sin_port = htons(2048);
    inet_aton("192.168.0.1", &infos_struct.src_addr.sin_addr);
    infos_struct.dst_addr.sin_family = AF_INET;
    infos_struct.dst_addr.sin_port = htons(atoi(port));
    inet_aton(target, &infos_struct.dst_addr.sin_addr);
    strcpy((char *)infos_struct.data, "hello client");
    return (infos_struct);
}

static int manage_argv(char **target, char **port, int oc)
{
    switch (oc) {
        case 't':
            *(target) = strdup(optarg);
            break;
        case 'p':
            *(port) = strdup(optarg);
            break;
        case 'P':
            break;
        default:
            return (84);
    }
    return (0);
}

static int handle_argument(infos_t *infos_struct, int ac, char **av)
{
    struct option   longopts[] = {
        {"target", required_argument, NULL, 't'},
        {"port", required_argument, NULL, 'p'},
        {"password", required_argument, NULL, 'P'}
    };
    char            *target = NULL;
    char            *port = NULL;
    int             oc = 0;

    for (size_t i = 0; i < ARRAY_SIZE(longopts); ++i) {
        oc = getopt_long(ac, av, "t:p:P:", longopts, NULL);
        if (manage_argv(&target, &port, oc) == 84)
            return (84);
    }
    if (check_ip_port(target, port) == 84)
        return (84);
    *infos_struct = init_struct(target, port);
    return (0);
}

int main(int ac, char **av)
{
    infos_t infos_struct;
    const int i = 1;

    if (ac != 7 || handle_argument(&infos_struct, ac, av) == 84) {
        fprintf(stderr, "Usage: [-t target] [-p port] [-P password]\n");
        return (84);
    }
    if ((infos_struct.sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
        fprintf(stderr, "The socket failed\n");
        return (84);
    }
    setsockopt(infos_struct.sock, IPPROTO_IP, IP_HDRINCL, &i, sizeof(i));
    return (udp_client(infos_struct));
}