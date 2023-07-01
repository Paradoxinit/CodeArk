all: hw5 clean

hw5: minishell.o
	gcc minishell.o -g
minishell.o: minishell.c minishell.h
	gcc -c minishell.c -g
clean:
	rm -f *.o hw5