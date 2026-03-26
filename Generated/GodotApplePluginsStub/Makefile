CC ?= cc
GODOT_HEADERS_DIR ?= /path/to/godot-cpp/gdextension
CFLAGS ?= -std=c11 -Wall -Wextra

ifeq ($(OS),Windows_NT)
SHARED_EXT := dll
SHARED_FLAGS := -shared
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
SHARED_EXT := dylib
SHARED_FLAGS := -dynamiclib
else
SHARED_EXT := so
SHARED_FLAGS := -shared -fPIC
endif
endif

LIBRARY := godot_apple_plugins_stub.$(SHARED_EXT)

.PHONY: all clean

all: $(LIBRARY)

$(LIBRARY): godot_apple_plugins_stub.c
	$(CC) $(CFLAGS) -I "$(GODOT_HEADERS_DIR)" $(SHARED_FLAGS) $< -o $@

clean:
	rm -f $(LIBRARY)