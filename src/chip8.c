#include "chip8.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void chip8_load_rom(CHIP8 *chip8, char *file_name) {
    (void) file_name;
    FILE *file = fopen("/home/undefined/C/chip8-emu/IBM_Logo.ch8","rb");

    if (file == NULL) puts("fuck");

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
            }
            break;
        case 0x1000:
            chip8->pc = nnn;
            break;
        case 0x2000:
        case 0x3000:
        case 0x4000:
        case 0x5000:
        case 0x6000:
            chip8->V[x] = nn;
            break;
        case 0x7000:
            chip8->V[x] += nn;
            break;
        case 0x8000:
        case 0x9000:
        case 0xA000:
            chip8->I = nnn;
            break;
        case 0xB000:
        case 0xC000:
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
        case 0xF000:
    }
}
