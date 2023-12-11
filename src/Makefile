# Makefile  C

# Directories
SRC_DIR = c
INC_DIR = h
BUILD_DIR = ../build
DIST_DIR = ../dist

# Source code and object
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Name of project
TARGET = $(DIST_DIR)/ciso8583

# Compiler and options flag "no warning"
CC = gcc
CFLAGS = -I$(INC_DIR)

# Main executable program
all: $(TARGET)

# gcc --all
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# gcc --code
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Regla para limpiar
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET) ../dist/*.tgz ../dist/*.tar
