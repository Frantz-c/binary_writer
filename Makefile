OBJ  = binw.o octal.o hexa.o binary.o
NAME = binw

all: $(OBJ)
	gcc -O3 -Wall -Wextra $^ -o $(NAME)

%.o: %.c
	gcc -O3 -Wall -Wextra -c $<

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: clean all
