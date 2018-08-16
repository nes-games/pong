#include "familib/familib.h"

#ifndef __GLOBALS__
#define __GLOBALS__
typedef struct {
    u8_t current;
    bool start;
} state_t;

typedef struct {
    state_t state;
    bool    vs_cpu;
    s16_t   timer;
} game_t;

typedef struct {
    u8_t x;
    u8_t y;
} pos_t;

typedef struct {
    pos_t pos;
    u8_t  num;
    s8_t  direction;
} paddle_t;

typedef struct {
    s8_t x;
    s8_t y;
} speed_t;

typedef struct {
    pos_t   pos;
    speed_t speed;
    bool    out;
} ball_t;

typedef struct {
    paddle_t paddle[2];
    ball_t   ball;
    u8_t     score[2];
} match_t;
#endif

#define MAX_SCORE 15
#define COLOR_BLACK 0x0F
#define COLOR_WHITE 0x20
#define NT NAME_TABLE_TOP_LEFT

extern const u8_t NAM[30][32];
extern const u8_t ATR[64];
extern const u8_t PAL[16];

extern game_t  game;
extern match_t match;

#define timer_stopped() (game.timer == 0)