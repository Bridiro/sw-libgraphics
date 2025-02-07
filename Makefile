TARGET = app

OS = $(shell uname -s)

# Flags and compiler
CC = gcc
CFLAGS = -Iinc -Ilibs/sw-libfont/inc -Wall -Wextra
DEBUG_CFLAGS = "-g"
LDFLAGS = -lSDL2 -lm 

ifeq ($(OS), Darwin) 
	CFLAGS += -I/opt/homebrew/Cellar/sdl2/2.30.10/include/SDL2
	LDFLAGS += -L/opt/homebrew/Cellar/sdl2/2.30.10/lib
endif

# Directory
BUILD_DIR = build

# Sources and objects
SOURCES = test/main.c src/graphics.c \
		libs/sw-libfont/src/font.c \
		libs/sw-libfont/src/text.c
		
OBJECTS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SOURCES)))

vpath %.c src test libs/sw-libfont/src

# Default build
all: $(TARGET)

# Build with debug flags
debug: CFLAGS += $(DEBUG_CFLAGS)
debug: $(TARGET)

# Executable build
$(TARGET): $(OBJECTS)
	@echo "Building for target: $(OS)"
	$(CC) $(OBJECTS) -o $@ $(CFLAGS) $(LDFLAGS)

# Object file compilation
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# Build dir creation
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	@echo "Cleaning up"
	rm -rf $(BUILD_DIR) $(TARGET) app.dSYM
	
test: $(TARGET)
	./$<

.PHONY: all clean

