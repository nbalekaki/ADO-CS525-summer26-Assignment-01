# Advanced Database Organization - Assignment 01
# Student Makefile

CC = gcc
CFLAGS = -Wall -g

# Source files
STORAGE_SRC = storage_mgr.c dberror.c
BUFFER_SRC = buffer_mgr.c storage_mgr.c dberror.c buffer_mgr_stat.c

# Test files
TEST_STORAGE_SRC = test_storage_basic.c
TEST_BUFFER_SRC = test_buffer_basic.c

.PHONY: all clean test test-storage test-buffer

# Default target
all: test_storage_basic test_buffer_basic

# Build storage manager test
test_storage_basic: $(TEST_STORAGE_SRC) $(STORAGE_SRC)
	$(CC) $(CFLAGS) -o $@ $^

# Build buffer manager test
test_buffer_basic: $(TEST_BUFFER_SRC) $(BUFFER_SRC)
	$(CC) $(CFLAGS) -o $@ $^

# Run all tests
test: test-storage test-buffer

test-storage: test_storage_basic
	@echo "========================================"
	@echo "Running Storage Manager Tests"
	@echo "========================================"
	./test_storage_basic

test-buffer: test_buffer_basic
	@echo "========================================"
	@echo "Running Buffer Manager Tests"
	@echo "========================================"
	./test_buffer_basic

# Memory check with Valgrind
memcheck-storage: test_storage_basic
	valgrind --leak-check=full --show-leak-kinds=all ./test_storage_basic

memcheck-buffer: test_buffer_basic
	valgrind --leak-check=full --show-leak-kinds=all ./test_buffer_basic

# Clean build artifacts
clean:
	rm -f test_storage_basic test_buffer_basic
	rm -f *.o
	rm -f *.bin
