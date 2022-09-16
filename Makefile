CC=g++
CFLAGS=-I.
DEPS = process.h generators.h semaphore.h
OBJ = main.o process.o  semaphore.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

exe: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	