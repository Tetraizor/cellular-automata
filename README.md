# Cellular Automata

A real-time falling sand simulation built in C++ with SDL2. Materials interact with each other through simple per-cell physics rules, producing emergent behaviour like sand piling, water levelling, and gas rising.

<video src="https://github.com/user-attachments/assets/18ff5b9e-74f3-453c-aac6-ec55e5e5a752" autoplay loop muted playsinline></video>

---

## Materials

| Material  | Behaviour                                                 |
| --------- | --------------------------------------------------------- |
| **Sand**  | Falls downward, slides off surfaces, sinks through water  |
| **Stone** | Static — acts as a wall or platform                       |
| **Water** | Flows and levels out, displaced upward by sand            |
| **Gas**   | Rises and disperses in all directions, floats above water |

<video src="https://github.com/user-attachments/assets/e2a1ff94-39de-412d-b1fe-23c906863b2e" autoplay loop muted playsinline></video>

---

## Controls

| Input                  | Action                  |
| ---------------------- | ----------------------- |
| **Left click + drag**  | Place selected material |
| **Right click + drag** | Erase material          |
| **Space**              | Clear the world         |
| **Escape**             | Quit                    |

Use the right-side panel to select the active material and brush size.

<video src="https://github.com/user-attachments/assets/013fc6d3-f39a-4393-a22e-180499434188" autoplay loop muted playsinline></video>

---

## Building

**Dependencies:** SDL2, SDL2_ttf, CMake ≥ 3.15, a C++17 compiler.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./out/CellularAutomata
```

You can pass custom world dimensions and zoom as command-line arguments:

```bash
./out/CellularAutomata <width> <height>
./out/CellularAutomata <width> <height> <zoom>
# Default: 600 400 2
```

---

## Project structure

```
src/          — simulation loop, rendering, input, world update
include/      — headers (physics, world, UI, engine)
assets/fonts/ — pixel font used for the UI
assets/gifs/  — demo GIFs (see above)
```
