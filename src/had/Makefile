BUILD_DIR := .
CC := clang++-14
LD := ld
WARNINGS := -Wall -Wextra -Wpedantic -Werror -fsanitize=address \
															-fno-exceptions
FLAGS := -g -DBUILD_DEB -std=c++20 $(WARNINGS)
OUTPUT_NAME := had.o

LIBS := -lsndfile -lmpg123 -lasound -lncursesw

get_libs_list:
	@echo $(LIBS)

build:
	mkdir had_tmp
	- $(CC) $(FLAGS) -c had.cpp -o had_tmp/had.o \
	&& $(CC) $(FLAGS) -c audioFile.cpp -o had_tmp/audioFile.o \
	&& $(LD) -r had_tmp/had.o had_tmp/audioFile.o \
											-o $(BUILD_DIR)/$(OUTPUT_NAME)
	rm -r had_tmp
