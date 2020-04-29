all: main
	gcc -o main obj/map.o obj/main.o obj/err.o -Wall

main: main.c map
	gcc -c main.c -o obj/main.o -Wall

map: map.c map.h err
	gcc -c map.c -o obj/map.o -Wall

err: err.c err.h base
	gcc -c err.c -o obj/err.o -Wall

clean:
	rm -f main
	rm -r obj
base:
	if [ -d "obj" ]; then \
		echo "dir exist"; \
	else \
		mkdir obj; \
	fi