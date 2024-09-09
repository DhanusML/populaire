#include "utils.h"
#include <ctype.h>
#include <stdlib.h>

static int handle_space(Game_Status *, Stats *);

static int handle_backspace(Game_Status *);

static int handle_ctrl_backspace(Game_Status *);

static int handle_default(Game_Status *, Stats *, char);


int
update_game_state(Game_Status *status, Stats *measures)
{
    int ret_val = 0;
    int ch = status->input;
    switch(ch)
    {
        case ' ':
            ret_val |= handle_space(status, measures);
            break;
        case KEY_BACKSPACE:
        case 127:
            ret_val |= handle_backspace(status);
            break;
        case 8: // ctrl + backspace
        case 23://ctrl + w
            ret_val |= handle_ctrl_backspace(status);
            break;
        default:
            if(!isascii(ch) || iscntrl(ch)) break;
            ret_val |= handle_default(status, measures, ch);
            break;
    }
    return ret_val;
}


static int
handle_space(Game_Status *status, Stats *measures)
{
    int ret_val = 0;
    measures->raw_ch++;
    if ((status->err_char == 0) && (*(status->text_ptr) == ' '))
    {
        status->index++;
        status->start = status->index;
        status->text_ptr++;
        buff_clear(status->buffer);
    }
    else
    {
        status->err_char++;
        buff_add(status->buffer, ' ');
    }
    return ret_val;
}


static int
handle_backspace(Game_Status *status)
{
    int ret_val = 0;
    if (status->err_char)
    {
        status->err_char--;
    }
    else if (status->index > status->start)
    {
        status->index--;
        status->text_ptr--;
    }
    buff_pop(status->buffer);
    return ret_val;
}

static int
handle_ctrl_backspace(Game_Status *status)
{
    //TODO: change this to delete until the previous whitespace (incl.)
    int ret_val = 0;
    status->err_char = 0;
    status->text_ptr -= (status->index - status->start);
    status->index = status->start;
    buff_clear(status->buffer);
    return ret_val;
}


static int
handle_default(Game_Status *status, Stats *measures, char c)
{
    int ret_val = 0;
    measures->raw_ch++;
    if ((status->err_char == 0) && (*status->text_ptr == c))
    {
        status->index++;
        status->text_ptr++;
    }
    else
    {
        status->err_char++;
    }
    buff_add(status->buffer, c);
    return ret_val;
}
