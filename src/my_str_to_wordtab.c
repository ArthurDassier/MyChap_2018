/*
** EPITECH PROJECT, 2018
** my_str_to_wordtab.c
** File description:
** My_str_to_wordtab
*/

#include "my_chap.h"

static int count_word(char *str, char sep)
{
    int word = 0;
    int letter = 0;
    int i = 0;

    while (str[i] != '\0') {
        if (str[i] != sep && letter == 0) {
            ++word;
            letter = 1;
        }
        if (str[i] == sep && letter == 1)
            letter = 0;
        ++i;
    }
    if (word == 0)
        return (strlen(str));
    return (word);
}

char **my_str_to_wordtab(char *str, char sep)
{
    char    **tab = malloc(sizeof(char *) * (count_word(str, sep) + 1));
    int     j = 0;
    int     col = 0;

    tab[col] = malloc(sizeof(char) * (strlen(str) + 1));
    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] == sep) {
            tab[col][j] = '\0';
            ++col;
            j = 0;
            ++i;
            tab[col] = malloc(sizeof(char) * (strlen(str) + 1));
        }
        tab[col][j] = str[i];
        ++j;
    }
    tab[col][j] = '\0';
    tab[++col] = NULL;
    return (tab);
}
