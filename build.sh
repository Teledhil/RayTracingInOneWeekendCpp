#!/bin/bash

g++ -o rtx -std=c++17 -Wall -Wextra -pedantic -O3 -ffast-math -march=native rtx.cc -lpthread

# -Og -ggdb -glldb -fsanitize=address -fno-omit-frame-pointer
