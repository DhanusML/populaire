#ifndef CONFIG_WPMC_H
#define CONFIG_WPMC_H

// Text window config + outline window config
// makes up the complete text window config.
// dimenstions and pos of main text window
// the outline will appear in this position
#define OUTLINE_WIN_HEIGHT    10
#define OUTLINE_WIN_WIDTH     (COLS)
#define OUTLINE_WIN_X         0
#define OUTLINE_WIN_Y         3

// details of the text inside text window
// This is not a separate window, but just
// the contents inside the text window
#define TEXT_WIN_START_X      1 // adjust space from margin
#define TEXT_WIN_START_Y      1 // space from margin
#define TEXT_WIN_HEIGHT       (OUTLINE_WIN_HEIGHT - 2 * TEXT_WIN_START_Y)
#define TEXT_WIN_WIDTH        (OUTLINE_WIN_WIDTH - 2 * TEXT_WIN_START_X)
//#define TEXT_WIN_X            (OUTLINE_WIN_X + 1) // unused
//#define TEXT_WIN_Y            (OUTLINE_WIN_Y + 1) // unused

// text window config
#define TITLE_WIN_HEIGHT      2
#define TITLE_WIN_WIDTH       (OUTLINE_WIN_WIDTH)
#define TITLE_X               (TITLE_WIN_WIDTH - 9) / 2 // to center the title
#define TITLE_Y               0
#define TITLE_WIN_X           0
#define TITLE_WIN_Y           0
#define TITLE                 "Populaire" // field width is 9

// prompt window config
#define PROMPT_WIN_HEIGHT     1
#define PROMPT_WIN_WIDTH      2
#define PROMPT_WIN_X          0
#define PROMPT_WIN_Y          (OUTLINE_WIN_Y + OUTLINE_WIN_HEIGHT + 2)
#define PROMPT                "> " // filed width is 2

// user window config
#define USER_WIN_HEIGHT       2
#define USER_WIN_WIDTH        (COLS)
#define USER_WIN_X            (PROMPT_WIN_X + 2)
#define USER_WIN_Y            (PROMPT_WIN_Y)

// debug window config
#ifdef DEBUG
#define DEBUG_WIN_HEIGHT      3
#define DEBUG_WIN_WIDTH       (COLS)
#define DEBUG_WIN_X           0
#define DEBUG_WIN_Y           (USER_WIN_Y + 10)
#endif

// statistics window config
#define STATS_WIN_HEIGHT      10
#define STATS_WIN_WIDTH       (COLS/4)
#define STATS_WIN_X           0
#define STATS_WIN_Y           (USER_WIN_Y + 2)
#endif
