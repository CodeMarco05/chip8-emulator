#include "chip8.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

static volatile int keepRunning = 1;

void INThandler(int sig) {
  char c;

  signal(sig, SIG_IGN);
  printf("Shutting down");
  printf("\x1b[?25h"); // show cursor
  exit(0);
}

int main(int argc, char *argv[]) {

  signal(SIGINT, INThandler);

  const char *rom_path = "../roms/chip8-logo.ch8";

  printf("\033[2J");   // clear once at start
  printf("\033[?25l"); // hide cursor

  print_pwd();
  Chip8 cpu;
  chip8_init(&cpu);
  chip8_load_rom(&cpu, rom_path);

  time_t cycle_start_time;
  time_t cycle_end_time;

  while (true) {
    time(&cycle_start_time);
    chip8_cycle(&cpu);
    time(&cycle_end_time);
    if (cycle_end_time - cycle_start_time < 5) {
      usleep(1000 * (10 - (cycle_end_time - cycle_start_time)));
    }
  }
}
