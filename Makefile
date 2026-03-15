.PHONY: clean
CFLAGS = -Iinclude -Wall -g

main : main.o test_of_the_structure.o tridiagonal_matrix.o integer_algebra.o real_number_algebra.o complex_number_algebra.o
	gcc main.o test_of_the_structure.o tridiagonal_matrix.o integer_algebra.o real_number_algebra.o complex_number_algebra.o -o main

%.o : %.c
	gcc $(CGLAGS) -c $< -o $@

clean:
	del main.o test_of_the_structure.o tridiagonal_matrix.o integer_algebra.o real_number_algebra.o complex_number_algebra.o main