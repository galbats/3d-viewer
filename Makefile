CC := gcc
CFLAGS := -Wall -Werror -Wextra -std=c++17
BUILD_DIR := build
TEST_BUILD_DIR := test_build

GTEST_LIB := $(shell pkg-config --libs gtest)
INCLUDE := $(shell pkg-config --cflags gtest)
GCOV_FLAGS := -fprofile-arcs -ftest-coverage

MODEL_DIR := backend
MODEL_SRC := $(wildcard $(MODEL_DIR)/*.cpp)
MODEL_OBJ := $(patsubst $(MODEL_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(MODEL_SRC))
APP_FILE := 3DViewer_v2

TEST_DIR := tests
TEST_SRC := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ := $(patsubst $(TEST_DIR)/%.cpp,$(TEST_BUILD_DIR)/%.o,$(TEST_SRC))
TEST_EXEC := test

OS := $(shell uname -s)

ifeq ($(OS),Linux)
LDFLAGS += -lstdc++ -pthread -lrt -lm
OPEN_CMD := xdg-open
else ifeq ($(OS),Darwin)
LDFLAGS += -lstdc++ -lm
OPEN_CMD := open
endif

all: clean install

install:
	@mkdir -p $(BUILD_DIR)
	@qmake6 -o $(BUILD_DIR)/Makefile frontend/3DViewer_v2.pro
	@make -C $(BUILD_DIR)
	@mv $(BUILD_DIR)/3dViewer_v2.app/Contents/MacOS/$(APP_FILE) $(BUILD_DIR)/
	@rm -rf $(BUILD_DIR)/3dViewer_v2.app
	@make -C $(BUILD_DIR) clean
	@rm -rf $(BUILD_DIR)/.qmake.stash $(BUILD_DIR)/Makefile

uninstall:
	@rm -rf $(BUILD_DIR)

dvi:
	@$(OPEN_CMD) doc/documentation.md

dist:
	@tar -czvf 3DViewer_v2.tgz $(BUILD_DIR)/$(APP_FILE)

gcov_report: tests
	@lcov --capture --directory . --output-file coverage.info --ignore-errors inconsistent,corrupt
	@lcov --remove coverage.info '/usr/*' '/opt/*' --output-file coverage.info --ignore-errors inconsistent,corrupt
	@lcov --list coverage.info --ignore-errors inconsistent,corrupt
	@genhtml coverage.info --output-directory report --ignore-errors inconsistent,corrupt
	@rm -f *.gcno *.gcda *.info
	@$(OPEN_CMD) ./report/index.html &

$(BUILD_DIR)/%.o: $(MODEL_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $(GCOV_FLAGS) $(INCLUDE) -c $< -o $@

$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	@$(CC) $(CFLAGS) $(GCOV_FLAGS) $(INCLUDE) -c $< -o $@

tests: clean $(MODEL_OBJ) $(TEST_OBJ)
	@$(CC) $(CFLAGS) $(GCOV_FLAGS) $(MODEL_OBJ) $(TEST_OBJ) -o $(TEST_BUILD_DIR)/$(TEST_EXEC) $(GTEST_LIB) $(LDFLAGS)
	@./$(TEST_BUILD_DIR)/$(TEST_EXEC)
	@rm -rf $(MODEL_OBJ) $(TEST_OBJ)

clean:
	@rm -rf $(TEST_EXEC)
	@rm -rf report build test_build *.o
	@rm -rf *.gcno *.gcda *.info *.tgz

.PHONY: all install uninstall dvi dist gcov_report tests clean
