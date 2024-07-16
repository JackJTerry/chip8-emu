#include "chip8.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

const uint8_t chip8_font[] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

uint8_t keymap[16] = {
    SDLK_x, // 0
    SDLK_1, // 1
    SDLK_2, // 2
    SDLK_3, // 3
    SDLK_q, // 4
    SDLK_w, // 5
    SDLK_e, // 6
    SDLK_a, // 7
    SDLK_s, // 8
    SDLK_d, // 9
    SDLK_z, // A
    SDLK_c, // B
    SDLK_4, // C
    SDLK_r, // D
    SDLK_f, // E
    SDLK_v  // F
};

void chip8_init(CHIP8 *chip8) {
    srand((unsigned int)time(NULL));

    chip8->opcode = 0;
    chip8->pc = 0x200;
    chip8->I = 0;
    chip8->sp = 0;
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
    memset(chip8->memory, 0, 4096);
    memset(chip8->stack, 0, 16*sizeof(uint16_t));
    memset(chip8->V, 0, 16*sizeof(uint8_t));
    memset(chip8->graphics, 0, 2048);
    memset(chip8->keypad, 0, 16*sizeof(uint8_t));
    memcpy(chip8->memory, chip8_font, 80*sizeof(int8_t));
}

void chip8_handle_keypad(CHIP8 *chip8, SDL_Event *e){
    int val;
    if (e->type == SDL_KEYDOWN) val = 1;
    else if (e->type == SDL_KEYUP) val = 0;
    else val = -1;

    if (val != -1){
        for (int i = 0; i < 16; ++i) {
            if (e->key.keysym.sym == keymap[i]) {
                chip8->keypad[i] = val;
            }
        }
    }
}

void chip8_load_rom(CHIP8 *chip8, char *file_name) {
    (void) file_name;
    FILE *file = fopen(file_name,"rb");

    if (file == NULL) puts("Couldn't find File");

    fread(chip8->memory+0x200, 1, sizeof(chip8->memory)-0x200, file);
    fclose(file);
}

void chip8_emu_cycle(CHIP8 *chip8) {
    chip8->draw_flag = 0;
    chip8->sound_flag = 0;
    chip8_execute_instruction(chip8);
}

void chip8_execute_instruction(CHIP8 *chip8) {
    uint8_t x, y, nn, n;
    uint16_t nnn;
    chip8->opcode = MERGE_BYTES(chip8->memory[chip8->pc], chip8->memory[chip8->pc + 1]);
    x = X_MASK(chip8->opcode);
    y = Y_MASK(chip8->opcode);
    nnn = NNN_MASK(chip8->opcode);
    nn = NN_MASK(chip8->opcode);
    n = N_MASK(chip8->opcode);

    chip8->pc += 2;

    switch (chip8->opcode & 0xF000) {
        case 0x0000:
            switch (nn) {
                case 0x00E0:
                    memset(chip8->graphics, 0, 2048);
                    chip8->draw_flag = 1;
                    break;
                case 0x0EE:
                    chip8->pc = chip8->stack[chip8->sp];
                    chip8->sp--;
                    break;
            }
            break;
        case 0x1000:
            chip8->pc = nnn;
            break;
        case 0x2000:
            chip8->sp++;
            chip8->stack[chip8->sp] = chip8->pc;
            chip8->pc = nnn;
            break;
        case 0x3000:
            if (chip8->V[x] == nn) chip8->pc+=2;
            break;
        case 0x4000:
            if (chip8->V[x] != nn) chip8->pc+=2;
            break;
        case 0x5000:
            if (chip8->V[x] == chip8->V[y]) chip8->pc+=2;
            break;
        case 0x6000:
            chip8->V[x] = nn;
            break;
        case 0x7000:
            chip8->V[x] += nn;
            break;
        case 0x8000:
            switch(n){
                case 0x0000:
                    chip8->V[x] = chip8->V[y];
                    break;
                case 0x0001:
                    chip8->V[x] |= chip8->V[y];
                    break;
                case 0x0002:
                    chip8->V[x] &= chip8->V[y];
                    break;
                case 0x0003:
                    chip8->V[x] ^= chip8->V[y];
                    break;
                case 0x0004:
                    chip8->V[x] += chip8->V[y];
                    break;
                case 0x0005:
                    chip8->V[x] -= chip8->V[y];
                    break;
                case 0x0006:
                    chip8->V[x] >>= 1;
                    break;
                case 0x0007:
                    chip8->V[x] = (chip8->V[y] - chip8->V[x]);
                    break;
                case 0x000E:
                    chip8->V[x] <<= 1;
                    break;
            }
            break;
        case 0x9000:
            if (chip8->V[x] != chip8->V[y]) chip8->pc+=2;
            break;
        case 0xA000:
            chip8->I = nnn;
            break;
        case 0xB000:
            chip8->pc = (chip8->V[0] + nnn);
            break;
        case 0xC000:
            chip8->V[x] = ((rand() % 256) & nn);
            break;
        case 0xD000:
            uint8_t VX = chip8->V[x];
            uint8_t VY = chip8->V[y];
            uint8_t px;
            
            chip8->V[0xF] = 0;
            for (int yline = 0; yline < n; yline++) {
                px = chip8->memory[chip8->I + yline];
                for (int xline = 0; xline < 8; xline++) {
                    if((px & (0x80 >> xline)) !=0){
                        if(chip8->graphics[VX + xline +((VY + yline) * 64)] == 1) {
                            chip8->V[0xF] = 1;
                        }
                        chip8->graphics[VX + xline +((VY + yline) * 64)] ^= 1;
                    }
                }
            }
            chip8->draw_flag = 1;
            break;

        case 0xE000:
            switch(nn){
                case 0x09E:
                    if (chip8->keypad[chip8->V[x]]) chip8->pc += 2;
                    break;
                case 0x0A1:
                    if (!chip8->keypad[chip8->V[x]]) chip8->pc += 2;
                    break;
            }
            break;
        case 0xF000:
            switch(nn) {
                case 0x0007: 
                    chip8->V[x] = chip8->delay_timer;
                    break;
                case 0x000A:
                    int keypress = 0;
                    for (int i = 0; i < 16; i++){
                        if (chip8->keypad[i] != 0) {
                            chip8->V[x] = i;
                            keypress = 1;
                            break;
                        }
                    }
                    if (!keypress) chip8->pc -= 2;
                    break;
                case 0x0015:
                    chip8->delay_timer = chip8->V[x];
                    break;
                case 0x0018:
                    chip8->sound_timer = chip8->V[x];
                    break;
                case 0x001E:
                    chip8->I += chip8->V[x];
                    break;
                case 0x0029:
                    chip8->I = chip8->V[x] * 5;
                    break;
                case 0x0033:
                    chip8->memory[chip8->I] = chip8->V[x] / 100;
			        chip8->memory[chip8->I + 1] = (chip8->V[x] / 10) % 10;
			        chip8->memory[chip8->I + 2] = (chip8->V[x] % 100) % 10;
			        break;
                case 0x0055:
                    for (int i = 0; i <= x; i++) {
                        chip8->memory[chip8->I +i] = chip8->V[i];
                    }
                    break;
                case 0x0065:
                    for (int i = 0; i <= x; i++) {
                        chip8->V[i] = chip8->memory[chip8->I +i];
                    }
                    break;
            }
        break;
    }
}
