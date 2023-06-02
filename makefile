all : main

structs.o : structs.c structs.h
	gcc -Wall -Wextra -Wpedantic -c structs.c 
forces.o : forces.c forces.h
	gcc -Wall -Wextra -Wpedantic -lm -c forces.c 
	
main : structs.o forces.o main.c
	gcc -Wall -Wextra -Wpedantic main.c structs.o forces.o -o simulation -lm

test : structs.c main.c
	gcc -Wall -Wextra -Wpedantic -c structs.c
	gcc -Wall -Wextra -Wpedantic -c main.c
	gcc structs.o main.o -o simulation
clean : 
	rm simulation  *.o
	
