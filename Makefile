CC = clang++-14
# FLAGS = -std=c++20 -lncursesw -lasound
# DEB_FLAGS = -g -std=c++17
FLAGS = -g -DBUILD_DEB -std=c++20
LIBS = -lsndfile -lmpg123 -lasound -lncursesw

# /////////////////////////////

objects/had.o:  \
	src/had.cpp \
	src/had.h
	$(CC) $(FLAGS) -c src/had.cpp -o objects/had.o

objects/intf.o:  \
	src/intf.cpp \
	src/intf.o
	$(CC) $(FLAGS) -c src/intf.cpp -o objects/intf.o

objects/player_fileManager.o:  \
	src/player/fileManager.cpp \
	src/player/fileManager.h
	$(CC) $(FLAGS) -c src/player/fileManager.cpp -o objects/fileManager.o

# objects/player_player.o:  \
# 	src/player/player.cpp \
# 	src/player/player.h
# 	$(CC) $(FLAGS) -c src/player/player.cpp -o objects/player.o

# objects/player_GUIPlayer.h.o:  \
# 	src/player/GUIPlayer.cpp \
# 	src/player/GUIPlayer.h \
# 	$(CC) $(FLAGS) -c src/player/GUIPlayer.h.cpp -o objects/GUIPlayer.h.o

objects/audioFile.o: src/audioFile.cpp src/audioFile.h
	$(CC) $(FLAGS) -c src/audioFile.cpp -o objects/audioFile.o

# -------------------------------------------------------------------- test

test_audioFile: objects/audioFile.o
	$(CC) $(FLAGS) objects/audioFile.o src/audioFile.test.cpp $(LIBS) -o ./execs/test_audioFile.out
	./execs/test_audioFile.out

test_had: objects/had.o objects/audioFile.o
	$(CC) $(FLAGS) objects/had.o objects/audioFile.o tests/test_had.cpp $(LIBS) -o ./execs/test_had.out
	alacritty -e ./execs/test_had.out &

test_player_fileManager: \
	objects/player_fileManager.o \
	objects/intf.o \
	objects/had.o
	$(CC) $(FLAGS) objects/fileManager.o objects/had.o objects/intf.o tests/test_player_fileManager.cpp -o execs/test_player_fileManager.out
	alacritty -e ./execs/test_player_fileManager.out &

comp_and_run:
	$(CC) $(FLAGS) src/main.cpp src/had.cpp src/intf.cpp src/player/player.cpp src/player/fileManager.cpp src/player/GUIPlayer.cpp -o execs/a.out
	alacritty -e ./execs/a.out &




# -fno-exceptions !!!
