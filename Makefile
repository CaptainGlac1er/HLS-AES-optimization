CC = gcc
CC_FLAGS = -c -g -Wall -Wextra -Wno-unknown-pragmas
LDFLAGS=-lm
# File names
EXEC = run
ODIR = out
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
 
# Main target
$(EXEC): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $(ODIR)/$(EXEC) $(LDFLAGS)
	make clean
 
# To obtain object files
%.o: %.c
	$(CC) -c -Wall -Wextra $(CC_FLAGS) $< -o $@
 
# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
