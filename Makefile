all:
	gcc -O3 -Wall -Wextra binw.c -o binw

re: all
	/bin/rm -f binw
