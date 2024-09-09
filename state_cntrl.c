#include "utils.h"
#include <string.h>


int
pre_start_state_config(Game_Status *status,
        Stats *measures, char *text)
{
    int ret_val = 0;

    status->status = 0;
    status->text = text;
    status->input = 0;
    status->text_ptr = text;
    status->buffer = buff_create();
    status->index = 0;
    status->start = 0;
    status->err_char = 0;
    status->text_len = strlen(text);

    measures->start_t = 0.0;
    measures->end_t = 0.0;
    measures->cps_raw = 0.0;
    measures->wpm_raw = 0.0;
    measures->raw_ch = 0;
    measures->ch = status->text_len;

    return ret_val;
}

int
start_game(Game_Status *status, Stats *measures)
{
    int ret_val = 0;
    status->status = 1;
    start_measurement(measures);
    return ret_val;
}


int
end_of_game(Game_Status *status, Stats *measures)
{
    //end of game condition
    if (status->index == status->text_len)
    {
        status->status = -1;
        end_measurement(measures);
        compute_stats(measures);
        return 1;
    }
    return 0;
}
