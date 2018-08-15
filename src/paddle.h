#include "ball.h"
#include "familib/familib.h"

#define PADDLE_WIDTH_PX 4
#define PADDLE_HEIGHT_PX (2 * BALL_HEIGHT_PX)
#define PADDLE_SPEED 2
#define PADDLE_SPR_1_1_NUM 2
#define PADDLE_SPR_1_2_NUM (PADDLE_SPR_1_1_NUM + 1)
#define PADDLE_SPR_2_1_NUM (PADDLE_SPR_1_2_NUM + 1)
#define PADDLE_SPR_2_2_NUM (PADDLE_SPR_2_1_NUM + 1)
#define PADDLE_SPR_OFF 0
#define PADDLE_SPR1_ON 2
#define PADDLE_SPR2_ON 3

void paddle_draw(u8 n);

void paddle_hide(u8 n);

void paddle_move_up(u8 n);

void paddle_moved_down(u8 n);

void paddle_move_cpu(void);