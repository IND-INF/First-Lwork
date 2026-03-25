.PHONY: clean
CFLAGS = -Iinclude -Wall -g

main : *.o
	gcc $(CFLAGS) *.o -o main

%.o : %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	del *.o main
