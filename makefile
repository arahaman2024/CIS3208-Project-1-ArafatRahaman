MYps: MYps.o options.o allProcs.o
	gcc -o MYps MYps.o options.o allProcs.o -Wall -Werror

MYps.o: MYps.c options.h allProcs.h
	gcc -c MYps.c -Wall -Werror

options.o: options.c
	gcc -c options.c -Wall -Werror

allProcs.o: allProcs.c
	gcc -c allProcs.c -Wall -Werror

clean:
	rm -rf *.o main