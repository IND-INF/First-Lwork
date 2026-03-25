.PHONY: clean
CFLAGS = -Iinclude -Wall -g

main : *.o
	gcc *.o -o main

%.o : %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	del *.o main
