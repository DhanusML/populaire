#ifndef BUFFER_WPMC_H
#define BUFFER_WPMC_H
#include <stddef.h>

typedef struct Text_Buffer_t
{
    char *buffer;
    size_t top;
    size_t capacity;
} Text_Buff;

Text_Buff *buff_create();

int buff_add(Text_Buff *buff, char c);

int buff_clear(Text_Buff *buff);

int buff_pop(Text_Buff *buff);

int buff_free(Text_Buff *buff);

#endif
