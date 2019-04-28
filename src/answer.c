/*
** EPITECH PROJECT, 2019
** sha.c
** File description:
** Sha
*/

#include <openssl/sha.h>
#include "my_chap.h"

static header_t check_port(infos_t *infos_struct, connect_t *connection)
{
    socklen_t   size = sizeof(infos_struct->dst_addr);
    header_t    response;
    int         i = 0;

    recvfrom(infos_struct->sock, &response, sizeof(header_t), 0,
    (struct sockaddr *)&infos_struct->dst_addr, &size);
    while (response.udp.uh_sport != htons(atoi(connection->port)) && i < 20) {
        recvfrom(infos_struct->sock, &response, sizeof(header_t), 0,
        (struct sockaddr *)&infos_struct->dst_addr, &size);
        ++i;
    }
    return (response);
}

static int secret(infos_t *infos_struct, connect_t *connection, char *holder)
{
    socklen_t       size = sizeof(infos_struct->dst_addr);
    unsigned char   hash[SHA256_DIGEST_LENGTH];
    header_t        sct;
    SHA256_CTX      sha256;

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, holder, strlen(holder));
    SHA256_Final(hash, &sha256);
    init_struct(infos_struct, connection, (char *)hash);
    if (udp_client(infos_struct) == 84)
        return (84);
    recvfrom(infos_struct->sock, &sct, sizeof(header_t), 0,
    (struct sockaddr *)&infos_struct->dst_addr, &size);
    sct = check_port(infos_struct, connection);
    printf(strcmp(sct.data, "KO") == 0 ? "KO\n" : "Secret: '%s'\n", sct.data);
    return (0);
}

int handle_server_answer(infos_t *infos_struct, connect_t *connection)
{
    socklen_t   size = sizeof(infos_struct->dst_addr);
    char        *holder = NULL;
    header_t    response;

    recvfrom(infos_struct->sock, &response, sizeof(header_t), 0,
    (struct sockaddr *)&infos_struct->dst_addr, &size);
    response = check_port(infos_struct, connection);
    holder = malloc(sizeof(char) * (strlen(connection->password)
    + strlen(response.data) + 1));
    holder[0] = '\0';
    strcat(holder, response.data);
    strcat(holder, connection->password);
    secret(infos_struct, connection, holder);
    free(holder);
    return (0);
}