CC=gcc
CFLAGS= -std=c99 -Wall
LIBS= -lm -lreadline
OBJECTS= main.o brainfuck.o repl.o
TARGET=bf

all: $(TARGET)

.PHONY: clean

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $(TARGET)
	
%.o : %.c
	$(CC) $(CFLAGS) -c $<
	

clean:
	rm -f *.o
	rm -f $(TARGET)
