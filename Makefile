# Nome dell'eseguibile
TARGET = app
FONTDIR = libs/McuFont/fonts
MFDIR = libs/McuFont/decoder
include $(MFDIR)/mcufont.mk

# Compilatore e flags
CC = gcc
CFLAGS = -Iinc -I$(FONTDIR) -I$(MFDIR) -Wall -Wextra -g -I/opt/homebrew/Cellar/sdl2/2.30.10/include/SDL2
LDFLAGS = -lSDL2 -lm -L/opt/homebrew/Cellar/sdl2/2.30.10/lib

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
	$(CC) $(OBJECTS) -o $@ $(MFSRC) $(CFLAGS) $(LDFLAGS)

# Compilazione dei file oggetto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# Creazione della directory degli oggetti
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Pulizia
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Regole ausiliarie
.PHONY: all clean

