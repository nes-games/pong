#include "ball.h"
#include "global.h"
#include "paddle.h"
#include "playfield.h"
#include "sfx.h"
#include "state.h"
#include "title.h"

game_t game;
s8     score_to_draw;

void main(void) {
    ppu_detect_system();
    ft2_stop_music();
    ft2_init_sfx(sfx);

    score_to_draw      = -1;
    game.state.start   = true;
    game.state.current = STATE_TITLE;

    ppu_set_bg_at_left_pattern_table();
    ppu_set_spr_at_right_pattern_table();
    ppu_set_name_table_top_left();
    ppu_set_bg_left_col_on();
    ppu_set_spr_left_col_on();
    ppu_set_bg_off();
    ppu_set_spr_off();
    ppu_set_nmi_on();
    ppu_update();

    while (true) {
        switch (game.state.current) {
            case STATE_TITLE:
                title_update();
                break;
            case STATE_MATCH:
                if (game.state.start) {
                    ft2_play_sfx(SFX_HIT, 0);
                    game.timer       = 30;
                    game.state.start = false;
                } else if (timer_stopped()) {
                    playfield_reset();
                    playfield_draw();
                    state_next();
                }
                break;
            case STATE_SERVE:
                if (game.state.start) {
                    playfield_restart();
                    ft2_play_sfx(SFX_ALARM, 0);
                    game.timer       = 30;
                    game.state.start = false;
                } else if (timer_stopped()) {
                    match.ball.speed.x = -3;  // todo random
                    match.ball.speed.y = 0;
                    state_next();
                }
                break;
            case STATE_PLAY:
                if (timer_stopped()) {
                    game.timer = 1;
                    ball_move();

                    if (match.ball.out) {
                        state_next();
                    } else {
                        joy1_poll();

                        if (joy1_pressing(PAD_UP)) {
                            paddle_move_up(0);
                        } else if (joy1_pressing(PAD_DOWN)) {
                            paddle_moved_down(0);
                        } else {
                            match.paddle[0].direction = 0;
                        }

                        if (game.vs_cpu) {
                            paddle_move_cpu();
                        } else {
                            joy2_poll();

                            if (joy2_pressing(PAD_UP)) {
                                paddle_move_up(1);
                            } else if (joy2_pressing(PAD_DOWN)) {
                                paddle_moved_down(1);
                            } else {
                                match.paddle[1].direction = 0;
                            }
                        }

                        if ((joy1_pressing(BUTTON_START) && !joy1_pressed(BUTTON_START)) || (!game.vs_cpu && joy2_pressing(BUTTON_START) && !joy2_pressed(BUTTON_START))) {
                            state_goto(STATE_PAUSE);
                        }
                    }
                }
                break;
            case STATE_POINT:
                if (game.state.start) {
                    ft2_play_sfx(SFX_ALARM, 0);
                    game.timer       = 30;
                    game.state.start = false;
                } else if (timer_stopped()) {
                    state_next();
                }
                break;
            case STATE_SCORE:
                if (game.state.start) {
                    if (match.ball.pos.x > PF_RIGHT) {
                        match.score[0]++;
                        score_to_draw = 0;
                    } else {
                        match.score[1]++;
                        score_to_draw = 1;
                    }

                    ft2_play_sfx(SFX_SCORE, 0);
                    game.timer       = 15;
                    game.state.start = false;
                } else if (timer_stopped()) {
                    if (match.score[0] >= MAX_SCORE || match.score[1] >= MAX_SCORE) {
                        state_next();
                    } else {
                        state_goto(STATE_SERVE);
                    }
                }
                break;
            case STATE_GAME_OVER:
                if (game.state.start) {
                    ft2_play_sfx(SFX_ALARM, 0);
                    playfield_draw_winner();
                    game.timer       = 240;
                    game.state.start = false;
                } else if (game.timer == 180) {
                    ft2_play_sfx(SFX_ALARM, 0);
                } else if (game.timer == 120) {
                    ft2_play_sfx(SFX_ALARM, 0);
                    playfield_draw_game_over();
                } else if (timer_stopped()) {
                    state_goto(STATE_TITLE);
                }
                break;
            case STATE_PAUSE:
                if (game.state.start) {
                    game.state.start = false;
                    playfield_draw_pause();
                }

                joy1_poll();

                if (!game.vs_cpu) {
                    joy2_poll();
                }

                if ((joy1_pressing(BUTTON_START) && !joy1_pressed(BUTTON_START)) || (!game.vs_cpu && joy2_pressing(BUTTON_START) && !joy2_pressed(BUTTON_START))) {
                    playfield_hide_pause();
                    state_goto(STATE_PLAY);
                }
                break;
            default:
                break;
        }
    }
}

void nmi(void) {
    cpu_push_regs();

    if (score_to_draw >= 0) {
        playfield_draw_score(score_to_draw);
        score_to_draw = -1;
        PPUADDR       = 0;
        PPUADDR       = 0;
    }

    if (game.timer > 0) {
        game.timer--;
    }

    ft2_update();

    oam_flush_sprites();

    cpu_pull_regs();
    cpu_rti();
}

void irq(void) {
    cpu_rti();
}