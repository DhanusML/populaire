#ifndef WPMC_UTILS_H
#define WPMC_UTILS_H

#include <ncurses.h>
#include "buffer.h"

//TODO: streamline input
//TODO: allow custom imputs
//TODO: Add option to repeat at the end of the game
//TODO: multiple texts etc...


typedef struct Game_Status_t
{
    int status; // -1 end, 0 ready, 1 just started (first iter), 2 ongoing
    int input; //latest user input
    char *text; //beginning of text
    char *text_ptr; //start of current word
    Text_Buff *buffer; // buffer for storing user input
    size_t index; //index of the correct entry
    size_t start; //start of thw current word
    size_t err_char; //number of error characters
    size_t text_len; //total length of the text
} Game_Status;


typedef struct Statsistics_t
{
    double start_t;
    double end_t;
    double cps_raw;
    double wpm_raw;
    double cps;
    double wpm;
    double err;
    int raw_ch;
    int ch;
} Stats;


typedef struct Title_Window_t
{
    WINDOW *window;
} Title_Win;


typedef struct Text_Window_t
{
    WINDOW *window; // curses window
    char *content; // content of the window
    char *frame_start_ptr; // start of the frame
    size_t frame_size; // size of the fame
    size_t correct; // number of green chars
    size_t correct_lines; // number of lines to be highlighted green
    size_t correct_tail; // remainig green chars
    size_t incorrect; // number of red chars
    size_t incorrect_lines; // number of chars to be highlighted red
    size_t incorrect_head; // number of leading red chars
    size_t incorrect_tail; // number of trailing red chars
    size_t start_x; //position in window of text
    size_t start_y;
} Text_Win;


typedef struct User_Window_t
{
    WINDOW *window;
    char *buffer;
} User_Win;


typedef struct Prompt_Window_t
{
    WINDOW *window;
} Prompt_Win;


#ifdef DEBUG
typedef struct Debug_Window_t
{
    WINDOW *window;
    Game_Status *status;
    Stats *statistics;
    Text_Buff *user_buff;
} Debug_Win;
#endif


typedef struct Statistics_Window_t
{
    WINDOW *window;
    int hide;
    Stats *statistics;
} Stat_Win;


typedef struct Game_Window_t
{
    Title_Win *title;
    Text_Win *text;
    Prompt_Win *prompt;
    User_Win *user;
    Stat_Win *stats;
#ifdef DEBUG
    Debug_Win *debug;
#endif
} Game_Win;


void get_input_from_file(char **text, char *name);

void get_default_text(char **text);

int pre_start_state_config(Game_Status *status, Stats *measures, char *text);

int display_status_connect(Game_Win *game_window, Game_Status *status, Stats *measures);

int get_user_input(Game_Win *game_window, Game_Status *status);

int start_game(Game_Status *status, Stats *measures);

int end_of_game(Game_Status *status, Stats *measures);

int start_measurement(Stats *stats);

int end_measurement(Stats *stats);

int compute_stats(Stats *stats);

int update_game_display(Game_Win *game_window, Game_Status *status);

int update_game_state(Game_Status *status, Stats *measures);

int init_display_updates();

Game_Win *create_game_win();

Game_Status *create_game_status();

Stats *create_stats();

int delete_stats(Stats *stats);

int delete_game_status(Game_Status *status);

int delete_game_win(Game_Win *game_win);

void free_text(char *text);

#endif
