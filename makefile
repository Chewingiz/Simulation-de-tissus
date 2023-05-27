all : simulation

simulation : structs.c
	gcc -Wall -Wextra -Wpedantic structs.c  -o simulation
clean : 
	rm simulation *.o