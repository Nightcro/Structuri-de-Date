CC = gcc
CFLAG = -Wall -Wextra -Werror -g
EXE = hash_table
OBJ =  hash.o materie.o student.o lista.o hash_table.o
DEPS = lista.h materie.h student.h hash.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAG)

build: $(OBJ)
	$(CC) $^ $(CFLAG) -o $(EXE)

run:
	./$(EXE)

.PHONY : clean

clean:
	rm -f *.o $(EXE)
