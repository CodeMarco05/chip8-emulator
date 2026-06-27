#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void chip8_init(Chip8 *cpu) {
  memset(cpu, 0, sizeof(Chip8)); // set all to 0

  cpu->PC = 0x200; // programs starts here

  // load the font into first memory block
  uint8_t font[] = {
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
  memcpy(cpu->memory, font, sizeof(font)); // copy the font in
}

void chip8_load_rom(Chip8 *cpu, const char *path) {
  FILE *rom_file = fopen(path, "rb");
  if (rom_file == NULL) {
    perror("Failed to open file");
    exit(1);
  }

  fread(&cpu->memory[0x200], 1, MEMORY_SIZE - 0x200,
        rom_file); // where to put, 1 byte at a time
  fclose(rom_file);
}

static void clear_screen(void) {
  printf("\033[2J\033[H");
  fflush(stdout);
}

static void draw_in_mem_buff(Chip8 *cpu, uint8_t x, uint8_t y, uint8_t height) {
  for (int row = 0; row < height; row++) {
    uint8_t sprite_byte = cpu->memory[cpu->I + row];

    for (int col = 0; col < 8; col++) {
      // check each bit left to right
      if (sprite_byte & (0x80 >> col)) {
        int px = (x + col) % 64; // wrap horizontally
        int py = (y + row) % 32; // wrap vertically

        // collision pixel was on but turn off now
        if (cpu->display[py][px] == 1)
          cpu->V[0xF] = 1;

        // XOR the pixel
        cpu->display[py][px] ^= 1;
      }
    }
  }
}

static void display_render(Chip8 *cpu) {
  printf("\033[H"); // move to top, no flicker

  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < 64; x++) {
      printf(cpu->display[y][x] ? "█" : " ");
    }
    printf("\n");
  }
  fflush(stdout);
}

void chip8_cycle(Chip8 *cpu) {
  // fetch
  if (cpu->PC > sizeof(cpu->memory) || cpu->PC + 1 > sizeof(cpu->memory)) {
    fprintf(stderr, "Next memory index is out of bounds. PC max");
    exit(1);
  }
  uint16_t opcode = (cpu->memory[cpu->PC] << 8) | (cpu->memory[cpu->PC + 1]);
  // load the first byte and shift then merge with second byte
  cpu->PC += 2;

  // set nibbles for later use
  uint8_t n = (0x000F & opcode);    // fourth nibble
  uint8_t nn = (0x00FF & opcode);   // lower byte
  uint16_t nnn = (0x0FFF & opcode); // lower 12 bytes

  // execute
  switch (opcode & 0xF000) {
  case 0x0000: // clear screen
    if (opcode == 0x00E0) {
      clear_screen();
      display_render(cpu);
    }
    break;
  case 0x1000: // jump
    cpu->PC = nnn;
    break;
  case 0x6000: // load normal register with immediate value
    cpu->V[(opcode & 0x0F00) >> 8] = nn;
    break;
  case 0xA000: // load index register with immediate value
    cpu->I = nnn;
    break;
  case 0x7000: // add immediate value to normal register
    cpu->V[(opcode & 0x0F00) >> 8] = cpu->V[(opcode & 0x0F00) >> 8] + nn;
    break;
  case 0xD000:
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t vy = (opcode & 0x00F0) >> 4;
    uint8_t height = opcode & 0x000F; // N

    uint8_t x = cpu->V[vx] % 64; // wrap around screen width
    uint8_t y = cpu->V[vy] % 32; // wrap around screen height

    cpu->V[0xF] = 0; // reset collision flag
    draw_in_mem_buff(cpu, x, y, height);
    display_render(cpu);
    break;
  }
}
