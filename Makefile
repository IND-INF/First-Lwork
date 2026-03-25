.PHONY: clean
CFLAGS = -Wall -g
OBJ = main.o test_of_the_structure.o tridiagonal_matrix.o real_number_algebra.o complex_number_algebra.o

main: $(OBJ)
	gcc $(OBJ) -o main

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	del $(OBJ) main
