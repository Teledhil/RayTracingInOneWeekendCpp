#!/bin/bash

clang++ -o rtx -std=c++17 -Wall -Wextra -pedantic -O3 -ffast-math -fvisibility=hidden -fvisibility-inlines-hidden -march=native rtx.cc -lpthread
#clang++ -o rtx -std=c++17 -Wall -Wextra -pedantic -Og -ggdb -fno-omit-frame-pointer -march=native rtx.cc -lpthread
#clang++ -o rtx -std=c++17 -Wall -Wextra -pedantic -Og -ggdb -fsanitize=address -fno-omit-frame-pointer -march=native rtx.cc -lpthread

# -Og -ggdb -glldb -fsanitize=address -fno-omit-frame-pointer
