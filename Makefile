CXX := clang++

SRCS := $(shell find . -name '*.cc' -or -name '*.h')
CPPFLAGS := -std=c++17 -Wall -Wextra -pedantic -O3 -ffast-math -fvisibility=hidden -fvisibility-inlines-hidden -march=native
DBGCPPFLAGS := -std=c++17 -Wall -Wextra -pedantic -Og -ggdb -glldb -fsanitize=address -fno-omit-frame-pointer

all: rtx

rtx: $(SRCS)
	$(CXX) -o rtx $(CPPFLAGS) rtx.cc -lpthread

debug: $(SRCS)
	$(CXX) -o rtx_dbg $(DBGCPPFLAGS) rtx.cc -lpthread

.PHONY: clean
clean:
	rm -f rtx rtx_dbg
