CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -Itest

SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
OBJ_DIR = obj
TEST_DIR = test

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
SRC_OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ_FILES = $(TEST_FILES:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)

TARGET = $(BIN_DIR)/program
TEST_TARGET = $(BIN_DIR)/test_program

.PHONY: all run clean rebuild test

all: $(TARGET)

$(TARGET): $(SRC_OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Каталоги
$(BIN_DIR):
	@mkdir $(BIN_DIR) 2>nul || exit 0

$(OBJ_DIR):
	@mkdir $(OBJ_DIR) 2>nul || exit 0

# Запуск основной программы
run: $(TARGET)
	./$(TARGET)

# Сборка и запуск тестов
test: $(TEST_TARGET)
	./$(TEST_TARGET)

SRC_OBJ_TEST = $(filter-out $(OBJ_DIR)/main.o, $(SRC_OBJ_FILES))

$(TEST_TARGET): $(SRC_OBJ_TEST) $(TEST_OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	@if exist $(OBJ_DIR) rmdir /S /Q $(OBJ_DIR)
	@if exist $(BIN_DIR) rmdir /S /Q $(BIN_DIR)

rebuild: clean all
