CC := clang++-14
# DEB_FLAGS = -g -std=c++17
WARNINGS := -Wall -Wextra -Wpedantic -Werror -fsanitize=address \
															-fno-exceptions
FLAGS := -g -DBUILD_DEB -std=c++20 # $(WARNINGS)
LIBS := $(shell make -s -C src/had get_libs_list)

# /////////////////////////////

objects/had.o: $(wildcard src/had/*)
	make -C src/had build BUILD_DIR=../../objects

objects/intf.o:
	$(CC) $(FLAGS) -c src/GUIPlayer/intf.cpp -o objects/intf.o

objects/player_fileManager.o:
	$(CC) $(FLAGS) -c src/player/fileManager.cpp -o objects/fileManager.o

# objects/player_player.o: 
# 	$(CC) $(FLAGS) -c src/player/player.cpp -o objects/player.o

# objects/player_GUIPlayer.h.o:  \
# 	src/player/GUIPlayer.cpp \
# 	src/player/GUIPlayer.h \
# 	$(CC) $(FLAGS) -c src/player/GUIPlayer.h.cpp -o objects/GUIPlayer.h.o

# -------------------------------------------------------------------- test

test_audioFile: objects/audioFile.o
	$(CC) $(FLAGS) objects/audioFile.o src/audioFile.test.cpp $(LIBS) -o ./execs/test_audioFile.out
	./execs/test_audioFile.out

test_had: objects/had.o objects/audioFile.o
	$(CC) $(FLAGS) objects/had.o objects/audioFile.o tests/test_had.cpp $(LIBS) -o ./execs/test_had.out
	# alacritty -e ./execs/test_had.out &

test_player_fileManager: \
	objects/player_fileManager.o \
	objects/intf.o \
	objects/had.o
	$(CC) $(FLAGS) objects/fileManager.o objects/had.o objects/intf.o tests/test_player_fileManager.cpp -o execs/test_player_fileManager.out
	alacritty -e ./execs/test_player_fileManager.out &

# ------------------------------------------------------------------- final

OBJECTS := objects/had.o

comp: $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) $(LIBS) -o execs/a.out

comp_and_run:
	$(CC) $(FLAGS) src/main.cpp src/had.cpp src/audioFile.cpp src/intf.cpp src/player/player.cpp src/player/fileManager.cpp src/player/GUIPlayer.cpp $(LIBS) -o execs/a.out
	alacritty -e ./execs/a.out &

# -fno-exceptions !!!
# потоковые редакторы текста
# sed
# awk
# cut