CC = clang

TARGET = aes128
SRC_DIR = .
OBJ_DIR = .
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o ,$(SRC_FILES))

$(TARGET): $(OBJ_FILES)
	$(CC) -g -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c -o $@ $<

clean:
	rm $(TARGET) $(OBJ_FILES)
