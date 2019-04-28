/*
** EPITECH PROJECT, 2019
** init.c
** File description:
** Init
*/

#include <openssl/sha.h>
#include "my_chap.h"

void init_struct(infos_t *infos_struct, connect_t *connection, char *data)
{
    infos_struct->src_addr.sin_family = AF_INET;
    infos_struct->src_addr.sin_port = htons(2048);
    inet_aton("0.0.0.0", &infos_struct->src_addr.sin_addr);
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