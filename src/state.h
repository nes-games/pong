#include "familib/familib.h"

#define STATE_TITLE 0
#define STATE_MATCH (STATE_TITLE + 1)
#define STATE_SERVE (STATE_MATCH + 1)
#define STATE_PLAY (STATE_SERVE + 1)
#define STATE_POINT (STATE_PLAY + 1)
#define STATE_SCORE (STATE_POINT + 1)
#define STATE_GAME_OVER (STATE_SCORE + 1)
#define STATE_PAUSE (STATE_GAME_OVER + 1)

#define STATE_FIRST STATE_TITLE
#define STATE_LAST STATE_PAUSE

void state_next(void);

void state_goto(u8 state);