#pragma once
#include <stdint.h>
#include <SDL2/SDL.h>

typedef struct CHIP8 {
uint16_t opcode;
uint8_t memory[4096];
uint8_t V[16];
uint16_t I;
uint16_t pc;
uint8_t graphics[64 * 32];
uint8_t delay_timer;
uint8_t sound_timer;
uint16_t stack[16];
uint16_t sp;
uint8_t keypad[16];
} CHIP8;

#define MERGE_BYTES(hb, lb) (hb << 8 | lb)