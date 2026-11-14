all: banker

banker: main.c entrada.c entrada.h
	gcc -o banker main.c entrada.c

clean:
	rm -f banker result.txt