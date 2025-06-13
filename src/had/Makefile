BUILD_DIR := .
# CC := clang++-14
CC := g++
LD := ld
# WARNINGS := -Wall -Wextra -Wpedantic -Werror -fsanitize=address \
WARNINGS := -Wall -Wextra -Werror -fsanitize=address \
															-fno-exceptions
DFLAGS := -g -DBUILD_DEB -std=c++20 $(WARNINGS)
OUTPUT_NAME := had.o

LIBS := -lsndfile -lmpg123 -lasound -lncursesw \
		 $(shell pkg-config --cflags --libs libpipewire-0.3)

INCLUDES := $(shell pkg-config --cflags libpipewire-0.3)

get_libs_list:
	@echo $(LIBS)

get_include_list:
	@echo $(INCLUDES)

SOURCE = $(shell ls | grep cpp | grep -v test | sed "s/^/had\//")
get_source_files:
	@echo $(SOURCE)

# build:
# 	mkdir had_tmp
# 	$(CC) $(FLAGS) -c had.cpp -o had_tmp/had.o \
# 		&& $(CC) $(FLAGS) -c audioFile.cpp -o had_tmp/audioFile.o \
# 		&& $(LD) -r had_tmp/had.o had_tmp/audioFile.o \
# 										-o $(BUILD_DIR)/$(OUTPUT_NAME) \
# 		&& rm -r had_tmp \
# 		|| (rm -r had_tmp && false)

# test_had:
# 	$(CC) $(FLAGS) had.cpp audioFile.cpp had.test.cpp $(LIBS) -o test_had.out

# test_had:
# 	$(CC) $(FLAGS) audioFile.cpp audioFile.test.cpp $(LIBS) -o test_audioFile.out


# SOURCE = $(shell ls | grep cpp | grep -v test)
comp:
	echo $(SOURCE)
	cd src; $(CC) $(DFLAGS) $(SOURCE) $(LIBS) -o ../execs/output.out


# Tests

test_audioFile:
	$(CC) $(DFLAGS) audioFile.cpp audioFile.test.cpp $(LIBS) \
		-o ../../execs/test_audioFile.out

test_had_audio:
	$(CC) $(DFLAGS) had_audio.cpp audioFile.cpp had_audio.test.cpp \
		$(LIBS) -o ../../execs/test_had_audio.out

test_had_keys:
	$(CC) $(DFLAGS) had_interface.cpp had_keys.test.cpp \
		$(LIBS) -o ../../execs/test_had_keys.out
	alacritty -e ../../execs/test_had_keys.out &

test_had_colors:
	$(CC) $(DFLAGS) had_interface.cpp had_colors.test.cpp \
		$(LIBS) -o ../../execs/test_had_colors.out
	alacritty -e ../../execs/test_had_colors.out &
