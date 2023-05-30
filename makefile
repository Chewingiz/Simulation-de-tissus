all : simulation

simulation : structs.c
	gcc -Wall -Wextra -Wpedantic structs.c  -o simulation
forces : forces.c
	gcc -Wall -Wextra -Wpedantic -lm forces.c  -o forces
clean : 
	rm simulation forces *.o
