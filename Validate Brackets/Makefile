CC = gcc
CFLAG = -Wall -Wextra -Werror -g
EXE = tema2
OBJ = common.o lista.o stack.o queue.o tema2.o 
DEPS = common.h lista.h stack.h queue.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAG)

build: $(OBJ)
	$(CC) $^ $(CFLAG) -o $(EXE)

run:
	./$(EXE) input.in output.out

.PHONY : clean

clean:
	rm -f *.o $(EXE)

.PHONY: leak

leak:
	valgrind --leak-check=full --track-origins=yes ./$(EXE) input.in output.out