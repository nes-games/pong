#include "ball.h"
#include "global.h"
#include "paddle.h"
#include "playfield.h"
#include "state.h"
#include "title.h"

game_t game;

void main(void) {
    ppu_detect_system();
    ppu_set_bg_at_left_pattern_table();
    ppu_set_spr_at_right_pattern_table();
    ppu_set_name_table_top_left();
    ppu_set_bg_left_col_on();
    ppu_set_spr_left_col_on();
    ppu_set_bg_off();
    ppu_set_spr_off();
    ppu_set_nmi_on();
    ppu_update();

    game.state.start   = true;
    game.state.current = STATE_TITLE;

    while (true) {
        switch (game.state.current) {
            case STATE_TITLE:
                title_update();
                break;
            case STATE_MATCH:
                if (game.state.start) {
                    // TODO playsfx
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
                    // TODO playsdx
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
            default:
                break;
        }
    }
}

void nmi(void) {
    cpu_push_regs();

    if (game.timer > 0) {
        game.timer--;
    }

    oam_flush_sprites();

    cpu_pull_regs();
    cpu_rti();
}

void irq(void) { cpu_rti(); }