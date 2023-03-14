CC=g++
CFLAGS=-I.
DEPS =  process.hpp generators.hpp semaphore.hpp generators.hpp scheduler.hpp
OBJ = main.o process.o semaphore.o scheduler.o	generators.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

exe: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

