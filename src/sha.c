/*
** EPITECH PROJECT, 2019
** sha.c
** File description:
** Sha
*/

#include <openssl/sha.h>
#include "my_chap.h"

int sha(infos_t *infos_struct, connect_t *connection, char *holder)
{
    header_t sct;
    SHA256_CTX sha256;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    socklen_t size = sizeof(infos_struct->dst_addr);

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, holder, strlen(holder));
    SHA256_Final(hash, &sha256);
    init_struct(infos_struct, connection, (char *)hash);
    if (udp_client(infos_struct) == 84)
        return (84);
    recvfrom(infos_struct->sock, &sct, sizeof(header_t), 0,
    (struct sockaddr *)&infos_struct->dst_addr, &size);
    memset(sct.data, 0, 4096);
    recvfrom(infos_struct->sock, &sct, sizeof(header_t), 0,
    (struct sockaddr *)&infos_struct->dst_addr, &size);
    printf(strcmp(sct.data, "KO") == 0 ? "%s\n" : "Secret: '%s'\n", sct.data);
    return (0);
}

int degeu(infos_t *infos_struct, connect_t *connection)
{
    header_t resp;
    char *holder = NULL;
    socklen_t size = sizeof(infos_struct->dst_addr);

    recvfrom(infos_struct->sock, &resp, sizeof(header_t), 0,
    (struct sockaddr *)&infos_struct->dst_addr, &size);
    memset(resp.data, 0, 4096);
    recvfrom(infos_struct->sock, &resp, sizeof(header_t), 0,
    (struct sockaddr *)&infos_struct->dst_addr, &size);
    holder = malloc(sizeof(char) * (strlen(connection->password)
    + strlen(resp.data) + 1));
    holder[0] = '\0';
    strcat(holder, resp.data);
    strcat(holder, connection->password);
    sha(infos_struct, connection, holder);
    free(holder);
    return (0);
}