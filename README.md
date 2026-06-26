# chip8-emulator

A small experiment to understand how emulators work. Wanted to get a feel for the basics, fetch, decode, execute before
tackling something more complex like the Intel 8086.

CHIP-8 is a good starting point since the instruction set is tiny and there's a well-known test ROM (the IBM logo) to
verify things are working.

Only a subset of opcodes is implemented. Might flesh it out more later, but the main goal was just to get something
running.

## Building

```sh
cmake -B build && cmake --build build
```

## Running

```sh
./build/chip8
```

## Resources

- [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [chip8-test-suite](https://github.com/Timendus/chip8-test-suite)
