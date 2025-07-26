CC := clang++-14




test_had_unicodeStringView:
	$(CC) $(DFLAGS) unicodeStringView.cpp unicodeStringView.test.cpp \
		$(LIBS) -o ../../execs/test_had_unicode.out
	../../execs/test_had_unicode.out

test_stringEditor:
	$(CC) -std=c++20 ../had/had_interface.cpp \
		stringEditor.cpp stringEditor.test.cpp \
		unicodeStringView.cpp unicodeString.cpp \
		-lncursesw -o ../../execs/test_stringEditor.out
	alacritty -e ./../../execs/test_stringEditor.out
