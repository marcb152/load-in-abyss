# Load in Abyss

Small pure C++ game made in 3 months with friends to learn C++.

## Installation

```bash
git clone --recurse-submodules -j2 git@github.com:marcb152/load-in-abyss.git
cd load-in-abyss
git submodule update --init --recursive
```

#### Building assimp

```bash
cd libs/assimp
cmake CMakeLists.txt -DASSIMP_NO_EXPORT=ON
cmake --build .
```

> ![NOTE]
> The first build might take a while (5 to 10 minutes), but subsequent builds will be faster.

> ![WARNING]
> Make sure to have the latest version of CMake, (make, gcc, g++) or ninja or Clang installed.

#### Versions used

C++ 23

## Features

- [x] Cross-platform: Windows, Linux (Xorg and Wayland), MacOS (Untested)
- [x] 3D

## Authors

- [@marcb152](https://www.github.com/marcb152)
- [@Okinobi]
- [@Pyy0tr](https://github.com/Pyy0tr)
- [@TomSilfCreative](https://github.com/TomSilfCreative)
- [@AMO]
- [@Matthieu]
- [@EGIDE]
- [@Boo!!]
- [@LeGauth44](https://github.com/LeGauth44)

## Inspiration & references

Huge thanks to these amazing people & projects:
- [Igneous is an open source game engine written in C++](https://github.com/MissingBitStudios/igneous)
- [CatDogEngine: A cross-platform game engine/editor written in modern C++ (WIP)](https://github.com/meta4d-me/CatDogEngine)
- [Minecraft, but it's built on top of a deferred renderer](https://github.com/jdah/minecraft-again)
- [hw3d: C++ 3D graphics engine under Direct3D 11. Developed in a Planet Chili video tutorial series that can be found on YouTube. https://youtu.be/_4FArgOX1I4](https://github.com/planetchili/hw3d)
