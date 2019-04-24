/*
** EPITECH PROJECT, 2019
** arg_err_gest.c
** File description:
** arg_err_gest
*/

#include "my_chap.h"

static int check_ip_part(char *ip_part)
{
    for (size_t i = 0; ip_part[i] != '\0'; ++i) {
        if (!isdigit(ip_part[i]))
            return (84);
    }
    return (0);
}

static int check_ip(char *target)
{
    char    **tab = my_str_to_wordtab(target, '.');
    size_t  i = 0;

    for (i = 0; tab[i] != NULL; ++i) {
        if (strlen(tab[i]) == 0)
            return (84);
        if (check_ip_part(tab[i]) == 84)
            return (84);
        if (atoi(tab[i]) > 255)
            return (84);
    }
    if (i != 4)
        return (84);
    for (i = 0; tab[i] != NULL; ++i)
        free(tab[i]);
    free(tab);
    return (0);
}

int check_ip_port(char *target, char *port)
{
    for (size_t i = 0; port[i] != '\0'; ++i) {
        if (!isdigit(port[i])) {
            fprintf(stderr, "Bad port\n");
            return (84);
        }
    }
    if (atoi(port) < 1024 || atoi(port) > 65535) {
        fprintf(stderr, "Bad port\n");
        return (84);
    }
    if (check_ip(target) == 84) {
        if (strcmp(target, "localhost") == 0)
            return (0);
        fprintf(stderr, "No such hostname: '%s'\n", target);
        return (84);
    }
    return (0);
}