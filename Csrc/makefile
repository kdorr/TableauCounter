PROG = main
CC = gcc
CFLAGS = -Wall  -g  -fopenmp 
LFLAGS = -o exec.out

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) $(PROG).c $(LFLAGS)

clean:
	rm -f $(PROG) exec.out *~ *# *.o

