CC = gcc
CC_FLAGS = -c -g -Wall -Wextra -Wno-unknown-pragmas  -lstdc++
LDFLAGS=-lm
# File names
EXEC = run
ODIR = out
SOURCES = $(wildcard *.c *.cpp)
OBJECTS = $(SOURCES:.c=.o)
 
# Main target
$(EXEC): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $(ODIR)/$(EXEC) $(LDFLAGS)
 
# To obtain object files
%.o: %.c
	$(CC) -c -Wall -Wextra $(CC_FLAGS) $< -o $@
 
# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
