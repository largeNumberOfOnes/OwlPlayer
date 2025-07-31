CC := clang++-14
DFLAGS := -std=c++20 -g



test_had_unicodeString:
	$(CC) $(DFLAGS) unicodeString.cpp unicodeString.test.cpp \
		$(LIBS) -o ../../execs/test_unicodeString.out
	../../execs/test_unicodeString.out

test_stringEditor:
	$(CC) $(DFLAGS) ../had/had_interface.cpp \
		stringEditor.cpp stringEditor.test.cpp \
		unicodeString.cpp \
		-lncursesw -o ../../execs/test_stringEditor.out
	# alacritty -e valgrind --log-file=log.txt ./../../execs/test_stringEditor.out
	alacritty -e ./../../execs/test_stringEditor.out
