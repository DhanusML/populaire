#include "buffer.h"
#include <stdlib.h>
#include <assert.h>

Text_Buff *
buff_create()
{
    Text_Buff *t_buff = (Text_Buff *)malloc(sizeof(Text_Buff));
    t_buff->capacity = 40;
    t_buff->top = 0;
    char *buffer = (char *)malloc(t_buff->capacity * sizeof(char));
    assert(buffer!=0 && "malloc failed!\n");
    buffer[0] = 0;
    t_buff->buffer = buffer;
    return t_buff;
}


int
buff_add(Text_Buff *buff, char c)
{
    int ret_val = 0;
    if (buff->top > buff->capacity - 2)
    {
        char *temp_buff;
        buff->capacity *= 2;
        temp_buff = realloc(buff->buffer, buff->capacity);
        assert(temp_buff!=0 && "realloc failed\n");
        buff->buffer = temp_buff;
    }
    buff->buffer[buff->top++] = c;
    buff->buffer[buff->top] = 0;
    return ret_val;
}

int
buff_clear(Text_Buff *buff)
{ 
    int ret_val = 0;
    buff->top = 0;
    buff->buffer[0] = (char)0;
    return ret_val;
}

int
buff_pop(Text_Buff *buff)
{
    int ret_val = 0;
    if (buff->top) buff->top--;
    buff->buffer[buff->top] = 0;
    return ret_val;
}


int
buff_free(Text_Buff *buff)
{
    buff->top = 0;
    buff->capacity = 0;
    free(buff->buffer);
    free(buff);
    return 0;
}
