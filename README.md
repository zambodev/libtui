[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# libtui
A cross platform library that aims to simplify TUIs creation

## Compile for development
```bash
OS=<win64|linux64> COS=<windows|linux> make <testFile>.cpp run
```

## How to use

Dwonload the [latest release](https://github.com/zambodev/libtui/releases/latest) and add **include** and **lib** folders in the project folder

Include:
```cpp
// C++ example
#include <tui.hpp>
```
Read the [documentation](docs.md)

Link:
```bash
c++ -o <output> <your_files> -ltui -Llib -Iinclude
```
## Build from source
Clone the repository [link](https://github.com/zambodev/libtui.git)

Compile and create the static archive
```bash
make
```
