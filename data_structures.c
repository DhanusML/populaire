#include "utils.h"
#include <stdlib.h>
#include <assert.h>
#include "config.h"
#include <stdio.h>
#include "buffer.h"
#include <assert.h>

//VAR <- new(TYPE)
#define NEW(RET, VAR, TYPE)  TYPE * VAR = (TYPE *)malloc(sizeof(TYPE));\
    RET |= !(VAR)

//VAR <- newwin(h, w, y, x)
#define NEWWIN(RET, VAR, W_H, W_W, W_Y, W_X) \
    VAR = newwin(W_H, W_W, W_Y, W_X);\
    RET |= !(VAR)

#define FREE_WIN(VAR) delwin(VAR->window);\
    free(VAR)

Game_Win *
create_game_win()
{
    int status = 0;
    NEW(status, title, Title_Win);
    NEW(status, text, Text_Win);
    NEW(status, prompt, Prompt_Win);
    NEW(status, user, User_Win);
    NEW(status, stats, Stat_Win);
#ifdef DEBUG
    NEW(status, debug, Debug_Win);
#endif
    if (status) goto error_handle;

    NEWWIN(status, title->window, TITLE_WIN_HEIGHT, TITLE_WIN_WIDTH, TITLE_WIN_Y, TITLE_WIN_X);
    NEWWIN(status, text->window,OUTLINE_WIN_HEIGHT, OUTLINE_WIN_WIDTH, OUTLINE_WIN_Y, OUTLINE_WIN_X);
    NEWWIN(status, prompt->window, PROMPT_WIN_HEIGHT, PROMPT_WIN_WIDTH, PROMPT_WIN_Y, PROMPT_WIN_X);
    NEWWIN(status, user->window, USER_WIN_HEIGHT, USER_WIN_WIDTH, USER_WIN_Y, USER_WIN_X);
    NEWWIN(status, stats->window, STATS_WIN_HEIGHT, STATS_WIN_WIDTH, STATS_WIN_Y, STATS_WIN_X);
#ifdef DEBUG
    NEWWIN(status, debug->window, DEBUG_WIN_HEIGHT, DEBUG_WIN_WIDTH, DEBUG_WIN_Y, DEBUG_WIN_X);
#endif
    if (status) goto error_handle;

    text->content = NULL;
    text->frame_start_ptr = NULL;

    user->buffer = NULL;

    stats->statistics = NULL;

#ifdef DEBUG
    debug->status = NULL;
    debug->statistics = NULL;
    debug->user_buff = NULL;
#endif

    NEW(status, win_t, Game_Win);
    if (status) goto error_handle;

    win_t->title = title;
    win_t->text = text;
    win_t->prompt = prompt;
    win_t->user = user;
    win_t->stats = stats;
#ifdef DEBUG
    win_t->debug = debug;
#endif

    return win_t;

error_handle:
    if(title){
        if(title->window) {FREE_WIN(title);}
        else free(title);
    }
    if(text){
        if(text->window) {FREE_WIN(text);}
        else free(text);
    }
    if(prompt){
        if(prompt->window) {FREE_WIN(prompt);}
        else free(prompt);
    }
    if(user){
        if(user->window) {FREE_WIN(user);}
        else free(title);
    }
    if(stats){
        if(stats->window) {FREE_WIN(stats);}
        else free(stats);
    }
#ifdef DEBUG
    if(debug){
        if(debug->window){FREE_WIN(debug);}
        else free(debug);
    }
#endif
    if(win_t) free(win_t);
    return NULL;
}


Game_Status *
create_game_status()
{
    int ret = 0;
    NEW(ret, status, Game_Status);
    if (ret) return NULL;
    status->text = NULL;
    status->text_ptr = NULL;
    status->buffer = NULL;
    return status;
}


Stats *
create_stats()
{
    int ret = 0;
    NEW(ret, stats, Stats);
    return ret ? NULL : stats;
}

int
delete_game_win(Game_Win *game_win)
{
    FREE_WIN(game_win->title);
    FREE_WIN(game_win->text);
    FREE_WIN(game_win->prompt);
    FREE_WIN(game_win->user);
    FREE_WIN(game_win->stats);
#ifdef DEBUG
    FREE_WIN(game_win->debug);
#endif
    free(game_win);
    return 0;
}

int
delete_game_status(Game_Status *status)
{
    status->status = -1;
    status->input = 0;
    //free(status->text); // should this be free'd here or in main
    status->text_ptr = 0;
    buff_free(status->buffer);
    status->index = 0;
    status->start = 0;
    status->err_char = 0;
    status->text_len = 0;
    free(status);
    return 0;
}

int
delete_stats(Stats *stats)
{
    free(stats);
    return 0;
}
