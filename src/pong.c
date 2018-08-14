#include "familib/familib.h"
#include "titlescreen/title.h"

// define palette color aliases
#define COLOR_BLACK 0x0f
#define COLOR_WHITE 0x20

// clang-format off
#pragma bss-name(push, "ZEROPAGE")
u16 i;
u8 j;
#pragma bss-name(pop)
// clang-format on

const u8 TEXT[13] = "Hello, World!";

const u8 PALETTE[] = {COLOR_BLACK, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE,
                      COLOR_BLACK, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE};

extern u8 *music;

sprite_t meta[2];

/**
 * main() will be called at the end of the initialization code in reset.s.
 * Unlike C programs on a computer, it takes no arguments and returns no value.
 */
void main(void) {
    ppu_detect_system();
    ppu_load_bg_pal(PALETTE);
    ppu_load_spr_pal(PALETTE);
    ppu_load_name_table(NAME_TABLE_TOP_LEFT, (u8 *)NAME_TABLE);

    for (i = 0; i < sizeof(TEXT); ++i) {
        ppu_load_name_table_tile(TEXT[i], NAME_TABLE_TOP_LEFT, 14, 5 + i);
    }

    ppu_set_bg_at_left_pattern_table();
    ppu_set_spr_at_right_pattern_table();
    ppu_set_name_table_top_left();
    ppu_set_bg_left_col_on();
    ppu_set_spr_left_col_on();
    ppu_set_bg_on();
    ppu_set_spr_on();
    ppu_set_nmi_on();
    ppu_update();

    ft2_init(music);

    while (true) {
        ppu_wait_vblank();
        joy1_poll();
        joy2_poll();

        if ((joy1_pressing(PAD_LEFT) && !joy1_pressed(PAD_LEFT)) || (joy2_pressing(PAD_LEFT) && !joy2_pressed(PAD_LEFT))) {
            j -= 1;
            ft2_play_music(0);
        } else if (joy1_pressing(PAD_RIGHT) || joy2_pressing(PAD_RIGHT)) {
            j += 1;
            ft2_stop_music();
        }

        oam_buffer_spr(j, 15, 'J', SPRITE_FLIP | SPRITE_MIRROR, 0);
        meta[0].x = 0;
        meta[0].y = 25;

        if (ppu.system) {
            meta[0].spr = 'N';
        } else {
            meta[0].spr = 'P';
        }

        meta[0].opt = 0;
        meta[1].x   = 0x80;
        oam_buffer_metaspr(j, 25, 1, (u8 *)&meta);
        oam_hide_sprites_from(2);
    }
}

void nmi(void) {
    cpu_push_regs();
    oam_flush_sprites();
    ft2_update();
    cpu_pull_regs();
    cpu_rti();
}

void irq(void) { cpu_rti(); }