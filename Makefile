
CC := clang++-14
DFLAGS := -g

FLAGS = $(DFLAGS)

# OBJECTS = eventQueue.o app.o

# %.test.out: %.test.cpp %.cpp %.h
# 	$(CC) $(DFLAGS) $^ -o $@

# %.o: %.cpp %.h
# 	$(CC) $(FLAGS) -c $< -o $@

# comp_deb: $(OBJECTS)


SOURCE = $(shell ls src | grep cpp | grep -v test)
HAD_SOURCE = $(shell make -s -C src/had get_source_files)
HAD_LIBS = $(shell make -s -C src/had get_libs_list)
comp:
	echo $(HAD_SOURCE)
	cd src; $(CC) $(DFLAGS) $(SOURCE) $(HAD_SOURCE) $(HAD_LIBS) -o ../execs/output.out

car: comp
	alacritty -e ./execs/output.out &

tetd:
	$(CC) src/eventQueue.cpp src/eventQueue.test.cpp src/eventQueue.hpp
	./a.out

lint:
	cpplint src/*
