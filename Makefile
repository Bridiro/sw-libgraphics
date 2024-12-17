# Nome dell'eseguibile
TARGET = app
FONTDIR = libs/McuFont/fonts
MFDIR = libs/McuFont/decoder
include $(MFDIR)/mcufont.mk

OS = $(shell uname -s)

# Compilatore e flags
CC = gcc
CFLAGS = -Iinc -I$(FONTDIR) -I$(MFDIR) -Wall -Wextra -g 
LDFLAGS = -lSDL2 -lm 

ifeq ($(OS), Darwin) 
	CFLAGS += -I/opt/homebrew/Cellar/sdl2/2.30.10/include/SDL2
	LDFLAGS += -L/opt/homebrew/Cellar/sdl2/2.30.10/lib
endif

# Directory
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = build

# File sorgenti e oggetti
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Regola di default
all: $(TARGET)

# Creazione dell'eseguibile
$(TARGET): $(OBJECTS)
	@echo "Building for target: $(OS)"
	$(CC) $(OBJECTS) -o $@ $(MFSRC) $(CFLAGS) $(LDFLAGS)

# Compilazione dei file oggetto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# Creazione della directory degli oggetti
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Pulizia
clean:
	@echo "Cleaning up"
	rm -rf $(OBJ_DIR) $(TARGET)

run: $(TARGET)
	./$<

# Regole ausiliarie
.PHONY: all clean

