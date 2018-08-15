#include "familib/familib.h"

#define PF_TOP 16
#define PF_BOTTOM (SCREEN_HEIGHT_PX - 16)
#define PF_LEFT 8
#define PF_RIGHT (SCREEN_WIDTH_PX - 8)
#define PF_TILE_OFF 0
#define PF_TILE_ON 1
#define PF_SCORE_SPRNUM 6
#define PF_TEXT_SPRNUM 32

void playfield_reset(void);

void playfield_restart(void);

void playfield_draw(void);

void playfield_draw_score1(void);

void playfield_draw_score2(void);

void playfield_draw_winner(void);

void playfield_draw_cpu_wins(void);

void playfield_draw_game_over(void);

void playfield_draw_pause(void);

void playfield_hide_pause(void);