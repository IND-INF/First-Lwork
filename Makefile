.PHONY: clean
CFLAGS = -Iinclude -Wall -g

main : *.o
	gcc *.o -o main

%.o : %.c
	gcc $(CGLAGS) -c $< -o $@

clean:
	del *.o main