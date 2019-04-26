/*
** EPITECH PROJECT, 2019
** main.c
** File description:
** Main
*/

#include <getopt.h>
#include <openssl/sha.h>
#include "my_chap.h"

void init_struct(infos_t *infos_struct, connect_t *connection, char *data)
{
    infos_struct->src_addr.sin_family = AF_INET;
    infos_struct->src_addr.sin_port = htons(2048);
    inet_aton("127.0.0.1", &infos_struct->src_addr.sin_addr);
    infos_struct->dst_addr.sin_family = AF_INET;
    infos_struct->dst_addr.sin_port = htons(atoi(connection->port));
    if (strcmp(connection->target, "localhost") == 0)
        inet_aton("127.0.0.1", &infos_struct->dst_addr.sin_addr);
    else
        inet_aton(connection->target, &infos_struct->dst_addr.sin_addr);
    if (strcmp(data, "client hello") == 0)
        strcpy((char *)infos_struct->data, data);
    else {
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
            sprintf((char *)infos_struct->data + (i * 2), "%02x",
                    (unsigned char)data[i]);
    }
}

static int manage_argv(int oc, connect_t *connection)
{
    switch (oc) {
        case 't':
            connection->target = strdup(optarg);
            break;
        case 'p':
            connection->port = strdup(optarg);
            break;
        case 'P':
            connection->password = strdup(optarg);
            break;
        default:
            return (84);
    }
    return (0);
}

static int handle_argument(infos_t *infos_struct, int ac,
char **av, connect_t *connection)
{
    struct option   longopts[] = {
        {"target", required_argument, NULL, 't'},
        {"port", required_argument, NULL, 'p'},
        {"password", required_argument, NULL, 'P'}
    };
    int             oc = 0;

    for (size_t i = 0; i < ARRAY_SIZE(longopts); ++i) {
        oc = getopt_long(ac, av, "t:p:P:", longopts, NULL);
        if (manage_argv(oc, connection) == 84)
            return (84);
    }
    if (check_ip_port(connection->target, connection->port) == 84)
        return (84);
    init_struct(infos_struct, connection, "client hello");
    return (0);
}

static void destroyer(infos_t *infos_struct, connect_t *connection)
{
    close(infos_struct->sock);
    free(infos_struct);
    free(connection->target);
    free(connection->port);
    free(connection->password);
    free(connection);
}

int main(int ac, char **av)
{
    infos_t *infos_struct = malloc(sizeof(infos_t));
    connect_t *connection = malloc(sizeof(connect_t));
    const int i = 1;

    if (infos_struct == NULL)
        return (84);
    if (ac != 7 || handle_argument(infos_struct, ac, av, connection) == 84)
        return (84);
    if ((infos_struct->sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
        fprintf(stderr, "The socket failed\n");
        return (84);
    }
    setsockopt(infos_struct->sock, IPPROTO_IP, IP_HDRINCL, &i, sizeof(i));
    if (udp_client(infos_struct) == 84)
        return (84);
    if (degeu(infos_struct, connection) == 84)
        return (84);
    destroyer(infos_struct, connection);
    return (0);
}