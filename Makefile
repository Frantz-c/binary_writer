OBJ    = objects/binw.o objects/octal.o objects/hexa.o objects/binary.o
NAME   = binw
CFLAGS = -O3 -Wall -Wextra -I includes
CC     = gcc

all: objects $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

objects:
	mkdir -p objects

objects/%.o: sources/%.c includes/binw.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: clean all
