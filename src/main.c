#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_pwd() {
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Current directory: %s\n", cwd);
  } else {
    perror("Get cwd failed");
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  const char *rom_path = "../roms/2-ibm-logo.ch8";

  printf("\033[2J");   // clear once at start
  printf("\033[?25l"); // hide cursor

  print_pwd();
  Chip8 cpu;
  chip8_init(&cpu);
  chip8_load_rom(&cpu, rom_path);

  while (true) {
    chip8_cycle(&cpu);
  }
}
