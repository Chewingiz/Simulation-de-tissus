all : main

structs.o : structs.c structs.h
	gcc -Wall -Wextra -Wpedantic -c structs.c 
	
main : structs.o main.c
	gcc -Wall -Wextra -Wpedantic main.c structs.o -o simulation
forces : structs.o forces.c
	gcc -Wall -Wextra -Wpedantic struct.h forces.c structs.o -o forces -lm
test : structs.c main.c
	gcc -Wall -Wextra -Wpedantic -c structs.c
	gcc -Wall -Wextra -Wpedantic -c main.c
	gcc structs.o main.o -o simulation
clean : 
	rm simulation forces *.o
	
