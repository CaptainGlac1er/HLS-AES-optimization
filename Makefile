CC = gcc
CC_FLAGS = -c -g -Wall -Wextra -Wno-unknown-pragmas
LDFLAGS=-lm
# File names
EXEC = run
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
 
# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LDFLAGS)
 
# To obtain object files
%.o: %.c
	$(CC) -c -Wall -Wextra $(CC_FLAGS) $< -o $@
 
# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
