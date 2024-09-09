#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void
get_input_from_file(char **text, char *name)
{
    size_t buff_size = 2;
    size_t curr_buff_size = 0;
    //size_t curr_read_length;
    char *ptr = malloc(buff_size * sizeof(char) + 1);
    assert(ptr != 0 && "malloc failed!\n");
    char *temp_ptr = ptr;
    char *temp_temp_ptr;
    FILE *input_file = fopen(name, "r");
    assert(input_file!=0 && "can't open file!\n");
    while (fread(temp_ptr, 1, 1, input_file) == 1)
    {
        if (*temp_ptr == '\n')
        {
            *temp_ptr = ' ';
        }
        if ((curr_buff_size == 0) && (*temp_ptr == ' '))
        {
            continue;
        }
        if ((curr_buff_size > 1) &&
            (*temp_ptr == ' ') &&
            (*(temp_ptr - 1) == ' '))
        {
            continue;
        }
        curr_buff_size += 1;
        if (curr_buff_size == buff_size-1)
        {
            buff_size *= 2;
            temp_temp_ptr = realloc(ptr, buff_size*sizeof(char));
            assert(temp_temp_ptr !=0 && "realloc failed!");
            ptr = temp_temp_ptr;
            temp_ptr = ptr;
            temp_ptr += curr_buff_size;
        }
        else temp_ptr++;
    }
    fclose(input_file);
    if (curr_buff_size == 0)
    {
        free(ptr);
        *text = NULL;
        return;
    }
    temp_ptr--; // final new line
    while(*temp_ptr == ' ') temp_ptr--; //remove trailing white spaces
                                        //at most one
    *(++temp_ptr) = (char )0;
    *text = ptr;
}


void
get_default_text(char **text)
{
    const char def_text[] = "The quick brown fox jumps over the lazy dog.";
    char *ptr = (char *)malloc((strlen(def_text)+1) * sizeof(char));
    assert(ptr != 0 && "malloc failed!\n");
    strcpy(ptr, def_text);
    *text = ptr;
}


void
free_text(char *text)
{
    if (text) free(text);
}
