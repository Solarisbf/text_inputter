CC 	:= /opt/iwmmxt-1.0.0/bin/arm-linux-gcc
TEST_TARGET = hw3
TEST_OBJS = Main.o MakeCharacter.o TextEditer.o drFont.o
TEST_SRCS = Main.c MakeCharacter.c MakeCharacter.h TextEditer.c TextEditer.h drFont.c drFont.h
$(TEST_TARGET) : $(TEST_OBJS)
	$(CC) -o $@ $(TEST_OBJS)
clean:
	rm -rf *.o 
	rm -f $(TEST_TARGET)

