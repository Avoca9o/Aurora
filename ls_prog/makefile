FLGS = -fsanitize=address


main: main.o ls.o checker
	gcc $(FLGS) main.o ls.o -o main

main.o: main.c
	gcc -c main.c -o main.o
  
ls.o: ls.c
	gcc -c ls.c -o ls.o
	
checker:
	cppcheck main.c ls.c
