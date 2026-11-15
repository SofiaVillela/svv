all: banker

banker: main.c entrada.c banqueiro.c entrada.h banqueiro.h
	gcc -o banker main.c entrada.c banqueiro.c

clean:
	rm -f banker result.txt