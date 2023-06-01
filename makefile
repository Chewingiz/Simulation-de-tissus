all : simulation

simulation : structs.c
	gcc -Wall -Wextra -Wpedantic struct.h structs.c -o simulation
forces : forces.c
	gcc -Wall -Wextra -Wpedantic struct.h forces.c -o forces -lm
clean : 
	rm simulation forces *.o
