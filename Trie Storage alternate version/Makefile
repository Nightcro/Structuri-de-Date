CC = gcc
CFLAG = -Wall -Wextra -g
EXE = tema3
OBJ = trie.o tema3.o
DEPS = trie.h

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