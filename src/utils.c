/*
** EPITECH PROJECT, 2019
** utils.c
** File description:
** Utils
*/

#include "my_chap.h"

uint16_t check_sum(uint8_t *data, int size)
{
    uint16_t    *p = (uint16_t *)data;
    uint16_t    carry;
    uint16_t    tmp;
    uint16_t    res;
    int         sum = 0;
    int         i = 0;

    while (i < size) {
        sum += *(p++);
        i += 2;
    }
    carry = sum >> 16;
    tmp = 0x0000ffff & sum;
    res = ~(tmp + carry);
    return (res);
}
