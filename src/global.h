#include "familib/familib.h"

#ifndef __GLOBALS__
#define __GLOBALS__
typedef struct {
    u8   current;
    bool start;
} state_t;

typedef struct {
    state_t state;
    bool    vs_cpu;
    s16     timer;
} game_t;

typedef struct {
    u8 x;
    u8 y;
} pos_t;

typedef struct {
    pos_t pos;
    u8    num;
    s8    direction;
} paddle_t;

typedef struct {
    pos_t pos;
    pos_t speed;
    bool  out;
} ball_t;

typedef struct {
    paddle_t paddle[2];
    ball_t   ball;
    u8       score[2];
} match_t;
#endif

#define MAX_SCORE 15
#define COLOR_BLACK 0x0F
#define COLOR_WHITE 0x20
#define NT NAME_TABLE_TOP_LEFT

extern const u8 NAM[30][32];
extern const u8 ATR[64];
extern const u8 PAL[16];

extern game_t  game;
extern match_t match;

#define timer_stopped() (game.timer == 0)