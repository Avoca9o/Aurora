sobel: main.o sobel.o checker
	gcc -Wall -Wextra -Wcast-align -fstack-protector -fsanitize=address -lm main.o sobel.o -o sobel
	
main.o: main.c
	gcc -c main.c -o main.o
	
sobel.o: sobel.c
	gcc -c sobel.c -o sobel.o
	
checker:
	cppcheck main.c sobel.c
