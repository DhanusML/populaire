#include "utils.h"
#include "config.h"

static int update_title_win(Title_Win *title_win);
static int draw_title_win(Title_Win *title_win);
static int update_text_win(Text_Win *text_win, Game_Status *status);
static int draw_text_win(Text_Win *text_win);
static int update_prompt_win(Prompt_Win *promt_win);
static int draw_prompt_win(Prompt_Win *promt_win);
static int update_user_win(User_Win *user_win, Game_Status *status);
static int draw_user_win(User_Win *user_win);
static int update_stat_win(Stat_Win *stat_win, Game_Status *status);
static int draw_stat_win(Stat_Win *stat_win);
static int update_cursor_pos(Text_Win *text_win);
#ifdef DEBUG
static int update_debug_win(Debug_Win *debug_win);
static int draw_debug_win(Debug_Win *debug_win);
#endif

inline static size_t min(size_t a, size_t b)
{
    return a < b ? a : b;
}

int init_display_updates()
{
    int ret_val = 0;
    //common initializations
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(2);
    return ret_val;
}


int
get_user_input(Game_Win *game_window, Game_Status *status)
{
    int ret_val = 0;
    status->input = wgetch(game_window->user->window);
    return ret_val;
}


int
display_status_connect(Game_Win *game_window, Game_Status *status, Stats *measures)
{
    int ret_val = 0;
    game_window->text->content = status->text;
    game_window->text->frame_start_ptr = status->text;
    game_window->stats->statistics = measures;
    game_window->user->buffer = status->buffer->buffer;
#ifdef DEBUG
    game_window->debug->status = status;
    game_window->debug->statistics = measures;
    game_window->debug->user_buff = status->buffer;
#endif
    return ret_val;
}


int
update_game_display(Game_Win *game_window, Game_Status *status)
{
    int ret_val = 0;
    ret_val |= update_title_win(game_window->title);
    ret_val |= update_text_win(game_window->text, status);
    ret_val |= update_prompt_win(game_window->prompt);
    ret_val |= update_user_win(game_window->user, status);
    ret_val |= update_stat_win(game_window->stats, status);
#ifdef DEBUG
    ret_val |= update_debug_win(game_window->debug);
#endif
    ret_val |= update_cursor_pos(game_window->text);
    return ret_val;
}


static int
update_title_win(Title_Win *title_win)
{
    int ret_val = 0;
    ret_val |= draw_title_win(title_win);
    return ret_val;
}


static int
draw_title_win(Title_Win *title_win)
{
    int ret_val = 0;
    werase(title_win->window);
    wattr_on(title_win->window, A_BOLD|A_ITALIC|A_UNDERLINE, NULL);
    mvwprintw(title_win->window, TITLE_Y, TITLE_X, "%s", TITLE);
    wattr_off(title_win->window, A_BOLD|A_ITALIC|A_UNDERLINE, NULL);
    wrefresh(title_win->window);
    return ret_val;
}


static int
update_text_win(Text_Win *text_win, Game_Status *status)
{
    int ret_val = 0;

    text_win->start_x = TEXT_WIN_START_X;
    text_win->start_y = TEXT_WIN_START_Y;
    text_win->frame_size = (TEXT_WIN_HEIGHT) * (TEXT_WIN_WIDTH);
    int index_in_frame = (status->text + status->index - text_win->frame_start_ptr)\
                         % text_win->frame_size;
    if ((index_in_frame / TEXT_WIN_WIDTH) == (TEXT_WIN_HEIGHT - 2))
    {
        text_win->frame_start_ptr += (TEXT_WIN_WIDTH * (TEXT_WIN_HEIGHT - 3));
    }
    text_win->correct = status->text + status->index - text_win->frame_start_ptr;
    size_t correct_lines = text_win->correct / TEXT_WIN_WIDTH;
    size_t correct_tail = text_win->correct % TEXT_WIN_WIDTH;

    text_win->incorrect = min(status->err_char,
            text_win->frame_size - text_win->correct);
    text_win->incorrect = min(text_win->incorrect,
            status->text_len - status->index);

    size_t incorrect_lines = 0;
    size_t incorrect_tail = 0;
    size_t incorrect_head = 0;
    if (text_win->correct_tail)
        incorrect_head = min(TEXT_WIN_WIDTH - correct_tail,
                text_win->incorrect);
    if (incorrect_head < text_win->incorrect)
    {
        incorrect_lines = (text_win->incorrect - incorrect_head) / \
                          TEXT_WIN_WIDTH;
        incorrect_tail = (text_win->incorrect - incorrect_head) % \
                         TEXT_WIN_WIDTH;
    }
    text_win->correct_lines = correct_lines;
    text_win->correct_tail = correct_tail;
    text_win->incorrect_head = incorrect_head;
    text_win->incorrect_lines = incorrect_lines;
    text_win->incorrect_tail = incorrect_tail;
    ret_val |= draw_text_win(text_win);
    return ret_val;
}


