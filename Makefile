all: banker

banker: banker.c
	gcc -o banker banker.c

clean:
	rm -f banker result.txt
