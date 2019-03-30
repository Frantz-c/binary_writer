OBJ  = binw.o octal.o hexa.o
NAME = binw

all: $(OBJ)
	gcc -O3 -Wall -Wextra $^ -o $(NAME)

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: clean all
	/bin/rm -f $(NAME)