static int
draw_text_win(Text_Win *text_win)
{
   int ret_val = 0; 
   start_color();
   init_pair(1, COLOR_RED, COLOR_WHITE);
   init_pair(2, COLOR_GREEN, COLOR_BLACK);
   werase(text_win->window);
   box(text_win->window, 0, 0);
   size_t i = 0;
   char curr_char;
   size_t start_x = text_win->start_x, start_y = text_win->start_y;
   wmove(text_win->window, start_x, start_y);
   int x = (int)start_x, y = (int)start_y;
   while ((curr_char = text_win->frame_start_ptr[i++]) && 
           i<text_win->frame_size)
   {
       mvwaddch(text_win->window, y, x++, curr_char);
       if (x==(TEXT_WIN_WIDTH+(int)start_x)) // +1 because start_x starts at 1
       {
           x=start_x;
           y++;
       }
   }

   //green
   for (size_t row_num=0; row_num<text_win->correct_lines; row_num++)
   {
       mvwchgat(text_win->window, row_num+start_y, start_x,
               TEXT_WIN_WIDTH, A_COLOR, 2, NULL);
   }
   mvwchgat(text_win->window, text_win->correct_lines+start_y, start_x,
           text_win->correct_tail, A_COLOR, 2, NULL);

   //red
   mvwchgat(text_win->window, text_win->correct_lines+start_y,
           text_win->correct_tail+start_x,
           text_win->incorrect_head, A_REVERSE, 1, NULL);
   for (size_t row_num=0; row_num<text_win->incorrect_lines; row_num++)
   {
       mvwchgat(text_win->window,
           text_win->correct_lines+!!(text_win->incorrect_head)+row_num+start_y,
               start_x, TEXT_WIN_WIDTH,
               A_REVERSE, 1, NULL);
   }
   mvwchgat(text_win->window,
           text_win->correct_lines + \
               !!(text_win->incorrect_head)+text_win->incorrect_lines+start_y,
           start_x, text_win->incorrect_tail, A_REVERSE, 1, NULL);
   wrefresh(text_win->window);
   return ret_val;
}


static int
update_cursor_pos(Text_Win *text_win)
{
    size_t col, row;
    if (text_win->incorrect)
    {
        if (text_win->incorrect_tail) col = text_win->incorrect_tail;
        else col = text_win->correct_tail + text_win->incorrect_head;
    }
    else col = text_win->correct_tail;
    row = text_win->correct_lines + \
          !!(text_win->correct_tail + text_win->incorrect_head) +\
          text_win->incorrect_lines + \
          !!(text_win->incorrect_tail) - 1;
    if (!(text_win->correct_tail + text_win->incorrect_head +\
                text_win->incorrect_tail))
        row++;
    col += text_win->start_x;
    row += text_win->start_y;
    wmove(text_win->window, row, col);
    wrefresh(text_win->window);
    return 0;
}


static int
update_prompt_win(Prompt_Win *prompt_win)
{
    int ret_val = 0;
    ret_val |= draw_prompt_win(prompt_win);
    return ret_val;
}

static int
draw_prompt_win(Prompt_Win *prompt_win)
{
    int ret_val = 0;
    werase(prompt_win->window);
    mvwprintw(prompt_win->window, 0, 0, PROMPT);
    wrefresh(prompt_win->window);
    return ret_val;
}


static int
update_user_win(User_Win *user_win, Game_Status *status)
{
    int ret_val = 0;
    user_win->buffer = status->buffer->buffer;
    ret_val |= draw_user_win(user_win);
    return ret_val;
}


static int
draw_user_win(User_Win *user_win)
{
    int ret_val = 0;
    werase(user_win->window);
    wmove(user_win->window, 0, 0);
    if (!*(user_win->buffer)) mvwprintw(user_win->window, 0, 0, " ");
    else mvwprintw(user_win->window, 0, 0, "%s", user_win->buffer);
    wrefresh(user_win->window);
    return ret_val;
}


static int
update_stat_win(Stat_Win *stat_win, Game_Status *status)
{
    int ret_val = 0;
    if (status->status != -1)
    {
        stat_win->hide = 1;
        return ret_val;
    }
    // status = -1 means game over
    stat_win->hide = 0;
    ret_val |= draw_stat_win(stat_win);
    return ret_val;
}

static int
draw_stat_win(Stat_Win *stat_win)
{
    int ret_val = 0;
    if (stat_win->hide) return ret_val;
    Stats *statistics = stat_win->statistics;
    wattron(stat_win->window, A_BOLD);
    wprintw(stat_win->window, "wpm: %.2lf\n", statistics->wpm);
    wprintw(stat_win->window, "cps: %.2lf\n", statistics->cps);
    wprintw(stat_win->window, "wpm (raw): %.2lf\n", statistics->wpm_raw);
    wprintw(stat_win->window, "cps (raw): %.2lf\n", statistics->cps_raw);
    wprintw(stat_win->window,
            "accuracy: %.2lf%%\n", (1 - statistics->err) * 100);
    wattroff(stat_win->window, A_BOLD);
    wrefresh(stat_win->window);
    return ret_val;
}


#ifdef DEBUG
static int
update_debug_win(Debug_Win *debug_win)
{
    int ret_val = 0;
    ret_val |= draw_debug_win(debug_win);
    return ret_val;
}


static int
draw_debug_win(Debug_Win *debug_win)
{
    int ret_val = 0;
    Game_Status *status = debug_win->status;
    Stats *statistics = debug_win->statistics;
    Text_Buff *buff = debug_win->user_buff;
    werase(debug_win->window);
    wprintw(debug_win->window,
    "index: %zu, start: %zu, input: %c (%d), correct: %c (%d), err_char: %zu",
            status->index, status->start,
            (char)status->input, status->input, *(status->text_ptr),
            (int)(*(status->text_ptr)), status->err_char);

    char curr_char = buff->top ? buff->buffer[buff->top-1]: '_';
    wprintw(debug_win->window, "\ntop: %zu (%c), capacity: %zu",
            buff->top, curr_char, buff->capacity);
    wprintw(debug_win->window, "\nch = %d, raw_ch = %d, elapsed = %lf",
            statistics->ch,
            statistics->raw_ch, statistics->end_t - statistics->start_t);
    wrefresh(debug_win->window);
    return ret_val;
}
#endif
