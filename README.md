# NES Emulator

A modern C++ implementation of a Nintendo Entertainment System (NES) emulator.

## Build Instructions

```bash
mkdir -p build 
cd build
cmake ..
make
```

## Future Work

After completing the standard NES emulation, want to explore;

**Esoteric CPU Emulation**: Implementing 6502 instruction set using unconventional Turing-complete systems
- **MOV-only Computing**: Exploring single-instruction architectures
- **Filesystem-based Computing**: Using `find` + `mkdir` as computational primitives

## Status

🚧 **Work in Progress** - Currently implementing core 6502 CPU functionality and bus device infra.
