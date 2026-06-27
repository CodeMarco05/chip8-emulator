#pragma once

#include <stdbool.h>
#include <stdint.h>

#define MEMORY_SIZE 4096
#define DISPLAY_WIDTH 32
#define DISPLAY_HEIGHT 64
#define NUM_REGISTER 16 // V0-VF general purpose registers
#define STACK_SIZE 16

typedef struct {
  uint8_t memory[MEMORY_SIZE];
  uint8_t V[NUM_REGISTER];
  uint16_t I;  // used to point at locations in memory
  uint16_t PC; // programmcounter
  uint16_t SP; // stack pointer
  uint8_t stack[STACK_SIZE];
  uint8_t delay_timer;
  uint8_t sound_timer;
  bool display[DISPLAY_WIDTH][DISPLAY_HEIGHT];
  bool redraw_flag;
} Chip8;

void chip8_init(Chip8 *cpu);
void chip8_load_rom(Chip8 *cpu, const char *path);

// core
void chip8_cycle(Chip8 *cpu);
