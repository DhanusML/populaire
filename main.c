#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void print_help(char *exe_name)
{
    fprintf(stderr, "%s [-f FILE -r NUM -h]\n", exe_name);
}

int
main(int argc, char *argv[])
{
    char *text = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "f:r:h"))!= -1)
        switch (opt)
        {
            case 'f':
                get_input_from_file(&text, optarg);
                break;
            case 'r':
                get_random_text(&text, (size_t)atoi(optarg));
                printf("random:\n");
                printf("%s\n", text);
                break;
            case 'h':
                print_help(argv[0]);
                return 0;
            case '?':
                return 1;
        }
    if (! text) get_default_text(&text);
    if (! text)
    {
        fprintf(stderr, "error: could not load input text\n");
        return 1;
    }
    init_display_updates();
    Game_Win *game_window = create_game_win();
    if (!game_window)
    {
        fprintf(stderr, "error: game window creation failed\n");
        free_text(text);
        return 1;
    }
    Game_Status *status = create_game_status();
    if (!status)
    {
        fprintf(stderr, "error: game status creation failed\n");
        delete_game_win(game_window);
        free_text(text);
        return 1;
    }
    Stats *measures = create_stats();
    if (!measures)
    {
        fprintf(stderr, "error: game statistics creation failed\n");
        delete_game_win(game_window);
        delete_game_status(status);
        free_text(text);
        return 1;
    }

    pre_start_state_config(status, measures, text);
    display_status_connect(game_window, status, measures);
    update_game_display(game_window, status);

    int first_iter = 1;

    while (1)
    {
        get_user_input(game_window, status);
        if (first_iter)
        {
            start_game(status, measures);
            first_iter = 0;
        }
        update_game_state(status, measures);
        update_game_display(game_window, status);
        if (end_of_game(status, measures))
        {
            update_game_display(game_window, status);
            getchar();
            break;
        }
    }
    delete_game_win(game_window);
    delete_stats(measures);
    delete_game_status(status);
    free_text(text);
    endwin();
    return 0;
}
