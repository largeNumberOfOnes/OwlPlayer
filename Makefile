
CC = clang++-14


# %.test.out: %.test.cpp %.cpp %.h
# 	$(CC) $(TFLAGS) $< -o $@




tetd:
	$(CC) src/eventQueue.cpp src/eventQueue.test.cpp src/eventQueue.hpp
	./a.out
