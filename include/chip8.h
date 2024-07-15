#pragma once
#include <stdint.h>

typedef struct CHIP8 {
uint16_t opcode;
uint8_t memory[4096];
uint8_t V[16];
uint16_t I;
uint16_t pc;
uint8_t graphics[2048];
uint8_t delay_timer;
uint8_t sound_timer;
uint16_t stack[16];
uint16_t sp;
uint8_t keypad[16];
uint8_t fontset[80];
uint8_t draw_flag;
uint8_t sound_flag;
} CHIP8;

void chip8_init(CHIP8 *chip8);
void chip8_set_keypad(CHIP8 *chip8);
void chip8_emu_cycle(CHIP8 *chip8);
void chip8_execute_instruction(CHIP8 *chip8);
void chip8_load_rom(CHIP8 *chip8, char* file_name);

#define MERGE_BYTES(hb, lb) (hb << 8 | lb) // 0x[hblb]
#define NNN_MASK(opcode) (opcode & 0x0FFF) // nibbles 2, 3, 4
#define NN_MASK(opcode) (opcode & 0x00FF) // nibbles 3, 4 
#define N_MASK(opcode) (opcode & 0x000F) // nibble 4
#define X_MASK(opcode) ((opcode & 0x0F00) >> 8) // nibble 2
#define Y_MASK(opcode) ((opcode & 0x00F0) >> 4) // nibble 3