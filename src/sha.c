/*
** EPITECH PROJECT, 2019
** sha.c
** File description:
** Sha
*/

#include <openssl/sha.h>
#include "my_chap.h"

int sha(infos_t *infos_struct, char *holder)
{
    header_t secret;
    SHA256_CTX sha256;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    socklen_t size = sizeof(infos_struct->dst_addr);

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, holder, strlen(holder));
    SHA256_Final(hash, &sha256);
    init_struct(infos_struct, "localhost", "5555", (char *)hash);
    if (udp_client(infos_struct) == 84)
        return (84);
    recvfrom(infos_struct->sock, &secret, sizeof(header_t), 0,
    (struct sockaddr *)&infos_struct->dst_addr, &size);
    memset(secret.data, 0, 4096);
    recvfrom(infos_struct->sock, &secret, sizeof(header_t), 0,
    (struct sockaddr *)&infos_struct->dst_addr, &size);
    if (strcmp(secret.data, "KO") == 0)
        printf("%s\n", secret.data);
    else
        printf("Secret: '%s'\n", secret.data);
    return (0);
}

int degeu(infos_t *infos_struct, char *password)
{
    header_t resp;
    char *holder = NULL;
    socklen_t size = sizeof(infos_struct->dst_addr);

    recvfrom(infos_struct->sock, &resp, sizeof(header_t), 0,
    (struct sockaddr *)&infos_struct->dst_addr, &size);
    memset(resp.data, 0, 4096);
    recvfrom(infos_struct->sock, &resp, sizeof(header_t), 0,
    (struct sockaddr *)&infos_struct->dst_addr, &size);
    holder = malloc(sizeof(char) * (strlen(password) + strlen(resp.data) + 1));
    holder[0] = '\0';
    strcat(holder, resp.data);
    strcat(holder, password);
    sha(infos_struct, holder);
    free(password);
    free(holder);
    free(infos_struct);
    return (0);
}