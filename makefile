all : simulation

simulation : structs.c
	gcc -Wall -Wextra -Wpedantic structs.c  -o simulation
forces : forces.c
	gcc -Wall -Wextra -Wpedantic forces.c  -o forces -lm
clean : 
	rm simulation forces *.o
