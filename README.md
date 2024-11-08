# raycast_sdl3
This is a simple raycasting program written in C with raycast_sdl3

![screenshot](assets/screenshot.png)

### Controls
- Use UP/DOWN arrow keys to move player backwards and forwards
- Use LEFT/RIGHT arrow keys to turn player
- Left click anywhere in the board window to add a tile
- Right click anywhere in the board window to remove a tile
- Change current type of tile with numkeys (1: brick wall, 2: brick wall with window)

### Requirements
- SDL3
- C compiler
- CMake (if not using makefile)

### Building with makefile
This should work if you're on a mac.
NOTE: The included makefile will automatically run the program
```zsh
git clone https://github.com/nfriel/raycast_v2
cd raycast_v2
make
```
If this does not work for you, ensure SDL3 is installed according to official instructions.
If you're using a PC, try changing clang to gcc

### Building with CMake
```zsh
git clone https://github.com/nfriel/raycast_v2
cd raycast_v2
mkdir build
mv assets build/assets
cmake -S src -B build
cd build
make
./nf_raycast
```
