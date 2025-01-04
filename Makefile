CC = clang++-14
DEB_FLAGS = -g -std=c++17
# FLAGS = -g
OBJECTS =   src/player/main.cpp\
			src/player/intreface.cpp\
			src/player/manager.cpp\
			src/player/abstract.cpp
LIBS = -lncurses
OUT = a.out

comp_deb:
	$(CC) $(DEB_FLAGS) $(OBJECTS) $(LIBS) -o $(OUT)

run:
	./$(OUT)

cr: comp_deb run
	echo start